# ADC Sample Application

Demonstrates the SAR ADC driver on the SL2610 SoC (CM52 core).  
Five test scenarios exercise all major driver features in sequence.

## Supported Boards

| Board       | Defconfig                                   |
|-------------|---------------------------------------------|
| SL2610 PEK  | `sl2610_pek_cm52_adc_sample_app_defconfig`  |
| SL2610 RDK  | `sl2610_rdk_cm52_adc_sample_app_defconfig`  |

## Building

```bash
cd examples/driver_examples/adc_sample_app
SRSDK_DIR=<sdk-root> make sl2610_pek_cm52_adc_sample_app_defconfig BUILD=SRSDK
```

The firmware image is written to:

```
out/sl2610_cm52_fw/release/sl2610_cm52_fw.elf
```

## Hardware Setup

- **UART console** – connect a USB-serial adapter to the UART0 pins (see `hw_init.c` for the
  exact GPIO assignment per board revision).  Serial parameters: **115200 8N1**.
- **ADC inputs** – the driver uses `ADC_INSTANCE_0` (base address `0x48022000`).
  - CH0 is used for polling, continuous and window tests.
  - CH1 is used for the interrupt-driven test.
  - No external signal is required; the ADC analog front-end is enabled via
    `ANA_REG = 0x000201B4` which powers up the internal bandgap reference.
    Without an external signal the channel reads the internal reference bias
    (~half-scale, ≈0x7F5).

## Test Scenarios

### Test 1 – Single-Shot Polling Mode (CH0)

Configures the ADC in 21-cycle single-shot mode and triggers one conversion.
The driver polls `SAR_ADC_STATUS.CONV_DONE` in a spin loop, then reads
`SAR_ADC_DATA`.

**Expected output**
```
--- Test 1: Single-Shot Polling Mode (CH0) ---
Polling result: 0x7CD (1997)
Test 1 PASSED
```

> With `ANA_REG = 0x000201B4` the internal bandgap reference is powered;
> CH0 reads ~half-scale (≈0x7F5). The exact code varies slightly between runs.

---

### Test 2 – Interrupt-Driven Single-Shot Mode (CH1)

Registers an event callback, enables the `CONV_DONE` NVIC interrupt, triggers
one conversion on CH1, then blocks on a FreeRTOS binary semaphore.  The ISR
clears `SAR_ADC_STATUS` (W1C) and posts the semaphore.

Interrupt timeout: **100 ms**.

**Expected output**
```
--- Test 2: Interrupt-Driven Single-Shot Mode (CH1) ---
ADC[0]: CONV_DONE event
Interrupt result: 0x7EA (2026)
Test 2 PASSED
```

---

### Test 3 – Continuous Mode, 8 Samples (CH0)

Configures the ADC in 17-cycle continuous mode (`ADC_CONV_TYPE_CONTINUOUS`)
with `CONT_CONV_INTV = 64` cycles between conversions.  The ISR fires on each
`CONV_DONE`, clears the STATUS register (W1C), masks `INT_EN`, and posts the
semaphore.  The task reads each sample then calls `adc_enable_interrupt()` to
re-arm the interrupt before waiting for the next conversion.  The task
collects 8 samples.

**Expected output**
```
--- Test 3: Continuous Mode – 8 samples (CH0) ---
ADC[0]: CONV_DONE event
  Sample[0]: 0x7EE (2030)
ADC[0]: CONV_DONE event
  Sample[1]: 0x7EE (2030)
ADC[0]: CONV_DONE event
  Sample[2]: 0x7ED (2029)
ADC[0]: CONV_DONE event
  Sample[3]: 0x7EF (2031)
ADC[0]: CONV_DONE event
  Sample[4]: 0x7F0 (2032)
ADC[0]: CONV_DONE event
  Sample[5]: 0x7EE (2030)
ADC[0]: CONV_DONE event
  Sample[6]: 0x7EF (2031)
ADC[0]: CONV_DONE event
  Sample[7]: 0x7F1 (2033)
Test 3 PASSED
```

---

### Test 4 – Window Comparison Mode (CH0)

Programs `LLIMIT = 0x400` (≈25 % full-scale) and `ULIMIT = 0xC00`
(≈75 % full-scale).  All three interrupt sources (`CONV_DONE`,
`ULIMIT_FAIL`, `LLIMIT_FAIL`) are enabled.  One conversion is triggered;
the test reports whether the result was inside or outside the window.

The test always reports **PASSED** regardless of window outcome—the window
result is informational.

**Expected output (result inside window)**
```
--- Test 4: Window Comparison Mode (LLIMIT=0x400, ULIMIT=0xC00, CH0) ---
No interrupt fired – result may be within window
Window result: 0x000 (0)
Result is within window
Test 4 PASSED
```

**Expected output (result outside window)**
```
ADC[0]: CONV_DONE event
ADC[0]: LLIMIT_FAIL event
Window result: 0x200 (512)
Result is below LLIMIT (0x400)
Test 4 PASSED
```

---

### Test 5 – BIST (Built-In Self-Test)

Configures the ADC BIST sweep with `ref_min = 0x001` and `ref_max = 0xFFE`,
powers on the ADC analog core (`adc_enable`), then starts the BIST sequence.
The driver polls `SAR_ADC_BIST_STS.BIST_DONE` with a guard of 100 000
iterations.

**Expected output**
```
--- Test 5: BIST ---
BIST PASSED
Test 5 PASSED
```

---

## Full Expected Log

```
0000000000:[0][INF][SYS ]:Application drivers initialization complete without errors.
0000000000:[0][INF][SYS ]:sl2610 SDK version 1.5.0
0000000000:[0][INF][SYS ]:M52:: Build Date 10-08-2026 Time 16:47:47 Commit 645289f63
0000000000:[0][INF][GENR]:===================================
0000000005:[0][INF][GENR]:    ADC Sample Application Start
0000000011:[0][INF][GENR]:===================================
0000000016:[0][INF][GENR]:
--- Test 0: Raw Register Direct (CH0 single-shot) ---
0000000023:[0][INF][GENR]:  CTRL after START_CONV: 0x1540A382
0000000029:[0][INF][GENR]:  STATUS=0x00000001  DATA=0x0000080F  result=0x80F (2063) = 0.605 V
0000000037:[0][INF][GENR]:Test 0 PASSED
0000000041:[0][INF][GENR]:
--- Test 1: Single-Shot Polling Mode (CH0) ---
0000000047:[0][INF][GENR]:
===== ADC Register Dump (base=0x48022000) =====
0000000054:[0][INF][GENR]:RAW  0x48022000:  0x1540A382  0x000001B4  0x00000FFF  0x00000000
RAW  0x48022010:  0x00000001  0x00000000  0x0000080F
0000000067:[0][INF][GENR]:0x000 SAR_ADC_CTRL      = 0x1540A382
       PD[0]            = 0  (0=active, 1=powerdown)
       RESET_N[1]       = 1  (1=released)
       SEL[4:2]         = 0  (channel)
       CONV_MODE[6:5]   = 0  (0=21cyc, 1=20cyc, 2/3=17cyc)
       SEL_EN[7]        = 1
       EN_CTRL[8]       = 1
       START_CONV[9]    = 1
       ADC_CONV_TYPE[11:10] = 0  (0=single, 1=cont, 2=ext-trig)
       BGCLK_DIV[15:12] = 10  (diag=10)
       CONT_CONV_INTV[23:16] = 64  (diag=64)
       CON_DATA_LT[29:24]    = 21
0000000112:[0][INF][GENR]:0x004 SAR_ADC_CTRL1     = 0x000001B4  (ana_reg, diag=0x000201B4)
0000000121:[0][INF][GENR]:0x008 SAR_ADC_DATA_ULIMIT = 0x00000FFF  (4095)
0x00C SAR_ADC_DATA_LLIMIT = 0x00000000  (0)
0000000131:[0][INF][GENR]:0x010 SAR_ADC_STATUS    = 0x00000001
       CONV_DONE[0]     = 1
       ULIMIT_FAIL[1]   = 0
       LLIMIT_FAIL[2]   = 0
0000000144:[0][INF][GENR]:0x014 SAR_ADC_INT_EN    = 0x00000000
0x018 SAR_ADC_DATA      = 0x0000080F  (raw 12-bit = 0x80F, 2063)
0000000155:[0][INF][GENR]:===========================================
0000000161:[0][INF][GENR]:Polling result: 0x80F (2063) = 0.605 V
0000000167:[0][INF][GENR]:Test 1 PASSED
0000000171:[0][INF][GENR]:
--- Test 2: Interrupt-Driven Single-Shot Mode (CH1) ---
0000000178:[0][DBG][GENR]:ADC[0]: CONV_DONE event
0000000179:[0][INF][GENR]:Interrupt result: 0x004 (4) = 0.001 V
0000000184:[0][INF][GENR]:Test 2 PASSED
0000000188:[0][INF][GENR]:
--- Test 3: Continuous Mode , 8 samples (CH0) ---
0000000195:[0][DBG][GENR]:ADC[0]: CONV_DONE event
0000000196:[0][DBG][GENR]:ADC[0]: CONV_DONE event
0000000197:[0][INF][GENR]:  Sample[0]: 0x000 (0) = 0.000 V
0000000202:[0][DBG][GENR]:ADC[0]: CONV_DONE event
0000000203:[0][INF][GENR]:  Sample[1]: 0x80F (2063) = 0.605 V
0000000209:[0][DBG][GENR]:ADC[0]: CONV_DONE event
0000000210:[0][INF][GENR]:  Sample[2]: 0x80F (2063) = 0.605 V
0000000216:[0][DBG][GENR]:ADC[0]: CONV_DONE event
0000000217:[0][INF][GENR]:  Sample[3]: 0x80F (2063) = 0.605 V
0000000222:[0][DBG][GENR]:ADC[0]: CONV_DONE event
0000000223:[0][INF][GENR]:  Sample[4]: 0x80F (2063) = 0.605 V
0000000228:[0][DBG][GENR]:ADC[0]: CONV_DONE event
0000000229:[0][INF][GENR]:  Sample[5]: 0x80F (2063) = 0.605 V
0000000234:[0][DBG][GENR]:ADC[0]: CONV_DONE event
0000000235:[0][INF][GENR]:  Sample[6]: 0x80F (2063) = 0.605 V
0000000240:[0][DBG][GENR]:ADC[0]: CONV_DONE event
0000000241:[0][INF][GENR]:  Sample[7]: 0x80F (2063) = 0.605 V
0000000246:[0][INF][GENR]:Test 3 PASSED
0000000250:[0][INF][GENR]:
--- Test 4: Window Comparison Mode (LLIMIT=0x400, ULIMIT=0xC00, CH0) ---
0000000359:[0][WRN][GENR]:No interrupt fired, result may be within window
0000000359:[0][INF][GENR]:Window result: 0x000 (0) = 0.000 V
0000000359:[0][INF][GENR]:Result is within window
0000000359:[0][INF][GENR]:Test 4 PASSED
0000000214:[0][INF][GENR]:
                          --- Test 5: BIST ---
0000000214:[0][INF][GENR]:BIST PASSED
0000000214:[0][INF][GENR]:Test 5 PASSED
0000000214:[0][INF][GENR]:===================================
0000000214:[0][INF][GENR]:ADC Sample App: ALL TESTS PASSED
0000000214:[0][INF][GENR]:===================================
```

## Key Driver Notes

| Topic | Detail |
|---|---|
| ADC base address | `0x48022000` (non-secure alias) |
| IRQ | `PERIF_ADC_IRQn = 98` |
| Core clock | `CLK_CORE_ADC` |
| Config clock gate | `CLK_CFG_ADC` (enabled in `adc_soc_init`) |
| `ANA_REG` | Must be `0x000201B4` to power up bandgap + bias; zero yields ~0 counts |
| STATUS register | W1C — ISR clears fired bits; `INT_EN` is left enabled for continuous mode |
| Stale flags | `adc_init()` clears `SAR_ADC_STATUS` to discard flags from the previous test |
| BIST | `adc_enable()` must be called before `adc_bist_start()` (analog core must be on) |

## File Structure

```
adc_sample_app/
├── adc_sample_app.c        # Entry point – calls adc_sample_task()
├── adc_sample_app.h
├── main.c                  # FreeRTOS setup and task creation
├── CMakeLists.txt
├── Makefile
├── kconfig
├── src.cmake
├── configs/
│   ├── sl2610_pek_cm52_adc_sample_app_defconfig
│   └── sl2610_rdk_cm52_adc_sample_app_defconfig
└── hw/
    ├── SL2610_PEK/
    │   ├── hw_init.c       # UART0 + XSPI pinmux
    │   ├── hw_init.h
    │   ├── adc_board.c     # Five test scenarios (PEK)
    │   └── adc_board.h
    └── SL2610_RDK/
        ├── hw_init.c       # UART0 (Rev.A / Rev.B SW_STRAP0) + XSPI + I2C pinmux
        ├── hw_init.h
        ├── adc_board.c     # Five test scenarios (RDK)
        └── adc_board.h
```
