#!/bin/bash

for filename in ./tests/*.in; do
    echo $filename
    ./grader ./engine < $filename
done