#!/bin/bash -x
CFLAGS=`python include_paths.py`
gcc -shared -o hello.so -fPIC hello.c $CFLAGS
python -c "import hello; hello.world()"
