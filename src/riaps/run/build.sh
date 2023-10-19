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
clang++ -std=c++17 -stdlib=libc++ -fno-common -dynamic -DNDEBUG -g -fwrapv -O3 -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX12.3.sdk -c comp.cpp -o build/comp.o
clang++ -std=c++17 -stdlib=libc++ -fno-common -dynamic -DNDEBUG -g -fwrapv -O3 -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX12.3.sdk -I /opt/homebrew/Cellar/zeromq/4.3.4/include -I /opt/homebrew/Cellar/cppzmq/4.10.0/include -c launch.cpp -o build/launch.o
clang++ -std=c++17 -stdlib=libc++ -fno-common -dynamic -DNDEBUG -g -fwrapv -O3 -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX12.3.sdk -I /opt/homebrew/Cellar/zeromq/4.3.4/include -I /opt/homebrew/Cellar/cppzmq/4.10.0/include -I /Users/EthanMayer_1/Desktop/json/single_include/nlohmann -c threadBody.cpp -o build/threadBody.o

# Bundle compiled C code into .so
#echo "====Bundling C++ code...===="
clang++ -bundle -undefined dynamic_lookup -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX12.3.sdk build/launch.o -L /opt/homebrew/Cellar/zeromq/4.3.4/lib -L /opt/homebrew/Cellar/cppzmq/4.10.0/lib -lzmq -o launch.so
clang++ -bundle -undefined dynamic_lookup -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX12.3.sdk build/threadBody.o -L /opt/homebrew/Cellar/zeromq/4.3.4/lib -L /opt/homebrew/Cellar/cppzmq/4.10.0/lib -L /Users/EthanMayer_1/Desktop/json -lzmq -o threadBody.so

# python3.11 TEST_DRIVER.py