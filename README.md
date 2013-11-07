Qt Zeitgeist Library
----------------------------------------------------------------------------------------
Qt Zeitgeist library using libzeitgeist2.

Compiling
----------------------------------------------------------------------------------------
Make sure you have installed libzeitgeist2 development files. (On Debian`libzeitgeist2.0-dev`).
Doxygen is used for generating documentation.

    $ mkdir build && cd build
    $ cmake ..
    $ make
    $ make install

Tests, documentation and demos are all built by default. You can disable any of them:

    cmake {OPTIONS} ..

    OPTIONS:
       -DBUILD_TESTS=FALSE   disable building tests
       -DBUILD_DEMOS=FALSE   disable building demos
       -DBUILD_DOC=FALSE     disable building documentation
