/*
 * FeatureTensor.cpp
 *
 *  Created on: Dec 15, 2017
 *      Author: zy
 */

#include "FeatureTensor.h"
#include <iostream>
using namespace tensorflow;

#define TENSORFLOW_MODEL_META "/home/ixtiyor/Documents/deepsort/model/mars.ckpt.meta"
#define TENSORFLOW_MODEL "/home/ixtiyor/Documents/deepsort/model/mars.ckpt"

FeatureTensor *FeatureTensor::instance = NULL;

FeatureTensor *FeatureTensor::getInstance() {
	if(instance == NULL) {
		std::cout <<" new model --------------------" << std::endl;
		instance = new FeatureTensor();
	}
	return instance;
}

FeatureTensor::FeatureTensor() {
	//prepare model:
	std::cout<<"init started"<<std::endl;

	bool status = init();
	std::cout<<"init finished"<<std::endl;
	
	if(status == false)
	  {
	    std::cout<<"init failed"<<std::endl;
	  exit(1);
	  }
	else {
	    std::cout<<"init succeed"<<std::endl;
	  }
}

FeatureTensor::~FeatureTensor() {
	session->Close();
	delete session;
	output_tensors.clear();
	outnames.clear();
}

bool FeatureTensor::init() {
	tensorflow::SessionOptions sessOptions;
	//sessOptions.config.mutable_gpu_options()->set_allow_growth(true);
	session = NewSession(sessOptions);
	printf("session creation:%s\n\n\n\n\n", "1");
	if(session == nullptr) return false;

	const tensorflow::string pathToGraph = TENSORFLOW_MODEL_META;
	Status status;
	MetaGraphDef graph_def;
	//tensorflow::GraphDef graph_def;//不同类型的文件不同的定义

	status = ReadBinaryProto(tensorflow::Env::Default(), pathToGraph, &graph_def);
	if(status.ok() == false) return false;

	status = session->Create(graph_def.graph_def());
	if(status.ok() == false) return false;
	std::vector<std::string> node_names;
	printf("node larni chiqarish joyiga keldi:%s\n\n\n\n\n", "1");
	for (const auto &node : graph_def.graph_def().node()) {
		printf("node name:%s\n", node.name().c_str());

	
	}
	const tensorflow::string checkpointPath = TENSORFLOW_MODEL;
	Tensor checkpointTensor(DT_STRING, TensorShape());
	checkpointTensor.scalar<std::string>()() = checkpointPath;

	status = session->Run(
			{ {graph_def.saver_def().filename_tensor_name(), checkpointTensor}, },
			{}, {graph_def.saver_def().restore_op_name()}, nullptr );
	
	if(status.ok() == false) return false;

	input_layer = "Placeholder";
	outnames.push_back("l2_normalize");
	feature_dim = 128;


	return true;
}

bool FeatureTensor::getRectsFeature(const cv::Mat& img, DETECTIONS& d) {
	std::vector<cv::Mat> mats;
	for(DETECTION_ROW& dbox : d) {
		cv::Rect rc = cv::Rect(int(dbox.tlwh(0)), int(dbox.tlwh(1)),
				int(dbox.tlwh(2)), int(dbox.tlwh(3)));
		rc.x -= (rc.height * 0.5 - rc.width) * 0.5;
		rc.width = rc.height * 0.5;
		rc.x = (rc.x >= 0 ? rc.x : 0);
		rc.y = (rc.y >= 0 ? rc.y : 0);
		rc.width = (rc.x + rc.width <= img.cols? rc.width: (img.cols-rc.x));
		rc.height = (rc.y + rc.height <= img.rows? rc.height:(img.rows - rc.y));

		cv::Mat mattmp = img(rc).clone();
		//cv::Mat mattmp = img.clone();
		cv::resize(mattmp, mattmp, cv::Size(64, 128));
		mats.push_back(mattmp);
	}
	int count = mats.size();

	Tensor input_tensor(DT_UINT8, TensorShape({count, 128, 64, 3}));
	tobuffer(mats, input_tensor.flat<uint8>().data());
	std::vector<std::pair<tensorflow::string, Tensor>> feed_dict = {
			{input_layer, input_tensor},
	};

	Status status = session->Run(feed_dict, outnames, {}, &output_tensors);
	if(status.ok() == false)
	  return false;
	float* tensor_buffer = output_tensors[0].flat<float>().data();
	int i = 0;
	for(DETECTION_ROW& dbox : d) {
		for(int j = 0; j < feature_dim; j++)
			dbox.feature[j] = tensor_buffer[i*feature_dim+j];
		i++;
	}
	return true;
}
float* FeatureTensor::getRectsFeature_single(const cv::Mat& img, DETECTION_ROW& dbox) {
	cv::Mat mat;
	cv::Rect rc = cv::Rect(int(dbox.tlwh(0)), int(dbox.tlwh(1)),
			int(dbox.tlwh(2)), int(dbox.tlwh(3)));
	rc.x -= (rc.height * 0.5 - rc.width) * 0.5;
	rc.width = rc.height * 0.5;
	rc.x = (rc.x >= 0 ? rc.x : 0);
	rc.y = (rc.y >= 0 ? rc.y : 0);
	rc.width = (rc.x + rc.width <= img.cols? rc.width: (img.cols-rc.x));
	rc.height = (rc.y + rc.height <= img.rows? rc.height:(img.rows - rc.y));

	mat = img(rc).clone();
	//cv::Mat mattmp = img.clone();
	cv::resize(mat, mat, cv::Size(64, 128));

	Tensor input_tensor(DT_UINT8, TensorShape({1, 128, 64, 3}));
	tobuffer_single(mat, input_tensor.flat<uint8>().data());
	//std::cout << " buffering finished" << "\n\n";
	std::vector<std::pair<tensorflow::string, Tensor>> feed_dict = {
			{input_layer, input_tensor},
	};

	Status status = session->Run(feed_dict, outnames, {}, &output_tensors);
	//std::cout << " status -- " << status.ok() << "\n\n";
	if(status.ok() == false)
	  return false;
	//here it is unknown whether output tensor size is actually 1
	// float* tensor_buffer = output_tensors[0].flat<float>().data();
	// for(int j = 0; j < feature_dim; j++){
	// 	dbox.feature[j] = tensor_buffer[j];
	// 	std::cout << tensor_buffer[j]  << std::endl;
	// }
	return output_tensors[0].flat<float>().data();;
}

//this one mag give error , but i think it is correct
void FeatureTensor::tobuffer_single(const cv::Mat &mat, uint8 *buf) {
	int pos = 0;
	int Lenth = mat.rows * mat.cols * 3;
	int nr = mat.rows;
	int nc = mat.cols;
	if(mat.isContinuous()) {
		nr = 1;
		nc = Lenth;
	}
	for(int i = 0; i < nr; i++) {
		const uchar* inData = mat.ptr<uchar>(i);
		for(int j = 0; j < nc; j++) {
			buf[pos] = *inData++;
			pos++;
		}
	}//end for

}

void FeatureTensor::tobuffer(const std::vector<cv::Mat> &imgs, uint8 *buf) {
	int pos = 0;
	for(const cv::Mat& img : imgs) {
		int Lenth = img.rows * img.cols * 3;
		int nr = img.rows;
		int nc = img.cols;
		if(img.isContinuous()) {
			nr = 1;
			nc = Lenth;
		}
		for(int i = 0; i < nr; i++) {
			const uchar* inData = img.ptr<uchar>(i);
			for(int j = 0; j < nc; j++) {
				buf[pos] = *inData++;
				pos++;
			}
		}//end for
	}//end imgs;
}
void FeatureTensor::test()
{
return;
}
