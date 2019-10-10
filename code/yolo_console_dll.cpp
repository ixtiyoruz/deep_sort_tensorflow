#include "yolo_console_dll.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <thread>
#include <future>
#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end
#include <atomic>
#include <string>
#include <mutex>         // std::mutex, std::unique_lock
#include <cmath>
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


DS_DetectObjects det2detobj(std::vector<bbox_t> result_vec){
    DS_DetectObjects result;
    //person - car ... bus ..
    //int a[] = {0, 1, 2, 3, 5, 6, 7};
    //without person
    //int a[] = {1, 2, 3, 5, 6, 7};
    //only person
    int a[] = {0};
    for (auto &i : result_vec) {
        bool exists = std::find(std::begin(a), std::end(a), i.obj_id) != std::end(a);
        if(exists){
            DS_Rect rec;DS_DetectObject obj;
            rec.x = i.x;rec.y = i.y;rec.width = i.w;rec.height =  i.h;
            obj.class_id = i.obj_id;obj.rect = rec;obj.confidence = i.prob;
            result.push_back(obj);
        }
    }
    return result;
}
std::deque<cv::Point> line_point;

std::vector<std::string> objects_names_from_file(std::string const filename) {
    std::ifstream file(filename);
    std::vector<std::string> file_lines;
    if (!file.is_open()) return file_lines;
    for(std::string line; getline(file, line);) file_lines.push_back(line);
    std::cout << "object names loaded \n";
    return file_lines;
}
int main(int argc, char *argv[])
{
    std::string  names_file = "./data/coco.names";
    std::string  cfg_file = "./yolov3.cfg";
    std::string  weights_file = "./yolov3.weights";
    std::string filename = "test.mp4";

    if(argc > 4){ // weights , cfg, names, videopath 
        names_file = argv[1];
        cfg_file = argv[2];

        weights_file = argv[3];
        filename = argv[4];
    }

    int const skip_rate = (argc > 5) ? std::stof(argv[5]) : 3;
    float const thresh = (argc > 6) ? std::stof(argv[6]) : 0.2;
    std::cout << filename << std::endl;
    Detector detector(cfg_file, weights_file);
    std::cout << "weights are loaded" << std::endl;
    auto obj_names = objects_names_from_file(names_file);
    std::string out_videofile = "result.avi";

    DS_DetectObjects detect_objects;
    Deep_sort Tracker = Deep_sort();
	DS_TrackObjects track_objects;
    
    char text[30];
        try {
            bool show_small_boxes = false;

            std::string const file_ext = filename.substr(filename.find_last_of(".") + 1);
            std::string const protocol = filename.substr(0, 7);
            if (true)   // ZED stereo camera
            {
 
                cv::Mat frame;
                int video_fps = 25;


                cv::VideoCapture cap;
                if (filename == "web_camera") {
                    cap.open(0);
                    cap >> frame;
                } else {
                    cap.open(filename);
                    cap >> frame;
                }
                video_fps = cap.get(cv::CAP_PROP_FPS);
                cv::Size const frame_size = frame.size();
                //cv::Size const frame_size(cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_HEIGHT));
                std::cout << "\n Video size: " << frame_size << std::endl;

                int k = 10;
                cv::namedWindow("Detect",1);
                bool miss = false;
		clock_t start,finish;
		double totaltime;
                int font_face = cv::FONT_HERSHEY_COMPLEX;
                int frame_count = 0;
		start  = clock();
                do {
                    
                    if(k % skip_rate == 0){
                        miss = false;
                    }else{
                        miss = true;
                    }
                    k++;
                    if(miss){
                        cap.grab();
                    }else{
                        cap.grab();
                        cap.retrieve(frame);
                    }
                    
                    if (frame.empty()) {
                        frame = cv::Mat(frame_size, CV_8UC3);
                    }
                    // here resizing is occuring
                    std::shared_ptr<image_t> det_image;
                    std::vector<bbox_t> result_vec;

                    if(!miss){
                        //std::cout<<k<<miss<<std::endl;
                        det_image = detector.mat_to_image_resize(frame);
                        if(det_image){
                            result_vec = detector.detect_resized(*det_image, frame_size.width, frame_size.height, thresh, true);  // true
                            detect_objects = det2detobj(result_vec);
                            Tracker.update(detect_objects,line_point,frame);
                        }
                    }
                    else{
                        std::cout<<"skip"<<std::endl;
                    }

                    track_objects = Tracker.get_detect_obj();
                    

                    for(auto oloop : track_objects) 
                    {
                            int col = int(oloop.track_id%9);
                            // cv::Scalar color = color_map[col];
                            cv::Scalar color;
                            if(oloop.outside){
                                sprintf(text,"%d  outside",oloop.track_id);
                                color = cv::Scalar(255, 0, 0);}
                            else{
                                sprintf(text,"%d  inside",oloop.track_id);
                                color = cv::Scalar(0, 0, 255);}
                            if(oloop.rect.x<0)
                            {
                                oloop.rect.x = 0;
                            }
                            if(oloop.rect.y<0)
                            {
                                oloop.rect.y = 0;
                            }
                            if((oloop.rect.x + oloop.rect.width)>frame.size[1])
                            {
                                oloop.rect.width = frame.size[1]-oloop.rect.x;
                            }
                            if((oloop.rect.y + oloop.rect.height)>frame.size[0])
                            {
                                oloop.rect.height = frame.size[0]-oloop.rect.y;
                            }
                            
                            cv::Rect box = cv::Rect(oloop.rect.x,oloop.rect.y,oloop.rect.width,oloop.rect.height);
                            cv::rectangle(frame,box,color,2,1);
                            cv::Point origin = cv::Point(oloop.rect.x,oloop.rect.y-2);
                            // if(oloop.tracklet.size()>=2){
                            // for(int i=0;i<(oloop.tracklet.size()-1);i++){
                            //     cv::line(frame, oloop.tracklet[i],oloop.tracklet[i+1], color, 2);
                            //     cv::circle(frame, oloop.tracklet[i], 3, color);
                            //     }
                            // }
                        
                        cv::putText(frame, std::to_string(oloop.track_id), cv::Point(oloop.rect.x,oloop.rect.y-5), font_face, 0.7, cv::Scalar(255, 0, 255), 2, 8, 0);
                    }

                    // cv::Mat draw_frame = cap_frame.clone();
                    // draw_boxes(draw_frame, result_vec, obj_names, current_fps_det, current_fps_cap);
                    // draw_frame = draw_frame;

                    // cv::imshow("Detect", draw_frame);
                    frame_count++;
//                    if(!miss)
			//totaltime = ((double)(clock()-start)/CLOCKS_PER_SEC)/ frame_count;
            //cv::putText(frame, (std::to_string(1/totaltime)).append(" fps"), cv::Point(30,30), font_face, 0.7, cv::Scalar(155, 0, 155), 2, 4, 0);
			//if(frame_count >= 1000){
			//    frame_count = 0;
            //    start = clock();
			//}
                    cv::imshow("Detect", frame);
                    int key = cv::waitKey(3);    // 3 or 16ms
                    if (key == 'p') while (true) if (cv::waitKey(100) == 'p') break;
                    if (key == 27) { break;}
                } while (true);

                cv::destroyWindow("Detect");
            }
        }
        catch (std::exception &e) { std::cerr << "exception: " << e.what() << "\n"; getchar(); }
        catch (...) { std::cerr << "unknown exception \n"; getchar(); }
        filename.clear();

    return 0;
}
