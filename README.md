# Fantacom
A WIP toy/fantasy computer, powered by the Z80.

## The specs
- 8KiB ROM. A boot ROM for testing can be found in the `bootrom` directory.
- Paged MMU with 4KiB pages. Can address up to 1MiB of RAM.
- Default CPU speed is 1MHz. Can be adjusted from command line.
- VGA Text Mode 80x25 emulation, buffer page can be adjusted via I/O ports.
- Disk images can be used for file I/O.

## Font license
This emulator embeds Viznut's [UNSCII](http://viznut.fi/unscii/) font, which is licensed under the GPL license.
