#!/bin/bash

rm vtuner-simple

# uncomment this lines for ARM
#CFLAGS="-mcpu=cortex-a9 -march=armv7-a -mfpu=neon -mfpu=vfpv3 -mfloat-abi=soft"
#CC=arm-linux-gnueabi-gcc

CC=gcc
$CC $CFLAGS -O2 -o vtuner-simple vtuner-simple.c
