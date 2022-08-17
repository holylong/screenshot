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
 * @file YuvConvertor.hpp
 * @author holylong (mrhlingchen@163.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _YUV_CONVERTOR_H_
#define _YUV_CONVERTOR_H_

#include <stdio.h>
#include <stdint.h>
#include <libyuv.h>

namespace sshot{
  class YuvConvertor{
      public:
    	static int ARGBToI420(const uint8_t* rgbptr, int dst_width, int dst_height, uint8_t** yuvptr)
      {
        int half_width = (dst_width + 1) / 2;
        int half_height = (dst_height + 1) / 2;
        return libyuv::ARGBToI420(
            rgbptr, dst_width * 4, *yuvptr, dst_width,
            *yuvptr + dst_width * dst_height, half_width,
            *yuvptr + dst_width * dst_height + half_width * half_height,
            half_width, dst_width, dst_height);
      }
  };
}

#endif //_YUV_CONVERTOR_H_