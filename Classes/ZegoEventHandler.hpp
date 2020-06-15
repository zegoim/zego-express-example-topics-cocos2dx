//
//  ZegoEventHandler.hpp
//  TestExpress-mobile
//
//  Created by 李展鹏@ZEGO on 2020/4/9.
//

#ifndef ZegoEventHandler_hpp
#define ZegoEventHandler_hpp

#include <functional>
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "ZegoExpressSDK.h"
#else
#include <ZegoExpressSDK.h>
#endif

using namespace ZEGO;
using namespace EXPRESS;

using OnPreviewRGBDataCallback = std::function<void(const unsigned char* data, unsigned int dataLen, ZegoVideoFrameParam param, ZegoVideoFlipMode flipMode)>;
using OnPlayerRGBDataCallback = std::function<void(const unsigned char* data, unsigned int dataLen, ZegoVideoFrameParam param, const std::string& streamID)>;

class ZegoEventHandler : public IZegoEventHandler, public IZegoCustomVideoRenderHandler
{
public:
    void setPreviewDataCallback(OnPreviewRGBDataCallback callback);
    void setPlayerDataCallback(OnPlayerRGBDataCallback callback);
    
protected:
    virtual void onDebugError(int errorCode, const std::string& funcName, const std::string& info) override;
    virtual void onRoomStateUpdate(const std::string& roomID, ZegoRoomState state, int errorCode, const std::string& extendedData) override;
    virtual void onRoomStreamUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoStream>& streamList) override;
    virtual void onPublisherStateUpdate(const std::string& streamID, ZegoPublisherState state, int errorCode, const std::string& extendedData) override;
    virtual void onPublisherQualityUpdate(const std::string& streamID, const ZegoPublishStreamQuality& quality) override;
    virtual void onPlayerStateUpdate(const std::string& streamID, ZegoPlayerState state, int errorCode, const std::string& extendedData) override;
    virtual void onPlayerQualityUpdate(const std::string& streamID, const ZegoPlayStreamQuality& quality) override;
    
protected:
    virtual void onCapturedVideoFrameRawData(unsigned char** data, unsigned int* dataLength, ZegoVideoFrameParam param, ZegoVideoFlipMode flipMode, ZegoPublishChannel channel) override;
    virtual void onRemoteVideoFrameRawData(unsigned char** data, unsigned int* dataLength, ZegoVideoFrameParam param, const std::string& streamID) override;
    
private:
    OnPreviewRGBDataCallback m_preview_callback;
    OnPlayerRGBDataCallback m_player_callback;
};

#endif /* ZegoEventHandler_hpp */
