#!/bin/bash

gcc -w filter.c -o filter -lm;
./filter boat_512_512.raw 512;
