# Timer Driver Sample Application

## Description

The Timer Driver sample application validates timer APIs and both timer modes introduced in the updated naming model:
- `TIMER_MODE_ONE_SHOT`
- `TIMER_MODE_PERIODIC`

The app executes functional testcases and prints pass/fail status on the logger console.

Implemented testcases:
- **TMR-FN-01**: Validate blocking APIs (`timer_wait_us`, `timer_wait_ticks`).
- **TMR-FN-02**: Validate one-shot mode (`timer_start_us(..., TIMER_MODE_ONE_SHOT)`) callback behavior.
- **TMR-FN-03**: Validate periodic mode (`timer_start_us(..., TIMER_MODE_PERIODIC)`) and stop behavior.
- **TMR-FN-04**: Validate system counter progression and remaining-time query APIs (`timer_get_system_count`, `timer_get_remaining_ticks`, `timer_get_remaining_us`).
- **TMR-FN-05**: Validate cancellation of a running one-shot timer via `timer_stop`.
- **TMR-FN-06**: Optional slowclk mode validation (one-shot + periodic). If slowclk timers are unavailable on the target, this testcase is reported as skipped.

## Supported Boards

- `SL2610_RDK`
- `SR110_RDK`

## Defconfigs

- `sl2610_rdk_cm52_timer_sample_app_all_defconfig`
- `sr110_rdk_cm55_timer_sample_app_all_defconfig`

## Build and Flash (CLI)

1. Build from the application directory:

```bash
cd <sdk-root>/examples/driver_examples/timer_sample_app
export SRSDK_DIR=<sdk-root>
make sl2610_rdk_cm52_timer_sample_app_all_defconfig BUILD=SRSDK
```

SR110 build example:

```bash
make sr110_rdk_cm55_timer_sample_app_all_defconfig BUILD=SRSDK
```

2. Rebuild only the app (after SDK package is available):

```bash
make build
```

3. Generate image and flash using the standard SL2610 flow documented here:
- [SL2610 Build and Flash with CLI](../../../docs/SL2610/SL2610_Build_and_Flash_with_CLI.md)

## Expected Log Summary

On successful run, logs show the configured testcase flow for the selected board. For example:

```text
=== [TMR-FN-01] Blocking Wait API Test ===
...
=== [TMR-FN-01] Blocking Wait API Test PASSED ===
```

## Notes

- Test timing can be tuned via Kconfig options in `kconfig`.
- The sample avoids using blocking timer waits while periodic mode is active, to prevent self-contention in constrained timer-instance configurations.
