#!/bin/bash

gcc -w filter.c -o filter -lm;

./filter boat_512_512_dis.raw 512;
