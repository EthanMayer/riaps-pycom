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
clang++ -std=c++17 -stdlib=libc++ -fno-common -dynamic -DNDEBUG -g -fwrapv -O3 -Wall -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX12.3.sdk -c comp.cpp -o build/comp.o
clang++ -std=c++17 -stdlib=libc++ -fno-common -dynamic -DNDEBUG -g -fwrapv -O3 -Wall -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX12.3.sdk -c launch.cpp -o build/launch.o

# Bundle compiled C code into .so
#echo "====Bundling C++ code...===="
clang++ -bundle -undefined dynamic_lookup -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX12.3.sdk build/launch.o -o launch.so