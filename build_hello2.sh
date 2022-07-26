emcc \
-g0 \
-O3 \
-lembind \
-s ENVIRONMENT=web \
-s FETCH \
src/hello2.cpp \
-o build/hello2.js
