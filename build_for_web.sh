#!/bin/bash

shopt -s nullglob
src_list=(./src/*)
headers=./include/
raylib_path=~/raylib/src/wasm/libraylib_wasm.a
raylib_headers=~/raylib/src/
shell_path=~/raylib/src/minshell.html
games=games/
sound_files=sound/

source ~/emsdk/emsdk_env.sh

emcc -o web/chip8.html ${src_list[*]} -Os -Wall $raylib_path \
    -I$headers -I$raylib_headers -L$raylib_path \
    -s USE_GLFW=3 --shell-file $shell_path \
    -s TOTAL_STACK=64mb  \
    -DPLATFORM_WEB \
    --preload-file $sound_files \
    --preload-file $games
