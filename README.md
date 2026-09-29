# F407 Discovery — Blinky + UART status

Bare-metal (no HAL/CMSIS, no CubeMX needed) blink demo for the STM32F407G-DISC1
board. Toggles all four on-board LEDs (PD12-15) every 500 ms and prints the
LED state over USART2 (PA2 = TX, PA3 = RX) at 115200 8N1.

## 1. One-time prerequisites

- **ARM GCC toolchain** (`arm-none-eabi-gcc`) — not currently on PATH on this
  machine. Install **STM32CubeCLT** from ST (see the links saved from the
  earlier VS Code setup) and make sure its `bin` directory is on PATH,
  or install the plain [Arm GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
  and add that to PATH instead.
- **GNU Make** — already present (`D:\EMBEDDED\CTOOLS\mingw32\bin\make.exe`).
- **STM32_Programmer_CLI** (for `make flash`) — comes with STM32CubeCLT /
  STM32CubeProgrammer. If you'd rather not install it, flash the built
  `build/blinky.bin` with the STM32CubeProgrammer GUI instead (drag the file
  in, target address `0x08000000`, connect via ST-LINK, Download).
- A **USB-to-TTL serial adapter** (FTDI, CP2102, CH340, ...) — see step 3,
  this board's on-board ST-Link does **not** expose a virtual COM port like
  a Nucleo does.

## 2. Build and flash

From this folder (VS Code terminal or `Ctrl+Shift+B` for the default build task):

```
make            # -> build/blinky.elf, build/blinky.bin
make flash      # requires STM32_Programmer_CLI on PATH and the board connected via USB (mini-USB ST-LINK port)
```

VS Code tasks are already wired up: `Ctrl+Shift+B` builds, or run task
"flash" from the Command Palette (`Tasks: Run Task`).

If `make` fails with "arm-none-eabi-gcc: command not found", the toolchain
from step 1 isn't on PATH yet — that's the only missing piece.

## 3. Wiring for the serial monitor

The STM32F4-Discovery's on-board ST-Link is a plain debugger — unlike a
Nucleo board, it is **not** wired to a UART, so USART2 won't show up as a
COM port just from the mini-USB debug cable. You need a cheap USB-to-TTL
(3.3V) adapter:

| Adapter pin | Board pin      |
|-------------|-----------------|
| GND         | GND             |
| RX          | PA2 (USART2_TX) |

(TX on the adapter → PA3 isn't needed here since the board never reads UART input, but wire it too if you want to extend the demo later.)

Plug the adapter into your PC — Windows will enumerate it as a COM port
(check Device Manager → Ports (COM & LPT) for something like "USB-SERIAL CH340 (COMx)" or similar, note the COM number).

## 4. Watch it in VS Code's Serial Monitor

The Serial Monitor extension (`eclipse-cdt.serial-monitor`) is already
installed as part of the STM32Cube extension pack.

1. Open the **Serial Monitor** view: Command Palette (`Ctrl+Shift+P`) →
   "Serial Monitor: Open" (or find its icon in the bottom status bar / panel).
2. Pick the COM port your USB-to-TTL adapter enumerated as.
3. Set baud rate to **115200**, 8 data bits, no parity, 1 stop bit (default).
4. Click **Start Monitoring** / connect.
5. Reset the board (or reflash it) — you should see:
   ```
   --- STM32F407 Discovery blinky ---
   LED state: ON
   LED state: OFF
   LED state: ON
   ...
   ```
   printed every 500 ms in sync with the four LEDs turning on/off together.

## 5. Optional: MATLAB MCP server

`.mcp.json` in this folder registers MathWorks' [MATLAB MCP Server](https://github.com/matlab/matlab-mcp-server),
which lets Claude Code drive a MATLAB session on this machine — write MATLAB
code, run it, and read the results back — instead of you copy-pasting between
the two. It's entirely optional; the build above doesn't depend on it.

Why it's handy for this project: the firmware streams LED state over USART2, and
once that stream carries real sensor data (ADC samples, IMU readings) MATLAB is
the natural place to plot and analyse a capture, design a filter, and then hand
the resulting coefficients back to be pasted into `src/main.c`.

Setup (one time):

1. Install MATLAB R2021a or later and make sure it's on PATH.
2. Download the server binary for your platform from the
   [releases page](https://github.com/matlab/matlab-mcp-server/releases)
   (`matlab-mcp-server-windows-x64.exe` here).
3. Either put that binary on PATH as `matlab-mcp-server`, or point the
   `MATLAB_MCP_SERVER` environment variable at its full path.
4. Start Claude Code in this folder and approve the `matlab` server when it
   asks — project-scoped servers need a one-time approval per user.

Useful extra `args` in `.mcp.json` if you want them: `--matlab-root` (pick a
specific MATLAB install, path without `/bin`), `--matlab-display-mode=nodesktop`
(no MATLAB GUI), `--matlab-session-mode=existing` (attach to a MATLAB you
already have open), `--initialize-matlab-on-startup=true` (avoid the first-call
startup delay).

The server only works where MATLAB is actually installed — it's a local
desktop integration, so it does nothing in a Claude Code web/cloud session.

## Notes

- This project intentionally skips STM32CubeMX/HAL so it's buildable right
  now with just the ARM GCC toolchain — good for a first test. Once
  STM32CubeMX is installed, regenerating this as a CMake project through the
  STM32Cube VS Code extension will unlock its integrated one-click debug
  (breakpoints, live variable watch, etc.); that debug adapter is built
  around projects it generates itself, so it's not wired up for this
  hand-written one.
- Runs on the default 16 MHz HSI clock (no PLL) — plenty for this demo.
