#!/bin/bash

gcc -w dct.c -o dct -lm;

./dct cam1_64_64.raw 64 64;
