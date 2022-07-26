#!/bin/bash

emcc \
-g0 \
-O3 \
-lembind \
-s ENVIRONMENT=web \
-s FETCH \
-s EXPORTED_FUNCTIONS=_main,_lerp,_incOne,_doFetch \
-s EXPORTED_RUNTIME_METHODS=ccall,cwrap \
src/hello.c \
-o build/hello.js
