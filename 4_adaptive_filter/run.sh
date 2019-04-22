#!/bin/bash

gcc -w filter.c -o filter -lm;

./filter boat_noise.raw 512 512;