```
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
```


```
./build_hello.sh
./build_hello2.sh

http-server . -c-1 -p 8080
```


## reference

- https://emscripten.org/docs/tools_reference/emcc.html