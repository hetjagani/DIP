#!/bin/bash

gcc -w gaussian_noise.c -o noise -lm;
./noise boat_512_512.raw 512 512;
