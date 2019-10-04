cp CMakeLists_jetson.txt tracker/src/CMakeLists.txt
cd tracker
sh makelib.sh

cd ..

cd darknet-master
make -j8

cd ..

cp ./darknet-master/libdarknet.so ./
cp ./tracker/libdeepsort.so ./

sudo cp libdarknet.so /usr/lib/
sudo cp libdeepsort.so /usr/lib/
rm *.so

mkdir build
cd build
cmake ..
make

cp code/yolo_tracker ../
