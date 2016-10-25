#!/usr/bin/env bash
autoreconf -iv
./configure --prefix=$HOME/.local CXXFLAGS='-std=gnu++14'
