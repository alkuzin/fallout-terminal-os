# Fallout Terminal OS

## Description

This project was made on top of [simple-os](https://github.com/alkuzin/simple-os/) and [fallout terminal
library](https://github.com/alkuzin/FalloutTerminal).

## Tools

The project was made on Debian 12
 - GCC (13.2.0)
 - NASM (2.16.01)
 - GNU Make (4.3)
 - GNU ld (2.42)
 - GNU ar (2.42)
 - truncate (9.4)
 - QEMU emulator (8.2.1)
 - GNU GRUB (2.12-1)

## 3. Installation
First clone this repository:
```console
https://github.com/alkuzin/fallout-terminal-os.git
```
Then move to the `fallout-terminal-os/` directory:
```console
cd fallout-terminal-os/
```

To build kernel run:

```console
make all
```

To run OS on QEMU use this:
 ```console
make init
```

To return everything to original state:
```console
make fclean
```

## Project

There are 3 builtin types of terminal:
 - institute terminal
 - Mass Fusion terminal
 - Brotherhood Of Steel terminal

## Institute terminal

<img src="res/fallout-terminal-os.gif">

## Mass Fusion terminal

<img src="res/mass-fusion-terminal.png">

## Brotherhood Of Steel terminal

<img src="res/bos-terminal.png">

## License

MIT License

Copyright (c) 2024 Alexander (@alkuzin)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
