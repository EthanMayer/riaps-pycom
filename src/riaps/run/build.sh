#   build.sh
#   
#   Author: Ethan Mayer
#   Spring 2023
#
#   This is the script used to build the Cython component of the project, compile the C code, and bundle the compiled C code into a shared library.

# Make and clean build folder
mkdir build
rm -rf ~/build/*

# Build Cython
echo "====Building Cython files...===="
python3.11 setup.py build_ext -b . -t ./build

# Compile C code
echo "====Compiling C code...===="
clang -fno-common -dynamic -DNDEBUG -g -fwrapv -O3 -Wall -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX11.sdk -I /opt/homebrew/Cellar/zeromq/4.3.4/include -I /opt/homebrew/Cellar/python@3.11/3.11.2_1/Frameworks/Python.framework/Versions/3.11/include/python3.11 -c TEST_funcBody.c -o build/TEST_funcBody.o

# Bundle compiled C code into .so
echo "====Bundling C code...===="
clang -bundle -undefined dynamic_lookup -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX11.sdk build/TEST_funcBody.o -L /opt/homebrew/Cellar/zeromq/4.3.4/lib -lzmq -o TEST_funcBody.so