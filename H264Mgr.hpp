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

      virtual int Init() = 0;
      virtual int Decode(unsigned char* h264buf, unsigned int h264len) = 0;
#if 0
      virtual int Encode(yuv420frame yuv, int nLen) = 0;
#endif
      // virtual int Encode(uint8_t* yuvdata, uint8_t** h264buf, int *outlen){
      virtual int Encode(uint8_t* yuvdata, uint8_t* h264buf, int *outlen){
        return 0;
      }
  };

  class H264Decoder;
  class H264Encoder;

  H264Mgr* createH264Mgr(MgrType type = Mgr_Encode);
}

#endif //_H264_MGR_H_