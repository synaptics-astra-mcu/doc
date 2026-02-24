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


#include "inference_multi_model_sample_app.hpp"

#include "inference_attributes.hpp"
#include "infer_job.hpp"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "logger.h"
#include "syna_device.h"
#if CONFIG_INFRA_PROFILER_ENABLED
#include "Profiler.hpp"
#endif

// choose SHARED arena if you wish to reuse the arena for both models
// otherwise choose INDEPENDENT to have a saparate arena for each of the models
#define SHARED_TENSOR_ARENA		0
#define INDEPENDENT_ARENA		1
#define ARENA_TYPE				SHARED_TENSOR_ARENA

namespace multi_model1 {
	extern const uint8_t * get_model_pointer();
	extern size_t get_model_len();
	extern tflite::MicroOpResolver& get_resolver(void);
	extern int8_t* get_user_input_buffer(int index) ;
	extern int8_t* get_expected_output_buffer(int index);
} /* namespace multi_model1 */

namespace multi_model2 {
	extern const uint8_t * get_model_pointer();
	extern size_t get_model_len();
	extern tflite::MicroOpResolver& get_resolver(void);
	extern int8_t* get_user_input_buffer(int index) ;
	extern int8_t* get_expected_output_buffer(int index);
} /* namespace multi_model2 */

static uint8_t tensor_arena_1[200*1024] ACTIVATION_BUF_ATTRIBUTE;
#if ARENA_TYPE == INDEPENDENT_ARENA
	static uint8_t tensor_arena_2[200*1024] ACTIVATION_BUF_ATTRIBUTE;
#endif


static uint8_t option = 0;
static infer_job model1;
static infer_job model2;

static SemaphoreHandle_t user_model1_sem;
static SemaphoreHandle_t user_model2_sem;

static void model2_callback_with_sem(infer_job* obj, uint32_t arg, TfLiteStatus status)
{
	LOG_INFO(LOG_MOD_INFERENCE, "model2 callback with sem : %d : %d\n",arg, status);
	// Print the error statistics by comparing the infer output with expected output
	obj->print_error_metrics(0, multi_model2::get_expected_output_buffer(0));

	// Release tensor arena
	obj->release_tensor_arena();

	// Signal the application task indicating that the inference is finished
	xSemaphoreGive(user_model2_sem);
}

static void model1_callback_without_sem(infer_job* obj, uint32_t arg, TfLiteStatus status)
{
	infer_job* pmodel2 = (infer_job*)arg;
	LOG_INFO(LOG_MOD_INFERENCE, "model1 callback without sem : %d : %d\n",arg, status);
	// Print the error statistics by comparing the infer output with expected output
	obj->print_error_metrics(0, multi_model1::get_expected_output_buffer(0));

	// Release tensor arena
	obj->release_tensor_arena();

	// Lock model2 tensor arena
	pmodel2->lock_tensor_arena(portMAX_DELAY);

	// Set input data
	pmodel2->set_input_data(0,multi_model2::get_user_input_buffer(0));

	// Invoke model2 asynchronously
	pmodel2->invoke_async(model2_callback_with_sem, 1);
}

static void model1_callback_with_sem(infer_job* obj, uint32_t arg, TfLiteStatus status)
{
	LOG_INFO(LOG_MOD_INFERENCE, "model1 callback with sem : %d : %d\n",arg, status);
	// Print the error statistics by comparing the infer output with expected output
	obj->print_error_metrics(0, multi_model1::get_expected_output_buffer(0));

	// Release tensor arena
	obj->release_tensor_arena();

	// Signal the application task indicating that the inference is finished
	xSemaphoreGive(user_model1_sem);
}

void multi_model_sample_app(void *argument)
{
	// Setup need to called before using any infer api's.
	// It will create all the necessary queues and tasks needed for the api to run
	infer_job::setup();

	const uint8_t* model_ptr_model1 = multi_model1::get_model_pointer();
	const uint8_t* model_ptr_model2 = multi_model2::get_model_pointer();

	// Initialize the model with the tflite location, tensor arena area and resolver
	model1.init(model_ptr_model1, tensor_arena_1, sizeof(tensor_arena_1), multi_model1::get_resolver());

#if ARENA_TYPE == SHARED_TENSOR_ARENA
	// Initialize the model with the tflite location, shared tensor allocator info from model1 model and resolver
	// In this case the tensor_arena_1 will be shared between model2 and model1 model
	model2.init(model_ptr_model2, model1.get_allocator_info(), multi_model2::get_resolver());
#else
	// Initialize the model with the tflite location, tensor arena area and resolver
	// In this case model2 has its own tensor arena area which is tensor_arena_2
	model2.init(model_ptr_model2, tensor_arena_2, sizeof(tensor_arena_2), multi_model2::get_resolver());
#endif

	// Create semaphores used for signalling the application task from inference call back
	user_model1_sem = xSemaphoreCreateBinary();
	if(user_model1_sem == nullptr)
		LOG_ERROR(LOG_MOD_INFERENCE, "Semaphore creation failed\n");
	user_model2_sem = xSemaphoreCreateBinary();
	if(user_model2_sem == nullptr)
		LOG_ERROR(LOG_MOD_INFERENCE, "Semaphore creation failed\n");

	while(1)
	{
		LOG_INFO(LOG_MOD_INFERENCE, "\n********** Option %d **********\n",option);
		volatile BaseType_t status;
		switch(option)
		{
			// model1 sync
			case 0:
			{
				// Lock tensor arena
				model1.lock_tensor_arena(portMAX_DELAY);

				// Pass input data to be set
				model1.set_input_data(0,multi_model1::get_user_input_buffer(0));

#if CONFIG_INFERENCE_ETHOSU_PROFILING
				Profiler profiler{"Inference"};

				// Start the profiler with a tag ("Inference") to measure the inference time
				profiler.StartProfiling();
#endif

				// Invoke synchronously and returns when infer is done
				status = model1.invoke();

#if CONFIG_INFERENCE_ETHOSU_PROFILING
				// Stop the profiler and record the time taken for the "Inference" section
				profiler.StopProfiling();

				// Print the profiling results to the console or other output stream
				// This will display the time taken for the "Inference" section
				profiler.PrintProfilingResult();
#endif

				model1.print_error_metrics(0, multi_model1::get_expected_output_buffer(0));

				// Release tensor arena
				model1.release_tensor_arena();

				LOG_INFO(LOG_MOD_INFERENCE, "model1 invoke sync done\n");
				break;
			}
			// model1 async followed by model2 async queued back to back
			case 1:
			{
				// Lock tensor arena
				model1.lock_tensor_arena(portMAX_DELAY);

				// Pass input data to be set
				model1.set_input_data(0,multi_model1::get_user_input_buffer(0));

				// Invoke asynchronously
				status = model1.invoke_async(model1_callback_with_sem,0);
				LOG_INFO(LOG_MOD_INFERENCE, "model1 status = %ld\n",status);

				// Wait for the user semaphore
				xSemaphoreTake(user_model1_sem,portMAX_DELAY);

				LOG_INFO(LOG_MOD_INFERENCE, "model2 lock tensor arena\n");
				// Lock tensor arena
				model2.lock_tensor_arena(portMAX_DELAY);

				LOG_INFO(LOG_MOD_INFERENCE, "model2 set input data\n");
				// Pass input data to be set
				model2.set_input_data(0,multi_model2::get_user_input_buffer(0));

				LOG_INFO(LOG_MOD_INFERENCE, "model2 invoke async\n");
				// Invoke asynchronously without callback
				status = model2.invoke_async(model2_callback_with_sem,0);
				LOG_INFO(LOG_MOD_INFERENCE, "model2 status = %ld\n",status);

				// Wait for the user semaphore
				xSemaphoreTake(user_model2_sem,portMAX_DELAY);

				LOG_INFO(LOG_MOD_INFERENCE, "model2 wait for infer done\n");
				break;
			}
			/* model1 async, model2 queued from model1 callback, but probability of a deadlock might happen if INFER_MAX_QUEUE_SIZE kept low.
			 * INFER_MAX_QUEUE_SIZE should be kept with a value higher than the max number of jobs that could be queued at a time
			 * This option should be used with caution otherwise this might cause a deadlock.
			 */
			case 2:
			{
				// Lock tensor arena
				model1.lock_tensor_arena(portMAX_DELAY);

				// Pass input data to be set
				model1.set_input_data(0,multi_model1::get_user_input_buffer(0));

				// Invoke asynchronously and from the callback model2 will be queued
				model1.invoke_async(model1_callback_without_sem, (uint32_t)&model2);

				// Wait for user semaphore which is given when model2 is finished
				xSemaphoreTake(user_model2_sem,portMAX_DELAY);

				LOG_INFO(LOG_MOD_INFERENCE, "callback sem recieved after finishing model2\n");
				break;
			}
			/* model1 and model2 try to lock the tensor arena together, it will pass in case of different arena
			 * but will fail while they are shared
			 */
			case 3:
			{
				// Lock tensor arena
				model2.lock_tensor_arena(portMAX_DELAY);
				LOG_INFO(LOG_MOD_INFERENCE, "model2 locked\n");

				// Pass input data to be set
				model2.set_input_data(0,multi_model2::get_user_input_buffer(0));

				// Invoke asynchronously without callback
				model2.invoke_async(model2_callback_with_sem,0);

				// Lock tensor arena
				LOG_INFO(LOG_MOD_INFERENCE, "trying to lock model1\n");
				model1.lock_tensor_arena(portMAX_DELAY);
				LOG_INFO(LOG_MOD_INFERENCE, "model1 locked\n");

				// Pass input data to be set
				model1.set_input_data(0,multi_model1::get_user_input_buffer(0));

				// Invoke synchronously and returns when infer is done
				model1.invoke_async(model1_callback_with_sem,0);

				// Wait for the user semaphore
				xSemaphoreTake(user_model2_sem,portMAX_DELAY);

				// Release tensor arena
				model2.release_tensor_arena();
				LOG_INFO(LOG_MOD_INFERENCE, "model2 unlocked\n");

				// Wait for the user semaphore
				xSemaphoreTake(user_model1_sem,portMAX_DELAY);

				// Release tensor arena
				model1.release_tensor_arena();
				LOG_INFO(LOG_MOD_INFERENCE, "model1 unlocked\n");
				break;
			}
		}
		option++;
		if(option > 3)
		{
			option = 0;
		}
		vTaskDelay(1000);
	}
}
