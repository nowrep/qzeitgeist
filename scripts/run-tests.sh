#!/bin/bash

DIRS="event
log
mimetypes
symbols
timerange"

cd ../build && make

cd tests

for dir in $DIRS; do
    cd $dir
    ./test-$dir || exit 1
    cd ..
done
