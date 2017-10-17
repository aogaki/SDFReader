#!/bin/bash

for sdfFile in `ls *.sdf`
do
    echo $sdfFile
    sdf2root $sdfFile
done

root -l -q copyFiles.C
