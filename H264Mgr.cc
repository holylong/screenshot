#include <H264Mgr.hpp>
#include <H264Decoder.hpp>
#include <H264Encoder.hpp>

namespace sshot{
    H264Mgr* createH264Mgr(MgrType type)
    {
      if(type == Mgr_Decode){
        return reinterpret_cast<H264Mgr*>(new H264Decoder());
      }else if(type == Mgr_Encode){
        return reinterpret_cast<H264Mgr*>(new H264Encoder());
      }
      return nullptr;
    }
}