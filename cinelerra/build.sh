cd ~/repos/cinelerra-cv
git checkout 6ceda8ab4c5f91b50000756717b0e943ef6a9226
./autogen.sh
./configure --prefix=$HOME/.local
make
make install
echo "Run using cinelerracv"
