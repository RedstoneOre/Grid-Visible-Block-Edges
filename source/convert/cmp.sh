#! /bin/bash

# Grid Visible Block Edges by RedstoneOre
# Visit my profile on https://github.com/RedstoneOre

dir="${0%\/*}"
echo 'Compiling in ('"$dir"')...'
libs=( -I"$dir"'/lib/json/include/' )

g++ --std=c++17 -I"$dir" "${libs[@]}" "$dir"/main.cpp -o "$dir"/converter &&
echo 'Compiled as '"$(realpath "$dir"/converter)"
