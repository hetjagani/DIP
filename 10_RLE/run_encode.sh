#!/bin/bash

gcc -w encode.c -o encode -lm;

./encode image.raw 512 512;

echo "Size of encoded file";
ls -lh encoded;
