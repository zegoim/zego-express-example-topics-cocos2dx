//
//  ZegoEventHandler.cpp
//  TestExpress-mobile
//
//  Created by 李展鹏@ZEGO on 2020/4/9.
//

#include "ZegoEventHandler.hpp"

void ZegoEventHandler::setPreviewDataCallback(OnPreviewRGBDataCallback callback)
{
    m_preview_callback = callback;
}

void ZegoEventHandler::setPlayerDataCallback(OnPlayerRGBDataCallback callback)
{
    m_player_callback = callback;
}

void ZegoEventHandler::onDebugError(int errorCode, const std::string& funcName, const std::string& info)
{
    printf("[EXPRESS::onDebugError] error: %d, func name: %s, info: %s\n", errorCode, funcName.c_str(), info.c_str());
}

void ZegoEventHandler::onRoomStateUpdate(const std::string& roomID, ZegoRoomState state, int errorCode, const std::string& extendedData)
{
    printf("[EXPRESS::onRoomStateUpdate] state: %d, room id: %s, error: %d, extended data: %s\n", state, roomID.c_str(), errorCode, extendedData.c_str());
}

void ZegoEventHandler::onRoomStreamUpdate(const std::string& roomID, ZegoUpdateType updateType, const std::vector<ZegoStream>& streamList)
{
    
}
void ZegoEventHandler::onPublisherStateUpdate(const std::string& streamID, ZegoPublisherState state, int errorCode, const std::string& extendedData)
{
    
}

void ZegoEventHandler::onPublisherQualityUpdate(const std::string& streamID, const ZegoPublishStreamQuality& quality)
{
    printf("[EXPRESS::onPublisherQualityUpdate] hardware: %d\n", quality.isHardwareEncode);
}

void ZegoEventHandler::onPlayerStateUpdate(const std::string& streamID, ZegoPlayerState state, int errorCode, const std::string& extendedData)
{
    
}

void ZegoEventHandler::onPlayerQualityUpdate(const std::string& streamID, const ZegoPlayStreamQuality& quality)
{
    printf("[EXPRESS::onPublisherQualityUpdate] hardware: %d\n", quality.isHardwareDecode);
}

void ZegoEventHandler::onCapturedVideoFrameRawData(unsigned char** data, unsigned int* dataLength, ZegoVideoFrameParam param, ZegoVideoFlipMode flipMode, ZegoPublishChannel channel)
{
    if(m_preview_callback)
    {
        m_preview_callback(data[0], dataLength[0], param, flipMode);
    }
}

void ZegoEventHandler::onRemoteVideoFrameRawData(unsigned char** data, unsigned int* dataLength, ZegoVideoFrameParam param, const std::string& streamID)
{
    if(m_player_callback)
    {
        m_player_callback(data[0], dataLength[0], param, streamID);
    }
}

