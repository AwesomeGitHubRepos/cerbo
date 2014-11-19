DOWNLOAD:

http://mij.oltrelinux.com/devel/simclist/

tar xvf  simclist-1.5.tar.bz2
cd  simclist-1.5
cmake .
make
sudo cp simclist.h /usr/local/include

Linux:
   sudo cp libsimclist.so /usr/local/lib
   sudo ldconfig
Cygwin:
   cp cygsimclist.dll /usr/local/lib
   cp libsimclist.dll.a /usr/local/lib
   Still doesn't quite work

Compile example:
   cd examples
gcc ex1.c -lsimclist -o ex1

Leak test:
valgrind --leak-check=yes --show-leak-kinds=all ./ex1

