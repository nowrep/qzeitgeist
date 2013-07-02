#!/bin/bash
#
# cppcheck
#

function check_code {
    cppcheck \
    -j 4 \
    --enable=all \
    --force \
    --verbose \
    --include \
    . > /dev/null
}

cd ../src/

echo "Running cppcheck in `pwd` ..."
check_code

exit 0

