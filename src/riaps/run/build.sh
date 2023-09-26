#   build.sh
#   
#   Author: Ethan Mayer
#   Fall 2023
#
#   This is the script used to build the C++ component of the project and bundle the compiled code into a shared library.

# Make and clean build folder
mkdir build
rm -rf ~/build/*

# Compile C code
echo "====Compiling C++ code...===="
clang++ -std=c++17 -fno-common -dynamic -DNDEBUG -g -fwrapv -O3 -Wall -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX11.sdk -I /opt/homebrew/Cellar/zeromq/4.3.4/include -c comp.cpp -o build/comp.o

# Bundle compiled C code into .so
#echo "====Bundling C++ code...===="
#clang -bundle -undefined dynamic_lookup -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX11.sdk build/TEST_funcBody.o -L /opt/homebrew/Cellar/zeromq/4.3.4/lib -lzmq -o TEST_funcBody.so