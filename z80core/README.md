# Z80 core
This is a C++ port of [Redcode's Z80 core](https://github.com/redcode/Z80).
I decided to ditch the original C version mainly because it's doesn't have an
initializer. This means the CPU struct has to be zeroed out on Windows. I also
have other reasons but eh, too lazy to write em.
