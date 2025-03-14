# CHIP-8 Emulator

A C++ implementation of a CHIP-8 emulator with raylib for graphics and input handling, allowing you to play classic CHIP-8 games on desktop platforms and web browsers.

## About CHIP-8

The CHIP-8 system has:
- 4KB of memory
- 16 8-bit registers (V0-VF)
- A 16-key hexadecimal keypad
- A monochrome 64×32 pixel display
- Two timers (delay and sound)
- A 16-level stack for subroutine calls

## Features

- Full implementation of all CHIP-8 opcodes
- Built-in text input ROM selection menu
- Cross-platform support (Windows, macOS, Linux)
- Web browser support via WebAssembly (Emscripten)
- FPS display toggle with Left Shift key
- Audio support via raylib
- Accurate timing for delay and sound timers (60Hz)
- Clean modular architecture with separation of concerns

## Architecture

The emulator is built with a modular design:
- `CHIP_8`: Core emulation engine with opcode implementations
- `KeyPad`: Input handling and key state management
- `Stack`: Memory stack operations
- `Window`: Display management using raylib
- `Menu`: Text-based ROM selection interface
- `SoundEffect`: Audio playback using raylib
- `Manager`: Main controller class that coordinates all components

## Installation

There is a compiled linux x86_64 binary at zig-out/bin/ folder.

### Web Version

A web version of the emulator is available [here](https://guterfps.github.io/CHIP-8-Emulator/).

### Prerequisites

- [Zig](https://ziglang.org/download/) (for building)
- [raylib](https://www.raylib.com/)
- [Emscripten](https://emscripten.org/docs/getting_started/downloads.html) (for WebAssembly compilation)

### Building from Source

```bash
# Clone the repository
git clone https://github.com/Guterfps/CHIP-8-Emulator.git
cd CHIP-8-Emulator

# Build the native version with Zig
zig build

# Build the WebAssembly version with Emscripten
./build_for_web.sh
```
You will also need to compile raylib to wasm(see raylib [docs](https://github.com/raysan5/raylib/wiki/Working-for-Web-(HTML5))).

## Usage

### Desktop Version

```
./CHIP-8-Emulator
```

The emulator launches with a text input menu where you can enter the path to a ROM file.

### Web Version

1. Navigate to the web version
2. Use the text input menu to enter the path to a ROM file(in the games/ folder)
3. Press Enter to load the ROM and start the emulator

### Controls

The CHIP-8 uses a 16-key hexadecimal keypad. This emulator maps those keys to the following keyboard layout:

```
CHIP-8 Keypad    Keyboard
+-+-+-+-+        +-+-+-+-+
|1|2|3|C|        |1|2|3|4|
+-+-+-+-+        +-+-+-+-+
|4|5|6|D|        |Q|W|E|R|
+-+-+-+-+   =>   +-+-+-+-+
|7|8|9|E|        |A|S|D|F|
+-+-+-+-+        +-+-+-+-+
|A|0|B|F|        |Z|X|C|V|
+-+-+-+-+        +-+-+-+-+
```

Additional controls:
- `ESC`: Quit the emulator
- `Tab`: Return to the ROM selection menu
- `Left Shift`: Toggle FPS display

## ROM Selection Menu

The emulator includes a simple text-based menu system for entering ROM file paths:
- Type the path to the ROM file you want to load
- Use backspace to correct mistakes
- Press Enter to load the ROM and start the emulator

To use the emulator, you'll need to place ROM files in a location accessible to the program and enter the correct path in the menu.

## Games & ROMs

CHIP-8 ROMs are included in the /games folder :

- Pong
- Blinky
- Space Invaders
- Breakout
- ...

You can download more ROM files and enter their path in the ROM selection menu.

## Technical Implementation

The emulator is written in C++ and uses raylib for graphics, input handling, and audio. The build system is Zig build, and the emulator can be compiled to WebAssembly using Emscripten for browser support.
Key technical components:
- A class with full opcode implementation using a function map for dispatch
- Input handling through raylib mapped to the CHIP-8 keypad
- Sound generation using raylib's audio capabilities
- WebAssembly compatibility using Emscripten

## Bugs

Currently there is a bug in the web version, when a game waits for an input and dosent draw the page freezes.

## Future Improvements

- Directory browsing for ROM selection
- Runtime configuration options
- Super CHIP-8 (SCHIP) support
- Improved debugging features

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request to the [GitHub repository](https://github.com/Guterfps/CHIP-8-Emulator).


## Acknowledgements

- [raylib](https://www.raylib.com/) for providing an excellent graphics and input handling library
- [Zig](https://ziglang.org/) for the build system
- [Emscripten](https://emscripten.org/) for WebAssembly compilation
- Special thanks to Joseph Weisbecker, the original creator of CHIP-8
