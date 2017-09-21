#!/bin/bash

for number in 0003 0004 0006 0009 0011 0012 0015 0017 0019 0021 0023
do
    echo $number
    root -l -q "MakeSlice.cpp+O(\"$number\")"
done
