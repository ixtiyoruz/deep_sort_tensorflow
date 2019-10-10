# deep_sort_tensorflow

deepsort using deep cosine metric learning  &  hog feature extractor & darknet

- first of all please install curl, cmake and then tensorflow, and during installation after ./configure command use this command to build shared library:
<pre>
  bazel build --jobs=12 --config opt --config cuda  --config monolithic tensorflow:libtensorflow_cc.so
</pre>
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
4. download weight checkpoints from https://github.com/nwojke/deep_sort and put them in the folder  /model
5. ./yolo_console_dll {names file} {cfg file} {weights file} {video file or web_camera for web camera} {skip rate} {thresh} 
</pre>
if you want to compile it for aarch64 devices then go with make_jetson.sh
<pre>
references:
https://github.com/AlexeyAB/darknet
https://github.com/sephirothhua/DeepSort_yoloV3-HOG_feature
</pre>
