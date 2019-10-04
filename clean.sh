cd darknet-master
make clean

cd ../tracker
sh clean.sh


cd ..
rm tracker/src/CMakeLists.txt
rm yolo_tracker
rm -r build
rm ./code/*.o
rm *.so
rm yolo_console_dll
rm ./code/*.so
