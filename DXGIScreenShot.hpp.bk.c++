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
 * @file DXGIScreenShot.hpp
 * @author holylong (mrhlingchen@163.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _DXGI_SCREEN_SHOT_H_
#define _DXGI_SCREEN_SHOT_H_

#include <DXGI1_2.h>
#include <d3d11.h>
#include <atlbase.h>
#include <ShellScalingAPI.h>

namespace sshot{

// Driver types supported
const D3D_DRIVER_TYPE g_DriverTypes[] =
{
	D3D_DRIVER_TYPE_HARDWARE,
	D3D_DRIVER_TYPE_WARP,
	D3D_DRIVER_TYPE_REFERENCE,
};
const UINT g_NumDriverTypes = ARRAYSIZE(g_DriverTypes);

// Feature levels supported
const D3D_FEATURE_LEVEL g_FeatureLevels[] =
{
	D3D_FEATURE_LEVEL_11_0,
	D3D_FEATURE_LEVEL_10_1,
	D3D_FEATURE_LEVEL_10_0,
	D3D_FEATURE_LEVEL_9_1
};
const UINT g_NumFeatureLevels = ARRAYSIZE(g_FeatureLevels);

#define SAFE_RELEASE(obj) \
    if(obj != nullptr) obj->Release();

  class DXGIScreenShot{
    public:
    bool InitDXGI()
    {
        SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
      HRESULT hr;
      for(UINT i = 0; i < g_NumDriverTypes; i++){
        hr = D3D11CreateDevice(nullptr,
        g_DriverTypes[0],
        nullptr,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT,
        g_FeatureLevels,
        g_NumFeatureLevels,
        D3D11_SDK_VERSION,
        &_d3d11_device,
        &_d3d_feature_level,
        &_d3d11_device_context);
        if(SUCCEEDED(hr)){
          break;
        }
      }

      hr = _d3d11_device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&_dxgi_device));
      if(FAILED(hr))return false;
      hr = _dxgi_device->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&_dxgi_adapter));
      SAFE_RELEASE(_dxgi_device);
      if(FAILED(hr))return false;

      for(int i = 0;;i++){
        if(i >= 10){
          SAFE_RELEASE(_dxgi_adapter);
          return false;
        }
        hr = _dxgi_adapter->EnumOutputs(i, &_dxgi_output);
        if(SUCCEEDED(hr)){
          SAFE_RELEASE(_dxgi_adapter);
          _dxgi_output->GetDesc(&_dxgi_desc);
          break;
        }
      }

      hr = _dxgi_output->QueryInterface(__uuidof(IDXGIOutput1), reinterpret_cast<void**>(&_dxgi_output1));
      SAFE_RELEASE(_dxgi_output);
      if(FAILED(hr))return false;


      hr = _dxgi_output1->DuplicateOutput(_d3d11_device, &_dxgi_dup);
      SAFE_RELEASE(_dxgi_output1);
      if(FAILED(hr)){
        if(hr == DXGI_ERROR_NOT_CURRENTLY_AVAILABLE){

        }

        std::cout << "init failed" << std::endl;
        return false;
      }

      _dxgi_dup->GetDesc(&_dxgi_dup_desc);
      if(_dxgi_dup_desc.DesktopImageInSystemMemory){

      }else{

      }
      std::cout << "init ok" << std::endl;
      return true;
    }

    private:
      IDXGIDevice*  _dxgi_device{nullptr};
      IDXGIAdapter* _dxgi_adapter{nullptr};
      IDXGIOutput*  _dxgi_output{nullptr};
      IDXGIOutput1* _dxgi_output1{nullptr};
      // IDXGIOutputDuplication* _duplicated_output{nullptr};

      D3D_FEATURE_LEVEL       _d3d_feature_level{D3D_FEATURE_LEVEL_9_1};
	    ID3D11Device*           _d3d11_device{nullptr};
	    ID3D11DeviceContext*    _d3d11_device_context{nullptr};
      IDXGIOutputDuplication* _dxgi_dup{nullptr};

      DXGI_OUTPUT_DESC        _dxgi_desc;
      DXGI_OUTDUPL_DESC        _dxgi_dup_desc;

      // DxgiTextureMapping     
  };
}

#endif //_DXGI_SCREEN_SHOT_H_