#!/bin/bash

gcc -w zoom.c -o zoom -lm;
./zoom cam2_128_128.raw 128 128;
