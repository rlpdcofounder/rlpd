#!/bin/sh

cd "$(dirname "$0")"
g++ -Wall -Iinclude -c src/*.cpp
g++ *.o -o rlpd
