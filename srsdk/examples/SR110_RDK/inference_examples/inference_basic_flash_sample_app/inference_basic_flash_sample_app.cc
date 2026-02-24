/**
* SPDX-License-Identifier: Apache-2.0
*
* Copyright 2025 Synaptics Incorporated
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/


#include "inference_basic_flash_sample_app.hpp"

#include "inference_attributes.hpp"
#include "infer_job.hpp"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "logger.h"
#include "syna_device.h"

#if CONFIG_INFRA_PROFILER_ENABLED
#include "Profiler.hpp"
#endif //CONFIG_INFRA_PROFILER_ENABLED
#include "flash.h"

namespace model_flash {
//	extern const uint8_t * get_model_pointer();
//	extern size_t get_model_len();
	extern tflite::MicroOpResolver& get_resolver(void);
	extern int8_t* get_user_input_buffer(int index);
	extern int8_t* get_expected_output_buffer(int index);
} /* namespace model_flash */


/*
 * To allocate memory for the tensor arena, it's recommended to start with a large
 * number and then use the get_used_tensor_arena_size API to check the used bytes.
 * The optimal size of the tensor arena depends on the model's requirements and the
 * hardware platform. It's important to test and tune the size for your specific use
 * case to ensure the model runs efficiently and reliably.
 *
 * A good practice is to keep the tensor arena size ~1KB higher than
 * the actual used bytes. For example, if in this case the used arena size is ~14580, then keeping
 * the size at 15604 is optimal. It's also important to check that the arena used bytes
 * are not reduced after reducing the tensor arena, If it is reduced then
 * keep it a bit more higher value.
 */
static uint8_t tensor_arena_1[648000] ACTIVATION_BUF_ATTRIBUTE;

// User semaphore to be used to signal the user application from the callback when inference is finished
SemaphoreHandle_t user_model_sem;

void model_callback_with_sem(infer_job* obj, uint32_t arg, TfLiteStatus status)
{
	LOG_INFO(LOG_MOD_INFERENCE, "model callback arg : %d\n",arg);
	if (status == kTfLiteOk)
	{
		LOG_INFO(LOG_MOD_INFERENCE, "Invoke finished\n");
	}
	else
	{
		LOG_ERROR(LOG_MOD_INFERENCE, "Inference failed with error code : %d\n",status);
	}
	// Signal the application task indicating that the inference is finished
	xSemaphoreGive(user_model_sem);
}

void single_model_flash_sample_app(void *argument)
{
	/* Setup need to called before using any infer api's.*/
	/* It will create all the necessary queues and tasks needed for the api to run */
	infer_job::setup();

	int num_of_inferences = 3;
	const uint8_t* model_ptr = (uint8_t*)INFER_FLASH_MODEL_A_LOCATION;
	// Declare a inference job object
	infer_job job;

	// Lock the flash so that no other thread accesses it.
	flash_lock(0);

	// Initialize the model with the tflite location, tensor arena area and resolver
	LOG_INFO(LOG_MOD_INFERENCE, "Loading model from flash\n");
	if (!job.init(model_ptr, tensor_arena_1, sizeof(tensor_arena_1), model_flash::get_resolver()))
	{
		LOG_ERROR(LOG_MOD_INFERENCE, "Model init failed\n");
		return;
	}

	// Must release the flash to make it available for other threads
	flash_unlock(0);

	// Get the actual number of bytes being used in the provided tensor arena
	// With this info the size of the tensor arena can be reduced
	size_t arena_used = job.get_used_tensor_arena_size();
	LOG_INFO(LOG_MOD_INFERENCE, "Arena size provided : %d\n", sizeof(tensor_arena_1));
	LOG_INFO(LOG_MOD_INFERENCE, "Memory used by the model : %d bytes\n", arena_used);

	// Create semaphore used for signalling the application task from inference call back
	user_model_sem = xSemaphoreCreateBinary();
	if(user_model_sem == nullptr)
	{
		LOG_ERROR(LOG_MOD_INFERENCE, "Semaphore creation failed\n");
	}

	LOG_INFO(LOG_MOD_INFERENCE, "Run inference %d times\n", num_of_inferences);
	for (int k = 0; k< num_of_inferences; k++)
	{
		volatile BaseType_t status;

		LOG_INFO(LOG_MOD_INFERENCE, "\nBegin inference\n");

		// Lock the flash so that no other thread accesses it.
		flash_lock(0);

		// Set input data for each input of the model
		for(unsigned int i=0; i<job.get_num_of_inputs();i++)
		{
			job.set_input_data(i,model_flash::get_user_input_buffer(i));
		}

#if CONFIG_INFERENCE_ETHOSU_PROFILING
		Profiler profiler{"Inference"};
		// Start the profiler with a tag ("Inference") to measure the inference time
		profiler.StartProfiling();
#endif

		// Invoke asynchronously with callback and argument as params
		// Once the inference is done the callback will get called from the infer task context
		// User could do minimal things to notify the app task for eg using RTOS IPC mechanism
		// The argument will be passed as a param while the callback is called,
		// which the callback can use as needed, for eg to differentiate between models which use same callback
		status = job.invoke_async(model_callback_with_sem,0);
		if (!status)
		{
			LOG_INFO(LOG_MOD_INFERENCE, "Inference failed\n");
		}

#if CONFIG_INFERENCE_ETHOSU_PROFILING
		// Stop the profiler and record the time taken for the "Inference" section
		profiler.StopProfiling();

		// Print the profiling results to the console or other output stream
		// This will display the time taken for the "Inference" section
		profiler.PrintProfilingResult();
#endif

		// Wait for the user semaphore which gets triggered from callback
		// The infer task will get blocked when the NPU is running a particular operator
		// This time can be used by the user app for doing other work such as preparing data for next infer
		// or post processing data of previous infer, update display, etc
		xSemaphoreTake(user_model_sem,portMAX_DELAY);

		// Dump outputs of the model and compare each result with the expected output
		LOG_INFO(LOG_MOD_INFERENCE, "\nComparing inference results vs expected results from Python\n");
		LOG_INFO(LOG_MOD_INFERENCE, "\nThis model has %d output tensors\n", job.get_num_of_outputs());
		for(unsigned int i=0; i<job.get_num_of_outputs();i++)
		{
#ifdef INFER_DUMP_OUTPUT_ON_TERMINAL
			int8_t* output = job.get_output_buffer(i);

			LOG_INFO(LOG_MOD_INFERENCE, "Output index %d\n",i);
			for(int j=0; j<job.get_output_size(i); j++)
			{
				LOG_INFO(LOG_MOD_INFERENCE, "%d,",output[j]);
			}
			LOG_INFO(LOG_MOD_INFERENCE, "\n");
#endif
			// Print the error statistics by comparing the infer output with expected output
			// This would compare the actual output with the expected output being passed as second param
			// The api will print metrics such as MAE, MSE, RMSE, MAXDIFF, PSNR etc
			// This is useful at the time of bring up and in actual application this step may not be needed
			job.print_error_metrics(i, model_flash::get_expected_output_buffer(i));
		}

		// Must release the flash to make it available for other threads
		flash_unlock(0);

		// Wait for some time (just a demo request)
		vTaskDelay(1000);
	} // for


	LOG_INFO(LOG_MOD_INFERENCE, "Inference basic sample application finished\n");
	vTaskDelete( NULL );
}
