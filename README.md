# is1500-dtek-project

Tetris with animated block graphics, standard tetris rules, GUI and score tracking

Project in course [IS1500 Computer Organization and Components](https://www.kth.se/student/kurser/kurs/IS1500?l=en) at KTH Royal Institute of Technology.

## Introduction

The purpose of this project is to develop the classic Tetris game (see [Wikipedia](https://en.wikipedia.org/wiki/Tetris)) on an
embedded platform with limited resources.

## Requirements

- The game should incorporate animated graphics, with blocks transitioning pixel-by-pixel from one position to another.
- We will adhere to the standard Tetris rules, which can be found on this page.
- The next block will be displayed on the screen
- Both the current score and the high score will be visible, with the option to enter initials when the game ends. Optionally, we will implement high score storage between power cycles (i.e., that it is preserved even when the ChipKIT is turned off).

## Usage

Before you start, make sure you have [mcb32tools](https://github.com/is1200-example-projects/mcb32tools/releases/) installed.

Connect the ChipKIT USB cable to your computer.

In `src`, do:

```bash
make
```

... followed by:

```bash
make install
```

If the `Makefile` is unable to find the USB device, you can specify the correct USB serial port like this:

```bash
make install TTYDEV=/dev/ttyUSB0
```

- Linux: This is normally `/dev/ttyUSB0`
- Windows: In MSYS2, this is normally `/dev/ttyS2`
- Mac: This is normally `/dev/cu.usbserial-*` (replace `*` with something)

## License

Licensed under the terms of the MIT License. See the [license file](LICENSE).

The following files:

- `src/Makefile`
- `src/stubs.c`
- `src/vector.S`
- `src/display.c`
- `src/display.h`
- `src/data.c`
- some parts of `src/main.c`

... are **NOT** covered by the MIT License. The LICENSE for these files is located in [src/COPYING](src/COPYING).

## Authors

- [Vilhelm Prytz](https://github.com/vilhelmprytz) - vprytz@kth.se / vilhelm@prytznet.se
- [Filip Dimitrijevic](https://github.com/FilipDimitrijevic97) - filipdi@kth.se / filip@kthais.com
