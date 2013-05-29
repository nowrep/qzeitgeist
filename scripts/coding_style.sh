#!/bin/bash
#
# Requirements:
#   astyle >= 2.02
#

function format_source {
    astyle --indent=spaces=4 --style=1tbs \
       --indent-labels --pad-oper --unpad-paren --pad-header \
       --keep-one-line-statements --convert-tabs --break-closing-brackets \
       --align-pointer=name --align-reference=name --indent-preprocessor \
       `find -type f -name '*.cpp' -or -name '*.h'` | grep 'Formatted'

    find . -name "*.orig" -print0 | xargs -0 rm -f
}

cd ../src

echo "Running astyle in `pwd` ..."
format_source

exit 0
