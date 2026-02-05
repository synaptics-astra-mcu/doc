# Compiling a TFLite Model with Vela (SR110)

This guide explains how to compile a quantized TFLite model using the **Vela** compiler and generate the C++ sources used by SR110 inference examples. It uses the SDK inference tool under `tools/Inference`.

Throughout this guide, `<sdk-root>` refers to the folder where you extracted or cloned the SDK.

## Table of Contents

- [Prerequisites](#prerequisites)
- [Set up a Virtual Environment](#set-up-a-virtual-environment)
- [Install Vela](#install-vela)
- [Run the Inference Tool](#run-the-inference-tool)
- [Outputs](#outputs)
- [Prepare a Flashable Model Binary (VS Code)](#prepare-a-flashable-model-binary-vs-code)
- [Common Usage Examples](#common-usage-examples)
- [Notes](#notes)

## Prerequisites

- A quantized **INT8** `.tflite` model.
- Python **3.7–3.10** 
- Visual Studio C++ Build Tools (Windows only, required for some Python packages).

**Recommendation:** Use a separate venv for inference tools. The SDK build uses Python 3.13, which is not compatible with the inference tool dependencies.

## Set up a Virtual Environment

Create and activate a venv. Make sure the `python` you use is **3.7–3.10**:

```bash
python --version
```

**Windows:**
```bash
python -m venv my_venv
my_venv\\Scripts\\activate.bat
```

**Linux/macOS:**
```bash
python -m venv ~/my_venv
source ~/my_venv/bin/activate
```

Keep the venv active for the rest of this guide.

## Install Vela

The inference tool expects the `vela` command on PATH.

Install Vela from the repository and checkout the validated version:

```bash
git clone https://review.mlplatform.org/ml/ethos-u/ethos-u-vela.git
cd ethos-u-vela
git checkout 4.2
pip install .
```

Verify:
```bash
vela --version
```

Install requirements from the SDK inference folder:
```bash
cd <sdk-root>/tools/Inference
pip install -r requirements.txt
```

## Run the Inference Tool

From `<sdk-root>/tools/Inference`:
```bash
python infer_code_gen.py -t <path_to_tflite_model> \
  [-o <output_directory>] \
  [-n <namespace>] \
  [-s <scripts>] \
  [-i <input_files>] \
  [-c <compiler>] \
  [-tl <tflite_location>] \
  [-p <optimization_strategy>]
```

Key options (from the script):
- `-c` / `--compiler`: `vela` (default) or `none`
- `-p` / `--optimize`: `Performance` (default) or `Size`
- `-tl` / `--tflite_loc`: `1` = SRAM, `2` = FLASH
- `-s` / `--script`: `model` and/or `inout` (default runs both)
- `-i` / `--input`: optional `.npy`/`.bin` inputs for expected output generation

**About `-tl`:** This switch affects both Vela’s memory planning and the generated C++ attribute.  
`-tl 1` uses SRAM (`--memory-mode=Sram_Only` and `MODEL_TFLITE_ATTRIBUTE`).  
`-tl 2` targets flash/QSPI (`--memory-mode=Shared_Sram` and `MODEL_TFLITE_ATTRIBUTE_FLASH`).  
You still need the VS Code Image Conversion step to produce a flashable model binary.

**Tuning Vela memory planning:** Vela supports `--arena-cache-size <bytes>` to cap the arena it assumes during compilation.  
`infer_code_gen.py` does **not** expose this flag. To use it, either:

1. Run Vela manually with `--arena-cache-size`, then generate code/IO without re-compiling:
   ```bash
   vela --arena-cache-size <bytes> --output-dir <OUT_DIR> <MODEL_NAME>.tflite
   python infer_code_gen.py -t <OUT_DIR>/<MODEL_NAME>_vela.tflite -c none -o <OUT_DIR>
   ```
2. Or, add `--arena-cache-size` to the `vela_params` list inside `tools/Inference/infer_code_gen.py`:
   ```bash
   vela_params = ['vela', '--output-dir', args.output_dir, '--accelerator-config=ethos-u55-128', \
   '--optimise=' + args.optimize, '--config=Arm\\vela.ini', memory_mode, \
   '--system-config=Ethos_U55_High_End_Embedded', args.tflite_path, '--arena-cache-size=1500000']
   ```

## Outputs

In the output directory you will see:
- `<namespace>.cc` (model C++ source + resolver content)
- `<namespace>_io.cc` (input/expected output data)
- `<model>_vela.tflite` (when `-c vela`)
- `output_*.bin` and `output_*.npy` (expected outputs)
- `<namespace>_micro_mutable_op_resolver.hpp` (intermediate header, appended into `<namespace>.cc`)

## Prepare a Flashable Model Binary (VS Code)

If you plan to place model weights in flash, you must convert the Vela output into a flashable model binary using the **Astra MCU SDK VS Code Extension**:

1. **Rename** the Vela output `<model>_vela.tflite` from `.tflite` to `.bin` (the contents are unchanged).
2. In VS Code, open **Build and Deploy** → **Image Conversion**.
3. Use the **Advanced Configurations** options to generate a **Model Binary** from the renamed `.bin`.

For details on the Image Conversion workflow, see [Astra MCU SDK VS Code Extension User Guide](../Astra_MCU_SDK_VSCode_Extension_User_Guide.md).

## Common Usage Examples

**Size-optimized (SRAM)**:
```bash
python infer_code_gen.py -t <MODEL_NAME>.tflite -o <OUT_DIR> -p Size -tl 1
```

**Performance-optimized (FLASH)**:
```bash
python infer_code_gen.py -t <MODEL_NAME>.tflite -o <OUT_DIR> -p Performance -tl 2
```

## Notes

- Filenames should avoid spaces or special characters.
- The inference tool is maintained under `tools/Inference`. If behavior changes, check the [inference tool README](../../tools/Inference/README.md) and the script help output:
  ```bash
  python infer_code_gen.py -h
  ```

---
## Memory Allocation Notes

When configuring memory for your project, keep the following in mind:

* **Tensor Arena Size:** Set this to at least the `Total SRAM used` value printed by Vela. Add a **minimum of 10KB extra** as a buffer for runtime overhead.
  - The tensor arena size is **not** set by `infer_code_gen.py`. The `-p Size` option only changes Vela’s optimization strategy, not your arena allocation.
  - Set the arena size in your application code (for example, `TENSOR_ARENA_SIZE` in `examples/SR110_RDK/vision_examples/<usecase>/infer.cc` or the arena buffer in `examples/SR110_RDK/inference_examples/<app>/<app>.cc`).
  - Use `get_used_tensor_arena_size()` at runtime to size it properly, then keep a small safety margin.
* **Model Weights:** Weights reside in the space indicated by `Total On-chip Flash used` in the Vela output.