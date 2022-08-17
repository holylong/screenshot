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
    		int Init(int width, int height,float in_fps, float out_fps, int type){
          return 0;
        }
#if 0
        int Encode(yuv420frame yuv, int nLen)
        {
          return 0;
        }
#endif
        int Decode(unsigned char* h264buf, unsigned int h264len)
        {
          return 0;
        }
  };
}

#endif //_H264_DECODER_H_