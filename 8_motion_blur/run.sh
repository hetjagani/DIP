#!/bin/bash

gcc -w motion.c -o motion -lm;

./motion blurry.raw 256;
