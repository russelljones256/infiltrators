#!/bin/bash
./../emsdk/upstream/emscripten/em++ -std=c++17 -O2 -s ALLOW_MEMORY_GROWTH=1  -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s USE_LIBPNG=1 main.cpp -o infiltrators.html --preload-file ./assets && rm ../Public/* && cp infiltrators.data ../Public/ && cp infiltrators.wasm ../Public/ && cp infiltrators.js ../Public/ && cp minimal_infil.html ../Public/
