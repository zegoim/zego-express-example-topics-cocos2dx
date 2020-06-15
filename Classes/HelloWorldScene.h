/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ZegoEventHandler.hpp"
#include "ui/CocosGUI.h"

#include <memory>

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
private:
    void onCapturedVideoFrameRawData(const unsigned char* data, unsigned int dataLength, ZegoVideoFrameParam param, ZegoVideoFlipMode flipMode);
    void onRemoteVideoFrameRawData(const unsigned char* data, unsigned int dataLength, ZegoVideoFrameParam param, const std::string& streamID);
    
private:

    std::shared_ptr<ZegoEventHandler> m_handler;

    cocos2d::Label* m_version_label;
    
    cocos2d::Sprite* m_previewView = nullptr;
    cocos2d::Texture2D* m_preview_tex = nullptr;
    
    unsigned char* m_preview_frame = nullptr;
    unsigned int m_preview_frame_len = 0;
    
    cocos2d::Sprite* m_playerView = nullptr;
    cocos2d::Texture2D* m_player_tex = nullptr;
    
    unsigned char* m_player_frame = nullptr;
    unsigned int m_player_frame_len = 0;
};

#endif // __HELLOWORLD_SCENE_H__
