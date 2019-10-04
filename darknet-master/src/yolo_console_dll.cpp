
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <thread>
#include <future>
#include <atomic>
#include <mutex>         // std::mutex, std::unique_lock
#include <cmath>
// #include "yolo_v2_class.hpp"
//#include "../src/api/deepsort.h"
#include <opencv2/opencv.hpp>            // C++
#include <opencv2/core/version.hpp>
#ifndef CV_VERSION_EPOCH     // OpenCV 3.x and 4.x
#include <opencv2/videoio/videoio.hpp>
#define OPENCV_VERSION CVAUX_STR(CV_VERSION_MAJOR)"" CVAUX_STR(CV_VERSION_MINOR)"" CVAUX_STR(CV_VERSION_REVISION)
#ifndef USE_CMAKE_LIBS
#pragma comment(lib, "opencv_world" OPENCV_VERSION ".lib")
#ifdef TRACK_OPTFLOW
#pragma comment(lib, "opencv_cudaoptflow" OPENCV_VERSION ".lib")
#pragma comment(lib, "opencv_cudaimgproc" OPENCV_VERSION ".lib")
#pragma comment(lib, "opencv_core" OPENCV_VERSION ".lib")
#pragma comment(lib, "opencv_imgproc" OPENCV_VERSION ".lib")
#pragma comment(lib, "opencv_highgui" OPENCV_VERSION ".lib")
#endif    // TRACK_OPTFLOW
#endif    // USE_CMAKE_LIBS
#else     // OpenCV 2.x
#define OPENCV_VERSION CVAUX_STR(CV_VERSION_EPOCH)"" CVAUX_STR(CV_VERSION_MAJOR)"" CVAUX_STR(CV_VERSION_MINOR)
#ifndef USE_CMAKE_LIBS
#pragma comment(lib, "opencv_core" OPENCV_VERSION ".lib")
#pragma comment(lib, "opencv_imgproc" OPENCV_VERSION ".lib")
#pragma comment(lib, "opencv_highgui" OPENCV_VERSION ".lib")
#pragma comment(lib, "opencv_video" OPENCV_VERSION ".lib")
#endif    // USE_CMAKE_LIBS
#endif    // CV_VERSION_EPOCH

/**
void draw_boxes(cv::Mat mat_img, std::vector<bbox_t> result_vec, std::vector<std::string> obj_names,
    int current_det_fps = -1, int current_cap_fps = -1)
{
    int const colors[6][3] = { { 1,0,1 },{ 0,0,1 },{ 0,1,1 },{ 0,1,0 },{ 1,1,0 },{ 1,0,0 } };

    for (auto &i : result_vec) {
        cv::Scalar color = obj_id_to_color(i.obj_id);
        cv::rectangle(mat_img, cv::Rect(i.x, i.y, i.w, i.h), color, 2);
        if (obj_names.size() > i.obj_id) {
            std::string obj_name = obj_names[i.obj_id];
            if (i.track_id > 0) obj_name += " - " + std::to_string(i.track_id);
            cv::Size const text_size = getTextSize(obj_name, cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, 2, 0);
            int max_width = (text_size.width > i.w + 2) ? text_size.width : (i.w + 2);
            max_width = std::max(max_width, (int)i.w + 2);
            //max_width = std::max(max_width, 283);
            std::string coords_3d;
            if (!std::isnan(i.z_3d)) {
                std::stringstream ss;
                ss << std::fixed << std::setprecision(2) << "x:" << i.x_3d << "m y:" << i.y_3d << "m z:" << i.z_3d << "m ";
                coords_3d = ss.str();
                cv::Size const text_size_3d = getTextSize(ss.str(), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, 1, 0);
                int const max_width_3d = (text_size_3d.width > i.w + 2) ? text_size_3d.width : (i.w + 2);
                if (max_width_3d > max_width) max_width = max_width_3d;
            }

            cv::rectangle(mat_img, cv::Point2f(std::max((int)i.x - 1, 0), std::max((int)i.y - 35, 0)),
                cv::Point2f(std::min((int)i.x + max_width, mat_img.cols - 1), std::min((int)i.y, mat_img.rows - 1)),
                color, CV_FILLED, 8, 0);
            putText(mat_img, obj_name, cv::Point2f(i.x, i.y - 16), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, cv::Scalar(0, 0, 0), 2);
            if(!coords_3d.empty()) putText(mat_img, coords_3d, cv::Point2f(i.x, i.y-1), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(0, 0, 0), 1);
        }
    }
    if (current_det_fps >= 0 && current_cap_fps >= 0) {
        std::string fps_str = "FPS detection: " + std::to_string(current_det_fps) + "   FPS capture: " + std::to_string(current_cap_fps);
        putText(mat_img, fps_str, cv::Point2f(10, 20), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, cv::Scalar(50, 255, 0), 2);
    }
}


void show_console_result(std::vector<bbox_t> const result_vec, std::vector<std::string> const obj_names, int frame_id = -1) {
    if (frame_id >= 0) std::cout << " Frame: " << frame_id << std::endl;
    for (auto &i : result_vec) {
        if (obj_names.size() > i.obj_id) std::cout << obj_names[i.obj_id] << " - ";
        std::cout << "obj_id = " << i.obj_id << ",  x = " << i.x << ", y = " << i.y
            << ", w = " << i.w << ", h = " << i.h
            << std::setprecision(3) << ", prob = " << i.prob << std::endl;
    }
}
**/

// DS_DetectObjects det2detobj(std::vector<bbox_t> result_vec){
//     DS_DetectObjects result;
    
//     for (auto &i : result_vec) {
//         DS_Rect rec;DS_DetectObject obj;
//         rec.x = i.x;rec.y = i.y;rec.width = i.w;rec.height =  i.h;
//         obj.class_id = i.obj_id;obj.rect = rec;obj.confidence = i.prob;
//         result.push_back(obj);
//     }
//     return result;
// }
// std::deque<cv::Point> line_point;

// std::vector<std::string> objects_names_from_file(std::string const filename) {
//     std::ifstream file(filename);
//     std::vector<std::string> file_lines;
//     if (!file.is_open()) return file_lines;
//     for(std::string line; getline(file, line);) file_lines.push_back(line);
//     std::cout << "object names loaded \n";
//     return file_lines;
// }

int main(int argc, char *argv[])
{
//     std::string  names_file = "./data/coco.names";
//     std::string  cfg_file = "./yolov3.cfg";
//     std::string  weights_file = "./yolov3.weights";
//     std::string filename;

//     filename = "london.mp4";
//     std::cout << filename << std::endl;
//     float const thresh =  0.2;

//     Detector detector(cfg_file, weights_file);
//     std::cout << "weights are loaded" << std::endl;
//     auto obj_names = objects_names_from_file(names_file);
//     std::string out_videofile = "result.avi";

//     DS_DetectObjects detect_objects;
//     Deep_sort Tracker = Deep_sort();
// 	DS_TrackObjects track_objects;
//     char text[30];

//         try {
//             bool show_small_boxes = false;

//             std::string const file_ext = filename.substr(filename.find_last_of(".") + 1);
//             std::string const protocol = filename.substr(0, 7);
//             if (true)   // ZED stereo camera
//             {

//                 cv::Mat cur_frame;
//                 std::atomic<int> fps_cap_counter(0), fps_det_counter(0);
//                 std::atomic<int> current_fps_cap(0), current_fps_det(0);
//                 int video_fps = 25;


//                 cv::VideoCapture cap;
//                 if (filename == "web_camera") {
//                     cap.open(0);
//                     cap >> cur_frame;
//                 } else {
//                     cap.open(filename);
//                     cap >> cur_frame;
//                 }
//                 video_fps = cap.get(cv::CAP_PROP_FPS);
//                 cv::Size const frame_size = cur_frame.size();
//                 //cv::Size const frame_size(cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_HEIGHT));
//                 std::cout << "\n Video size: " << frame_size << std::endl;

//                 cv::VideoWriter output_video;
               
//                 cv::Mat frame;

//                 uint64_t frame_id = 0;
                
//                 do {
//                     cap >> frame;
//                     fps_cap_counter++;
//                     if (frame.empty()) {
//                         frame = cv::Mat(frame_size, CV_8UC3);
//                     }
//                     // here resizing is occuring
//                     std::shared_ptr<image_t> det_image;
//                     det_image = detector.mat_to_image_resize(frame);
//                     std::vector<bbox_t> result_vec;
//                     if(det_image)
//                             result_vec = detector.detect_resized(*det_image, frame_size.width, frame_size.height, thresh, true);  // true
                    
//                     detect_objects = det2detobj(result_vec);
                    
//                     Tracker.update(detect_objects,line_point,frame);
//                     track_objects = Tracker.get_detect_obj();
                    

//                     for(auto oloop : track_objects) 
//                     {
//                             int col = int(oloop.track_id%9);
//                             // cv::Scalar color = color_map[col];
//                             cv::Scalar color;
//                             if(oloop.outside){
//                                 sprintf(text,"%d  outside",oloop.track_id);
//                                 color = cv::Scalar(255, 0, 0);}
//                             else{
//                                 sprintf(text,"%d  inside",oloop.track_id);
//                                 color = cv::Scalar(0, 0, 255);}
//                             if(oloop.rect.x<0)
//                             {
//                                 oloop.rect.x = 0;
//                             }
//                             if(oloop.rect.y<0)
//                             {
//                                 oloop.rect.y = 0;
//                             }
//                             if((oloop.rect.x + oloop.rect.width)>frame.size[1])
//                             {
//                                 oloop.rect.width = frame.size[1]-oloop.rect.x;
//                             }
//                             if((oloop.rect.y + oloop.rect.height)>frame.size[0])
//                             {
//                                 oloop.rect.height = frame.size[0]-oloop.rect.y;
//                             }
//                             int font_face = cv::FONT_HERSHEY_COMPLEX;
//                             cv::Rect box = cv::Rect(oloop.rect.x,oloop.rect.y,oloop.rect.width,oloop.rect.height);
//                             cv::rectangle(frame,box,color,2,1);
//                             // Point origin = Point(oloop.rect.x,oloop.rect.y-2);
//                             // cv::putText(frame, text, origin, font_face, 0.5, color, 1, 8, 0);
//                             // if(oloop.tracklet.size()>=2){
//                             // for(int i=0;i<(oloop.tracklet.size()-1);i++){
//                                 // cv::line(frame, oloop.tracklet[i],oloop.tracklet[i+1], color, 2);
//                                 // cv::circle(frame, oloop.tracklet[i], 3, color);
//                                 // }
//                             // }
//                     cv::putText(frame, std::to_string(oloop.track_id), cv::Point(oloop.rect.x-5,oloop.rect.y), font_face, 0.7, cv::Scalar(255, 0, 255), 2, 8, 0);
//                     }



//                     // fps_det_counter++;
//                     // cv::Mat draw_frame = cap_frame.clone();
//                     // draw_boxes(draw_frame, result_vec, obj_names, current_fps_det, current_fps_cap);
//                     // draw_frame = draw_frame;

//                     // cv::imshow("window name", draw_frame);
//                     cv::imshow("Detect", frame);
//                     int key = cv::waitKey(3);    // 3 or 16ms
//                     if (key == 'p') while (true) if (cv::waitKey(100) == 'p') break;
//                     if (key == 27) { break;}
//                 } while (true);

//                 cv::destroyWindow("window name");
//             }
//         }
//         catch (std::exception &e) { std::cerr << "exception: " << e.what() << "\n"; getchar(); }
//         catch (...) { std::cerr << "unknown exception \n"; getchar(); }
//         filename.clear();

    return 0;
}
