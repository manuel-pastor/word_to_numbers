# word_to_numbers
Translate english words to numbers in the middle of a sentence

Compile in the usual way with CMake:

-cd to the folder where your code is
-mkdir build
-cd build
-cmake ..
-make 

The code has been tested on Mac, Linux and Windows. It probably works on BSD too, as long as it has cmake and a C++ 14 compiler.

TODO:

-The handling of what "a number is" is pretty naive, it should deal with special cases as : "No one went to party"
