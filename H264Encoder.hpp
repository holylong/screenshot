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
 * @file H264Encoder.hpp
 * @author holylong (mrhlingchen@163.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <iostream>
#include <H264Mgr.hpp>
#include <codec_app_def.h>
#include <codec_api.h>
#include <macros.h>
#include <BaseUtil.hpp>

namespace sshot {
	static int debugLevel = 4;
	class H264Encoder : public H264Mgr{
	public:
		H264Encoder() {

		}

		int Init(){
			int res = WelsCreateSVCEncoder(&_encoder);
			if(res < 0)return res;

			memset(&_encParam, 0, sizeof(SEncParamExt));
			_encoder->GetDefaultParams(&_encParam);
			_encParam.iUsageType = CAMERA_VIDEO_REAL_TIME;
			_encParam.bSimulcastAVC         = 0;
			_encParam.iPicWidth = 1920;
			_encParam.iPicHeight = 1080;
			_encParam.fMaxFrameRate = 30.0f;
			_encParam.iMultipleThreadIdc = 1;
			_encParam.iTargetBitrate = 1500000000;
			_encParam.eSpsPpsIdStrategy = INCREASING_ID;
			_encParam.uiIntraPeriod = 0;
			_encParam.iTemporalLayerNum = 2;
			_encParam.bEnableFrameCroppingFlag = 1;
			_encParam.uiMaxNalSize = 0;
			_encParam.iLoopFilterDisableIdc = 0;
			_encParam.iLoopFilterAlphaC0Offset = 0;
			_encParam.iLoopFilterBetaOffset = 0;
			_encParam.bUseLoadBalancing = 1;
			// _encParam.iRCMode = RC_BITRATE_MODE;
			_encParam.iRCMode = RC_QUALITY_MODE;
			_encParam.iMaxBitrate = UNSPECIFIED_BIT_RATE;
			_encParam.bEnableFrameSkip = true;
			_encParam.iMaxQp = 51;
			_encParam.iMinQp = 0;
			_encParam.bEnableDenoise = false;
			_encParam.bEnableSceneChangeDetect = true;
			_encParam.bEnableBackgroundDetection = true;
			_encParam.bEnableAdaptiveQuant = true;
			_encParam.iLtrMarkPeriod = 30;
			_encParam.bPrefixNalAddingCtrl = false;
			_encParam.iSpatialLayerNum = 1;
			_encParam.bEnableLongTermReference = true;
			_encParam.bPrefixNalAddingCtrl = false;
			

			// _encParam.iComplexityMode = LOW_COMPLEXITY;			
			// _encParam.bFixRCOverShoot = true;
			// _encParam.iIdrBitrateRatio = 4 * 100;

			//svc zero layer
			int iIndexLayer = 0;
			// _encParam.sSpatialLayers[0].uiProfileIdc = PRO_BASELINE;
			_encParam.sSpatialLayers[iIndexLayer].iVideoWidth = _width;
			_encParam.sSpatialLayers[iIndexLayer].iVideoHeight = _height;
			_encParam.sSpatialLayers[iIndexLayer].fFrameRate = 30;
			_encParam.sSpatialLayers[iIndexLayer].iSpatialBitrate = 120000000;
			_encParam.sSpatialLayers[iIndexLayer].iDLayerQp = 24;
			_encParam.sSpatialLayers[iIndexLayer].uiProfileIdc = PRO_BASELINE;
			// _encParam.sSpatialLayers[0].iMaxSpatialBitrate = _encParam.iMaxBitrate;
			_encParam.sSpatialLayers[iIndexLayer].iMaxSpatialBitrate    = 0;
  		    _encParam.sSpatialLayers[iIndexLayer].sSliceArgument.uiSliceMode = SM_SINGLE_SLICE;
			_encParam.sSpatialLayers[iIndexLayer].sSliceArgument.uiSliceNum = 1;
			_encParam.sSpatialLayers[iIndexLayer].sSliceArgument.uiSliceMbNum[0] = 960;
			_encParam.sSpatialLayers[iIndexLayer].sSliceArgument.uiSliceSizeConstraint = 1500;

			  ++ iIndexLayer;
			_encParam.sSpatialLayers[iIndexLayer].uiProfileIdc       = PRO_SCALABLE_BASELINE;
			_encParam.sSpatialLayers[iIndexLayer].iVideoWidth        = 320;
			_encParam.sSpatialLayers[iIndexLayer].iVideoHeight       = 180;
			_encParam.sSpatialLayers[iIndexLayer].fFrameRate         = 15.0f;
			_encParam.sSpatialLayers[iIndexLayer].iSpatialBitrate    = 160000;
			_encParam.sSpatialLayers[iIndexLayer].iMaxSpatialBitrate    = UNSPECIFIED_BIT_RATE;
			_encParam.sSpatialLayers[iIndexLayer].sSliceArgument.uiSliceMode = SM_SINGLE_SLICE;

			++ iIndexLayer;
			_encParam.sSpatialLayers[iIndexLayer].uiProfileIdc       = PRO_SCALABLE_BASELINE;
			_encParam.sSpatialLayers[iIndexLayer].iVideoWidth        = 640;
			_encParam.sSpatialLayers[iIndexLayer].iVideoHeight       = 360;
			_encParam.sSpatialLayers[iIndexLayer].fFrameRate         = 30.0f;
			_encParam.sSpatialLayers[iIndexLayer].iSpatialBitrate    = 512000;
			_encParam.sSpatialLayers[iIndexLayer].iMaxSpatialBitrate    = UNSPECIFIED_BIT_RATE;
			_encParam.sSpatialLayers[iIndexLayer].sSliceArgument.uiSliceMode = SM_SINGLE_SLICE;
			_encParam.sSpatialLayers[iIndexLayer].sSliceArgument.uiSliceNum = 1;

			++ iIndexLayer;
			_encParam.sSpatialLayers[iIndexLayer].uiProfileIdc       = PRO_SCALABLE_BASELINE;
			_encParam.sSpatialLayers[iIndexLayer].iVideoWidth        = 1280;
			_encParam.sSpatialLayers[iIndexLayer].iVideoHeight       = 720;
			_encParam.sSpatialLayers[iIndexLayer].fFrameRate         = 30.0f;
			_encParam.sSpatialLayers[iIndexLayer].iSpatialBitrate    = 1500000;
			_encParam.sSpatialLayers[iIndexLayer].iMaxSpatialBitrate    = UNSPECIFIED_BIT_RATE;
			_encParam.sSpatialLayers[iIndexLayer].sSliceArgument.uiSliceMode = SM_SINGLE_SLICE;
			_encParam.sSpatialLayers[iIndexLayer].sSliceArgument.uiSliceNum = 1;

			_sSrcPic = new SSourcePicture;
			_sSrcPic->iColorFormat = videoFormatI420;
			_sSrcPic->uiTimeStamp = 0;
			_sSrcPic->iPicHeight = _height;
			_sSrcPic->iPicWidth = _width;
			_sSrcPic->iStride[0] = _sSrcPic->iPicWidth; 
			_sSrcPic->iStride[1] = _sSrcPic->iStride[2] = _sSrcPic->iStride[0] >> 1;
			_info = new SFrameBSInfo;

			int ret = _encoder->InitializeExt(&_encParam);
			if (ret < 0)
				return ret;
			_encoder->SetOption(ENCODER_OPTION_TRACE_LEVEL, &debugLevel);
			return 0;
		}
#if 0
		int Encode(yuv420frame yuv, int nLen)
		{
			int res = 0;
				// memset(&_info, 0, sizeof(SFrameBSInfo)); 
				SSourcePicture pic; 
				memset(&pic, 0, sizeof(SSourcePicture)); 
				pic.iPicWidth = _encParam.iPicWidth;
				pic.iPicHeight = _encParam.iPicHeight;
				pic.iColorFormat = videoFormatI420; 
				pic.uiTimeStamp = 0;
				pic.iStride[0] = pic.iPicWidth; 
				pic.iStride[1] = pic.iStride[2] = pic.iPicWidth >> 1; 
				pic.pData[0] = yuv.y;
				pic.pData[1] = yuv.u;
				pic.pData[2] = yuv.v;
				// res = _encoder->EncodeFrame(&pic, &_info);
				// _info.sLayerInfo[0].pBsBuf;
				if (res < 0)
					return res;

				return res;
		}
	#endif

		int Encode(uint8_t* yuvdata, uint8_t* h264buf, int *outlen){
			// std::cout << __FUNCTION__ << " enter" << std::endl;
			int bufsize = _width*_height*3>>1;
        int res = 0;
				_sSrcPic->pData[0] = yuvdata;
				_sSrcPic->pData[1] = (_sSrcPic->pData[0] + (_width*_height));
				_sSrcPic->pData[2] = (_sSrcPic->pData[1] + (_width*_height>>2));
				// _sSrcPic->uiTimeStamp = WELS_ROUND (_iFrameIdx * (1000 / _encParam.fMaxFrameRate));
				_sSrcPic->uiTimeStamp = sshot::SystemUtil::getCPUTime()*90LL;

				res = _encoder->EncodeFrame(_sSrcPic, _info);
				++_iFrameIdx;
				if (videoFrameTypeSkip == _info->eFrameType || videoFrameTypeInvalid == _info->eFrameType) {
					return _info->eFrameType;
				}
				if(res == cmResultSuccess){
					int iLayer = 0;
					int iFrameSize = 0;
					while (iLayer < _info->iLayerNum) {
						SLayerBSInfo* pLayerBsInfo = &_info->sLayerInfo[iLayer];
						if (pLayerBsInfo != NULL) {
							int iLayerSize = 0;
							 int iNalIdx = pLayerBsInfo->iNalCount - 1;
							 do {
							 	iLayerSize += pLayerBsInfo->pNalLengthInByte[iNalIdx];
							 	--iNalIdx;
							 } while (iNalIdx >= 0);
							memcpy(h264buf + iFrameSize, pLayerBsInfo->pBsBuf, iLayerSize);
							iFrameSize += iLayerSize;
					 }
					 ++ iLayer;
				  }

					*outlen = iFrameSize;
				}
				
				// std::cout << __FUNCTION__ << " exit" << std::endl;
				return res;
    }

		int Decode(unsigned char* h264buf, unsigned int h264len)
		{
			return 0;
		}

		bool InitEncoder()
		{
			return false;
		}

		virtual ~H264Encoder() {
			std::cout << __FUNCTIONW__ << std::endl;
			if (_encoder) {
				std::cout << __FUNCTIONW__ << " close encoder"<< std::endl;
    		WelsDestroySVCEncoder (_encoder);
			}
		}

		private:
			// std::shar?ed_ptr<ISVCEncoder> _encoder;
			ISVCEncoder    *_encoder;
			SEncParamExt   _encParam;
			SSourcePicture *_sSrcPic;
			SFrameBSInfo   *_info;
			// SFrameBSInfo   _info;

			int _width{1920};
			int _height{1080};
			float _fps{40.0f};
			int _gop{320};
			int32_t _iFrameIdx{0};
			int _bitrate{2500000};
	};
}