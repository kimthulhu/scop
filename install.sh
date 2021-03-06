#!/bin/bash

#HOW TO USE: source install.sh

#if you still need to install homebrew and/or SDL, use:
#/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
#brew install SDL2

export C_INCLUDE_PATH=~/homebrew/Cellar/sdl2/2.0.5/include/
export LIBRARY_PATH=~/homebrew/Cellar/sdl2/2.0.5/lib
# Alternatively:
#export C_INCLUDE_PATH=~/.brew/include
#export LIBRARY_PATH=~/.brew/lib