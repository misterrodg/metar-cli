#!/bin/bash

destination="/usr/local/bin/metar"

if [ -e "$destination" ]; then
    rm "$destination"
    source ~/.zshrc
else
    echo "executable not found."
fi

