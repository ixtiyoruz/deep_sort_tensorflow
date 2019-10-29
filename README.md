# deep_sort_tensorflow

deepsort using deep cosine metric learning  &  hog feature extractor & darknet

- first of all please install curl, cmake and then tensorflow, and during installation after ./configure command use this command to build shared library:
<pre>
  bazel build --jobs=12 --config opt --config cuda  --config monolithic tensorflow:libtensorflow_cc.so
</pre>
- in order to install tensorflow for jetson products which has aarch64 architecture you should follow https://devtalk.nvidia.com/default/topic/1055131/jetson-agx-xavier/building-tensorflow-1-13-on-jetson-xavier/ untill configure finished and then run the command above.
- when it  finishes crating .so file , you have to go into /tensorflow/tensorflow/contrib/makefile and run the command ./build_all_linux.sh (it may fail at some point, it is ok  as long as you are not using whole tensorflow)
- download the repository
- according to whereabouts of your tensorflow, please change:
<pre>
1. make_ordinary.sh
2. make_jetson.sh
3. tracker/makelib.sh
4. CMakeLists_jetson.txt
5. CMakeLists_ordinary.txt
</pre>

usage 
<pre>
0.1 chmod a+x make_ordinary.sh
0.2 chmod a+x make_jetson.sh
0.3 chmod a+x clean.sh
1. sudo ./clean.sh
2. make_ordinary.sh
3. download weight files from https://github.com/AlexeyAB/darknet
4. download weight checkpoints from https://github.com/nwojke/cosine_metric_learning and put them in the folder  /model
5. ./yolo_console_dll {names file} {cfg file} {weights file} {video file or web_camera for web camera} {skip rate} {thresh} 
</pre>
if you want to compile it for aarch64 devices then go with make_jetson.sh
<pre>
references:
https://github.com/AlexeyAB/darknet
https://github.com/sephirothhua/DeepSort_yoloV3-HOG_feature
</pre>
