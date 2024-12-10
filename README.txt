

#Install cadical library:
cd /home/jsander/ParallelSolver/lib
chmod -R 755 cadical
cd cadical
./configure && make
cd ../..

#Create directory build
mkdir build-dir
cd build-dir
cmake ..
make