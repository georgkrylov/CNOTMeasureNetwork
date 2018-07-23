# CNOTMeasureNetwork
C++ implementation of a network described in CNOT-Measure Quantum Neural Networks  (Martin Lukac, Kamila Abdiyeva and Michitaka Kameyama), presented at ISMVL 2018 

Installation:
Quantum tool:
https://github.com/vsoftco/qpp
Clone it somewhere, say $QPPATH

Eigen3 library
http://eigen.tuxfamily.org/index.php?title=Main_Page
$sudo apt install g++
$sudo apt install git
$ sudo apt install cmake
$mkdir /tmp/eig3/
$cd /tmp/eig3/
$git clone https://github.com/eigenteam/eigen-git-mirror.git
$ cd eigen-git-mirror/
$mkdir bl
$cd bl
$cmake ../
$ sudo make install

This will be in /usr/local/include/eigen3 , call it $EIGPATH

To link libraries, add -std=c++11 -isystem $QPPATH -I $EIGPATH to your g++ compilation line.
