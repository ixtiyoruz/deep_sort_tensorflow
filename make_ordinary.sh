#rm -r tracker/build/

cp CMakeLists_ordinary.txt tracker/src/CMakeLists.txt
cd tracker
sh makelib.sh

cd ..

cd darknet-master
make -j8

cd ..

#sudo cp ./darknet-master/libdarknet.so ./code/
#sudo cp ./tracker/libdeepsort.so ./code/
#sudo rm /usr/lib/libdarknet.so
#sudo rm /usr/lib/libdeepsort.so

sudo cp ./darknet-master/libdarknet.so /usr/lib/
#sudo cp ./tracker/libdeepsort.so /usr/lib/

#rm *.so

 
# source code directory of tensorflow
TF_DIR=/home/essys/Downloads/tensorflow/

# external source code directory of tensorflow
TF_EXTERNAL_DIR=/home/essys/Downloads/tensorflow/bazel-tensorflow

# bazel build directory of tensorflow where `libtensorflow.so` exists.
# Please specify absolute path, otherwise cmake cannot find lib **. A
TF_BUILD_DIR=/home/essys/Downloads/tensorflow/bazel-bin/tensorflow


cmake -DTENSORFLOW_DIR=${TF_DIR}\
      -DTENSORFLOW_EXTERNAL_DIR=${TF_EXTERNAL_DIR}\
      -DTENSORFLOW_BUILD_DIR=${TF_BUILD_DIR}\
      -DSANITIZE_ADDRESS=On\
      -DCMAKE_BUILD_TYPE=Debug\
      -Bbuild\
      -H.

cd build
make
cp code/yolo_console_dll ../
