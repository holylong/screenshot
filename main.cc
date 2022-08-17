// MIT License

// Copyright (c) 2022 hooy

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


/**
 * @file main.cc
 * @author holylong (mrhlingchen@163.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <H264Encoder.hpp>
#include <H264Mgr.hpp>
#include <fstream>
#include <DXGIScreenShot.hpp>
#include <BaseUtil.hpp>
#include <iomanip>
#include <vector>
#include <numeric>
#include <chrono> // time duration
#include <YuvConvertor.hpp>

// #undef USE_OPENCV

#ifdef USE_OPENCV
#include <opencv2/opencv.hpp>
using namespace cv;
#endif

#define DUMP_FILE_H264
// #define DUMP_FILE_YUV
// #define DUMP_FILE_DEC_YUV
#define USE_DECODER

#if 1

void printHelp(char* argv[]){
	std::cout << "you can input arguments output file like:" << argv[0] << " output.264" << std::endl;
}

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printHelp(argv);
	}
	int res = 0;

	//init dxgi
	auto shoter = std::make_shared<sshot::DXGIScreenShot>();
	if (!shoter->setup()) {
		std::cout << "error" << std::endl;
		return -1;
	}
	RECT rect = shoter->get_output_rect();
	BYTE * pRgb = (BYTE*)malloc((rect.right - rect.left)* 4 * (rect.bottom - rect.top));
	size_t pSize = 0;
#ifdef USE_OPENCV
	Mat mat;
	mat = cv::Mat::zeros(rect.bottom - rect.top, rect.right - rect.left, CV_8UC4);
	namedWindow("img", 0);
#endif //USE_OPENCV

	float fps = 0.0f;
	short count = 0;
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	std::chrono::time_point pretime = std::chrono::system_clock::now();
	std::string fpstr = "";
	uint8_t* yuvdata = NULL;
	int yuvSize = width*height*3/2;
	yuvdata = (uint8_t*)malloc(yuvSize);
	int h264Size = 0;
	uint8_t* h264buf = (uint8_t*)malloc(1024*1024);

	uint8_t* decbuf[3];
	decbuf[0] = (uint8_t*)malloc(width * height);
	decbuf[1] = (uint8_t*)malloc((1+width * height)>>2);
	decbuf[2]= (uint8_t*)malloc((1+width * height)>>2);

		//init h264mgr
	auto encoder = sshot::createH264Mgr();
	encoder->Init(width, height, 15, 15, 0);

#ifdef USE_DECODER
	auto decoder = sshot::createH264Mgr(sshot::Mgr_Decode);
	decoder->Init(width, height);
#endif //USE_DECODER

#ifdef DUMP_FILE_YUV
	std::ofstream ofsyuv;
	ofsyuv.open("p.yuv", std::ios::binary);
#endif //DUMP_FILE_YUV

#if defined(DUMP_FILE_DEC_YUV) && defined(USE_DECODER)
	std::ofstream ofsdecyuv;
	ofsdecyuv.open("dec.yuv", std::ios::binary);
#endif //DUMP_FILE_DEC_YUV && USE_DECODER

#ifdef DUMP_FILE_H264
	std::ofstream ofsh264;
	if(argc == 2)
		ofsh264.open(argv[1], std::ios::binary);
		else ofsh264.open("output.264", std::ios::binary);
#endif //DUMP_FILE_H264

	while (count < 100)
	{
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - pretime);
		pretime = std::chrono::system_clock::now();

		fps = 1000.0f/duration.count();

		if(count++%10 == 0)
		{
			fpstr = "fps:";
			fpstr += sshot::SystemUtil::to_string(fps, 2);
		}
		shoter->get_output_data(&pRgb, &pSize);
//  #ifdef USE_OPENCV
//  		mat.data = pRgb;
//  		// putText fps
//  		cv::putText(mat,
//  		fpstr.c_str(),
//  		cv::Point(mat.cols-200, 100),
//  		cv::FONT_HERSHEY_PLAIN,
//  		2,
//  		cv::Scalar(255,125,125, 255), 2, 8, false);

//  		imshow("img", mat);
//  		waitKey(1);
//  #endif //USE_OPENCV

		//rgb2yuv
		if((res = sshot::YuvConvertor::ARGBToI420(pRgb, width, height, &yuvdata)) == 0)
		{
			// std::cout << "yuv convert ok" << std::endl;
#ifdef DUMP_FILE_YUV
		if (ofsyuv.is_open() && yuvSize > 0)
		{
			ofsyuv.write((char*)yuvdata, yuvSize);
			ofsyuv.flush();
		}
#endif //DUMP_FILE_YUV
		if((res = encoder->Encode(yuvdata, h264buf, &h264Size)) == 0)
		{
			std::cout << "encoder ok: " << h264Size << " bytes" << std::endl;
#ifdef DUMP_FILE_H264
			if (ofsh264.is_open() && h264Size > 0)
			{
				ofsh264.write((char*)h264buf, h264Size);
				ofsh264.flush();
			}
#endif //DUMP_FILE_H264

#ifdef USE_DECODER
			int decwidth = width;
			int decheight = height;
			int stride[2] = { 0,0 };
			if((res = decoder->Decode(h264buf, h264Size, decbuf, stride, decwidth, decheight)) == 0){
				std::cout << "decode ok" << std::endl;
#ifdef DUMP_FILE_DEC_YUV
				if (ofsdecyuv.is_open() && yuvSize > 0 
					&& decbuf[0] != NULL 
					&& decbuf[1] != NULL 
					&& decbuf[2] != NULL && decwidth > 0 && decheight > 0)
				{	
#ifdef USE_OPENCV
					//yuv
					cv::Mat yuvimg(decheight * 1.5, decwidth, CV_8UC1);
					int framelen = 0;
#endif //USE_OPENCV
					// ofsdecyuv.write((char*)decbuf[0], decheight * decwidth);
					// ofsdecyuv.write((char*)decbuf[1], (decwidth * decheight +1)>>2);
					// ofsdecyuv.write((char*)decbuf[2], (decwidth * decheight +1)>>2);
						int   i;
						unsigned char*  pPtr = NULL;

						pPtr = decbuf[0];
						for (i = 0; i < decheight; i++) {
							ofsdecyuv.write((char*)pPtr, decwidth);
							#ifdef USE_OPENCV
							memcpy(yuvimg.data + framelen, pPtr, decwidth);
							framelen += decwidth;
							#endif
							pPtr += stride[0];
						}

						decheight = decheight / 2;
						decwidth = decwidth / 2;
						pPtr = decbuf[1];
						for (i = 0; i < decheight; i++) {
							ofsdecyuv.write((char*)pPtr, decwidth);
							#ifdef USE_OPENCV
							memcpy(yuvimg.data + framelen, pPtr, decwidth);
							framelen += decwidth;
							#endif
							pPtr += stride[1];
						}

						pPtr = decbuf[2];
						for (i = 0; i < decheight; i++) {
							ofsdecyuv.write((char*)pPtr, decwidth);
							#ifdef USE_OPENCV
							memcpy(yuvimg.data + framelen, pPtr, decwidth);
							framelen += decwidth;
							#endif
							pPtr += stride[1];
						}
					ofsdecyuv.flush();

#ifdef USE_OPENCV
					cv::cvtColor(yuvimg, mat, CV_YUV420p2RGBA);
					// putText fps
					cv::putText(mat,
					fpstr.c_str(),
					cv::Point(mat.cols-200, 100),
					cv::FONT_HERSHEY_PLAIN,
					2,
					cv::Scalar(255,125,125, 255), 2, 8, false);

					imshow("img", mat);
					waitKey(1);
#endif //USE_OPENCV
				}
#elif defined(USE_OPENCV) //DUMP_FILE_DEC_YUV
				if(decbuf[0] != NULL 
					&& decbuf[1] != NULL 
					&& decbuf[2] != NULL && decwidth > 0 && decheight > 0)
				{	
					cv::Mat yuvimg(decheight * 1.5, decwidth, CV_8UC1);
					int framelen = 0;
					int   i;
						unsigned char*  pPtr = NULL;

						pPtr = decbuf[0];
						for (i = 0; i < decheight; i++) {
							memcpy(yuvimg.data + framelen, pPtr, decwidth);
							framelen += decwidth;
							pPtr += stride[0];
						}

						decheight = decheight / 2;
						decwidth = decwidth / 2;
						pPtr = decbuf[1];
						for (i = 0; i < decheight; i++) {
							memcpy(yuvimg.data + framelen, pPtr, decwidth);
							framelen += decwidth;
							pPtr += stride[1];
						}

						pPtr = decbuf[2];
						for (i = 0; i < decheight; i++) {
							memcpy(yuvimg.data + framelen, pPtr, decwidth);
							framelen += decwidth;
							pPtr += stride[1];
						}
					cv::cvtColor(yuvimg, mat, CV_YUV420p2RGBA);
					// putText fps
					cv::putText(mat,
					fpstr.c_str(),
					cv::Point(mat.cols-200, 100),
					cv::FONT_HERSHEY_PLAIN,
					2,
					cv::Scalar(255,125,125, 255), 2, 8, false);

					imshow("img", mat);
					waitKey(1);
				}
#endif //DUMP_FILE_DEC_YUV
			}else {
				std::cout << "decode failed" << res << std::endl;
			}
#endif //USE_DECODER

		}else std::cout << "encode failed:" << res << std::endl;
		}else{
			std::cout << "yuv convert failed:" << res << std::endl;
		}

		// std::cout << fpstr.c_str() << std::endl;
	}

#ifdef DUMP_FILE_YUV
if(ofsyuv.is_open())
	ofsyuv.close();
#endif //DUMP_FILE_YUV

#if defined(DUMP_FILE_DEC_YUV) && defined(USE_DECODER)
if(ofsdecyuv.is_open())
	ofsdecyuv.close();
#endif //DUMP_FILE_DEC_YUV

#ifdef DUMP_FILE_H264
if(ofsh264.is_open())
	ofsh264.close();
#endif //DUMP_FILE_H264

	delete encoder;
#ifdef USE_DECODER
	delete decoder;
#endif //USE_DECODER
}

#else
int main(int argc, char* argv[]){
	if(argc < 2){
		printf("please input a yuv file like:%s a.yuv\n", argv[0]);
		return -1;
	}

	//init h264mgr
	auto encoder = sshot::createH264Mgr();
	encoder->Init();

	ofstream ofsh264;
	ofsh264.open("p.264", ios::binary);
	int bufsize = 1080*1920*3>>1;
	uint8_t* yuvdata = (uint8_t*)malloc(1080*1920*3>>1);
	uint8_t* h264buf = (uint8_t*)malloc(1024*1024);
	int h264Size = 0;

	FILE* fpYuv = fopen(argv[1], "rb");
	if(fpYuv){
		while(!feof(fpYuv))
		{
			size_t st = fread(yuvdata, 1, bufsize, fpYuv);
			if(st >= bufsize){
				if(encoder->Encode(yuvdata, h264buf, &h264Size) == 0)
				{
					std::cout << "encoder ok: " << h264Size << " bytes" << std::endl;
					if (ofsh264.is_open() && h264Size > 0)
					{
						ofsh264.write((char*)h264buf, h264Size);
						ofsh264.flush();
					}
				}else std::cout << "encode failed" << std::endl;
			}
		}
	}
	if(ofsh264.is_open())
	ofsh264.close();

	if(fpYuv){
		fclose(fpYuv);
		fpYuv = NULL;
	}
}

#endif