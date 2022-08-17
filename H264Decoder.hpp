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
 * @file H264Decoder.hpp
 * @author holylong (mrhlingchen@163.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _H264_DECODER_H_
#define _H264_DECODER_H_

#include <H264Mgr.hpp>
#include <iostream>
#include <codec_app_def.h>
#include <codec_api.h>

namespace sshot{
  class H264Decoder : public H264Mgr{
    public:
  /**
   * @brief init codec
   * 
   * @param width 
   * @param height 
   * @param in_fps 
   * @param out_fps 
   * @param type 
   * @return int 
   */
    		int Init(int width, int height,float in_fps = 25.0f, float out_fps = 25.0f, int type = 0){
          _width = width;
          _height = height;

          int ret = 0;
          if((ret = WelsCreateDecoder(&_decoder)) < 0) return ret;

          memset(&_decParam, '\0', sizeof(SDecodingParam));
          _decParam.uiTargetDqLayer = UCHAR_MAX;
          _decParam.eEcActiveIdc = ERROR_CON_FRAME_COPY_CROSS_IDR;
          _decParam.sVideoProperty.size = sizeof(_decParam.sVideoProperty);
          _decParam.sVideoProperty.eVideoBsType = VIDEO_BITSTREAM_DEFAULT;
          
          if((ret = _decoder->Initialize(&_decParam)) < 0) return ret;
          return ret;
        }
#if 0
        int Encode(yuv420frame yuv, int nLen)
        {
          return 0;
        }
#endif
        int Decode(unsigned char* h264buf, unsigned int h264len, uint8_t** dstbuf,  int* stride, int& width, int& height)
        {
          DECODING_STATE ret = dsErrorFree;
          // int stride[2] = { _width, _height>>1};
          //return _decoder->DecodeFrame(h264buf, h264len, dstbuf, stride, width, height);
            SBufferInfo sDstBufInfo;
            memset(&sDstBufInfo, 0, sizeof(SBufferInfo));
            sDstBufInfo.uiInBsTimeStamp = uiTimeStamp++;
            //ret = _decoder->DecodeFrame2(h264buf, h264len, dstbuf, &sDstBufInfo);
            ret = _decoder->DecodeFrameNoDelay(h264buf, h264len, dstbuf, &sDstBufInfo);
            if(sDstBufInfo.iBufferStatus == 1){
                dstbuf[0] = sDstBufInfo.pDst[0];
                dstbuf[1] = sDstBufInfo.pDst[1];
                dstbuf[2] = sDstBufInfo.pDst[2];
                stride[0] = sDstBufInfo.UsrData.sSystemBuffer.iStride[0];
                stride[1] = sDstBufInfo.UsrData.sSystemBuffer.iStride[1];
                width = sDstBufInfo.UsrData.sSystemBuffer.iWidth;
                height = sDstBufInfo.UsrData.sSystemBuffer.iHeight;
            }
            return ret;
        }

        virtual ~H264Decoder(){
          if (_decoder) {
            _decoder->Uninitialize();
            WelsDestroyDecoder (_decoder);
          }
        }

      private:
        ISVCDecoder* _decoder{nullptr};
        SDecodingParam _decParam;
        uint64_t uiTimeStamp{ 0 };
  };
}

#endif //_H264_DECODER_H_