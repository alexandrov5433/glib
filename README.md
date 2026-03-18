# Glib
A general purpose C library. 

## Linux Installation
- Add the compiled static library (libgalxlib.a) -> /usr/lib
- Add the headers (*.h) -> /usr/include/galxlib
- 'touch /etc/ld.so.conf.d/galxlib.conf'   with content:  /usr/lib  (this is the path to the archive)
- run 'sudo ldconfig' (not sure if it is a must, because it updates cache for shared libraries)

Including in a project -> #include "galxlib/***.h"    (#include "galxlib/hash_map.h")

When using galxlib, the project must be compiled with linker flags -lgalxlib -lm: 'gcc test.c -o test -lgalxlib -lm'
