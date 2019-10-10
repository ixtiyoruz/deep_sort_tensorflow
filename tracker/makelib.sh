cp -f ./src/api/deepsort.h ./include/
mkdir build && cd build
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
      -H../src/
cd build
make -j4

cd ..
sudo cp build/libdeepsort.so /usr/lib/


