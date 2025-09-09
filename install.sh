#!/bin/bash

origin="./build/metar"

if [ ! -e "$origin" ]; then
    cd ./build/
    cmake ..
    make
    cd ..
fi

destination="/usr/local/bin/metar"

if [ -e "$origin" ]; then
    cp "$origin" "$destination"
    source ~/.zshrc
fi

