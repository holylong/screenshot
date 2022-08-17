#ifndef _H264_MGR_H_
#define _H264_MGR_H_

#include <stdint.h>

#if 0
#ifdef _WIN32 
#define DLL_EXPORTS __declspec(dllexport)
#define DLL_IMPORTS __declspec(dllimport)
#else
#define DLL_EXPORTS
#define DLL_IMPORTS
#endif
#endif

namespace sshot{
  enum MgrType{
    Mgr_None,
    Mgr_Decode,
    Mgr_Encode
  };

  class EncodeParameter {
		
	};

  class DecodeParameter{

  };

  typedef struct _yuv420frame
  {
    unsigned char* y;
	  unsigned char* u;
	  unsigned char* v;
  }yuv420frame, *pNodeYuv420;

  class H264Mgr{
    public:
      H264Mgr() = default;

      virtual ~H264Mgr() = default;
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
      virtual int Init(int width, int height,float in_fps  = 25.0f, float out_fps  = 25.0f, int type = 0) = 0;
      virtual int Decode(unsigned char* h264buf, unsigned int h264len, uint8_t** dstbuf,  int* stride, int& width, int& height) = 0;
#if 0
      virtual int Encode(yuv420frame yuv, int nLen) = 0;
#endif
      // virtual int Encode(uint8_t* yuvdata, uint8_t** h264buf, int *outlen){
      virtual int Encode(uint8_t* yuvdata, uint8_t* h264buf, int *outlen){
        return 0;
      }

protected:
      int _width{1920};
			int _height{1080};
  };

  H264Mgr* createH264Mgr(MgrType type = Mgr_Encode);
}

#endif //_H264_MGR_H_