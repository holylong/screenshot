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

#ifdef USE_OPENCV
#include <opencv2/opencv.hpp>
using namespace cv;
#endif

using namespace std;

#define DUMP_FILE_H264
// #define DUMP_FILE_YUV
#undef USE_OPENCV

#if 1

int main()
{
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
#endif

	//init h264mgr
	auto encoder = sshot::createH264Mgr();
	encoder->Init();


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
	uint8_t* h264buf = (uint8_t*)malloc(1024*1024);; 
#ifdef DUMP_FILE_YUV
	ofstream ofsyuv;
	ofsyuv.open("p.yuv", ios::binary);
#endif

#ifdef DUMP_FILE_H264
	ofstream ofsh264;
	ofsh264.open("p.264", ios::binary);
#endif

	while (count < 150)
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
#ifdef USE_OPENCV
		mat.data = pRgb;
		// putText fps
		cv::putText(mat,
		fpstr.c_str(),
		cv::Point(mat.cols-200, 100),
		cv::FONT_HERSHEY_PLAIN,
		2,
		cv::Scalar(255,125,125, 255), 2, 8, false);

		imshow("img", mat);
		waitKey(1);
#endif

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
#endif

		//yuv2h264
		uint8_t* bakyuv = (uint8_t*)malloc(yuvSize);
		memcpy(bakyuv, yuvdata, yuvSize);
		if((res = encoder->Encode(bakyuv, h264buf, &h264Size)) == 0)
		{
			std::cout << "encoder ok: " << h264Size << " bytes" << std::endl;
#ifdef DUMP_FILE_H264
			if (ofsh264.is_open() && h264Size > 0)
			{
				ofsh264.write((char*)h264buf, h264Size);
				ofsh264.flush();
			}
#endif
		}else std::cout << "encode failed:" << res << std::endl;
		free(bakyuv);
		}else{
			std::cout << "yuv convert failed:" << res << std::endl;
		}

		// std::cout << fpstr.c_str() << std::endl;
	}

#ifdef DUMP_FILE_YUV
if(ofsyuv.is_open())
	ofsyuv.close();
#endif

#ifdef DUMP_FILE_H264
if(ofsh264.is_open())
	ofsh264.close();
#endif

	delete encoder;
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