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

#include "HelloWorldScene.h"

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//#include <libyuv.h>
//#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#include "ZegoExpressSDK.h"
#include "platform/android/jni/JniHelper.h"
#endif
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,"test",__VA_ARGS__)

USING_NS_CC;
using namespace cocos2d::ui;
using namespace ZEGO::EXPRESS;
//Go to ZEGO Management Console and apply for appID and appSign
unsigned int APPID = 123456;
std::string APPSIGN = "xxxxxxx";

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    // 开启自定义渲染，SDK将抛出原始视频数据帧
    ZegoEngineConfig config;
    ZegoCustomVideoRenderConfig rconfig;
    rconfig.bufferType = ZEGO_VIDEO_BUFFER_TYPE_RAW_DATA;
    rconfig.frameFormatSeries = ZEGO_VIDEO_FRAME_FORMAT_SERIES_RGB;
    rconfig.enableEngineRender = false;
    
    config.customVideoRenderConfig = &rconfig;
    ZegoExpressSDK::setEngineConfig(config);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    m_version_label = Label::createWithTTF(ZegoExpressSDK::getVersion(), "fonts/Marker Felt.ttf", 24);
    if (m_version_label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        m_version_label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - m_version_label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(m_version_label, 1);
    }

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");
    if (sprite == nullptr)
    {
        problemLoading("'HelloWorld.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
    }
    
    auto createEngineButton = Button::create();
    createEngineButton->setTitleText("初始化");
    createEngineButton->addTouchEventListener([=](Ref* ref, Widget::TouchEventType type) {
        
        if(type != ui::Widget::TouchEventType::ENDED)
            return;
        if(createEngineButton->getTitleText() == "初始化") {

            if(!m_handler)
            {
                m_handler = std::make_shared<ZegoEventHandler>();
            }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            LOGD("Android JVM: %p. activity context: %p.", JniHelper::getJavaVM(), JniHelper::getActivity());
            ZegoExpressSDK::setAndroidEnv(JniHelper::getJavaVM(), JniHelper::getActivity());

#endif

            ZegoExpressSDK::createEngine(APPID, APPSIGN, true, ZEGO_SCENARIO_COMMUNICATION, m_handler);
            createEngineButton->setTitleText("反初始化");
        } else {
            auto engine = ZegoExpressSDK::getEngine();
            ZegoExpressSDK::destroyEngine(engine, nullptr);
            createEngineButton->setTitleText("初始化");
        }

    });
    
    createEngineButton->setPosition(Vec2(visibleSize.width - 100, visibleSize.height/2 + origin.y + 30));
    this->addChild(createEngineButton, 1);
    
    auto loginEngineButton = Button::create();
    loginEngineButton->setTitleText("登录房间");
    loginEngineButton->addTouchEventListener([=](Ref* ref, Widget::TouchEventType type) {
        
        if(type != ui::Widget::TouchEventType::ENDED)
            return;

        auto engine = ZegoExpressSDK::getEngine();
        if(loginEngineButton->getTitleText() == "登录房间") {
            ZegoUser user("lizhanpeng-cocos", "lizhanpeng-cocos");
            engine->loginRoom("cocos-room-777", user);

            loginEngineButton->setTitleText("登出房间");

        } else {
            engine->logoutRoom("cocos-room-777");

            loginEngineButton->setTitleText("登录房间");
        }
    });
    
    loginEngineButton->setPosition(Vec2(visibleSize.width - 100, visibleSize.height/2 + origin.y));
    this->addChild(loginEngineButton, 1);
    
    auto previewButton = Button::create();
    previewButton->setTitleText("预览推流");
    previewButton->addTouchEventListener([=](Ref* ref, Widget::TouchEventType type){
            
        if(type != ui::Widget::TouchEventType::ENDED)
            return;

        auto engine = ZegoExpressSDK::getEngine();
        if(previewButton->getTitleText() == "预览推流") {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            engine->enableHardwareEncoder(true);
        engine->enableHardwareDecoder(true);
#endif
            m_handler->setPreviewDataCallback(std::bind(&HelloWorld::onCapturedVideoFrameRawData, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
            m_handler->setPlayerDataCallback(std::bind(&HelloWorld::onRemoteVideoFrameRawData, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
            engine->setCustomVideoRenderHandler(m_handler);

            ZegoVideoConfig vconfig;

            engine->setVideoConfig(vconfig);
            engine->setAppOrientation(ZEGO_ORIENTATION_90);

            engine->startPreview(nullptr);
            engine->startPublishingStream("cocos-stream-777");

            previewButton->setTitleText("停止预览推流");
        } else {
            engine->stopPublishingStream();
            engine->stopPreview();
            engine->setCustomVideoCaptureHandler(nullptr);

            previewButton->setTitleText("预览推流");
        }

    });
    
    previewButton->setPosition(Vec2(visibleSize.width - 100, visibleSize.height/2 + origin.y - 30));
    this->addChild(previewButton, 1);
    
    auto playButton = Button::create();
    playButton->setTitleText("拉流播放");
    playButton->addTouchEventListener([=](Ref* ref, Widget::TouchEventType type){
            
        if(type != ui::Widget::TouchEventType::ENDED)
            return;

        auto engine = ZegoExpressSDK::getEngine();
        if(playButton->getTitleText() == "拉流播放") {

            engine->startPlayingStream("cocos-stream-777",nullptr);

            playButton->setTitleText("停止拉流");
        } else {

            engine->stopPlayingStream("cocos-stream-777");

            playButton->setTitleText("拉流播放");
        }

    });
    
    playButton->setPosition(Vec2(visibleSize.width - 100, visibleSize.height/2 + origin.y - 60));
    this->addChild(playButton, 1);
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
PixelFormat getPixelFormat(ZegoVideoFrameParam param){
    PixelFormat result =PixelFormat::BGRA8888;
    if(param.format == ZEGO_VIDEO_FRAME_FORMAT_BGRA32||param.format==ZEGO_VIDEO_FRAME_FORMAT_ARGB32){
        result=PixelFormat::BGRA8888;
    }else if (param.format==ZEGO_VIDEO_FRAME_FORMAT_RGBA32||param.format==ZEGO_VIDEO_FRAME_FORMAT_ABGR32){
        result=PixelFormat::RGBA8888;
    }
    return result;
}
void HelloWorld::onCapturedVideoFrameRawData(const unsigned char* data, unsigned int dataLength, ZegoVideoFrameParam param, ZegoVideoFlipMode flipMode)
{
    if(dataLength != m_preview_frame_len)
    {
        if(m_preview_frame)
        {
            delete [] m_preview_frame;
        }
        
        m_preview_frame = new unsigned char[dataLength];
        m_preview_frame_len = dataLength;
    }
    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    libyuv::ABGRToARGB(data, param.strides[0], m_preview_frame, param.strides[0], param.width, param.height);
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    memcpy(m_preview_frame, data, dataLength);
//#endif
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([dataLength, param, this](){
        if(m_preview_tex == nullptr)
        {
            m_preview_tex = new Texture2D;
            m_preview_tex->initWithData(this->m_preview_frame, dataLength, getPixelFormat(param), param.width, param.height, cocos2d::Size(param.width, param.height));
            m_previewView = Sprite::createWithTexture(m_preview_tex);
            
            auto visibleSize = Director::getInstance()->getVisibleSize();
            Vec2 origin = Director::getInstance()->getVisibleOrigin();
            
            m_previewView->setPosition(Vec2(100, origin.y + visibleSize.height - m_version_label->getContentSize().height - 60));
            m_previewView->setContentSize(cocos2d::Size(160, 90));
            this->addChild(m_previewView, 1);
        }
        else
        {
            m_preview_tex->updateWithData(this->m_preview_frame, 0, 0, param.width, param.height);
        }
    });
}
void HelloWorld::onRemoteVideoFrameRawData(const unsigned char* data, unsigned int dataLength, ZegoVideoFrameParam param, const std::string& streamID)
{
    if(dataLength != m_player_frame_len)
    {
        if(m_player_frame)
        {
            delete [] m_player_frame;
        }
        
        m_player_frame = new unsigned char[dataLength];
        m_player_frame_len = dataLength;
    }
    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    libyuv::ABGRToARGB(data, param.strides[0], m_player_frame, param.strides[0], param.width, param.height);
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    memcpy(m_player_frame, data, dataLength);
//#endif

    Director::getInstance()->getScheduler()->performFunctionInCocosThread([dataLength, param, this](){
        if(m_player_tex == nullptr)
        {
            m_player_tex = new Texture2D;
            m_player_tex->initWithData(this->m_player_frame, dataLength,getPixelFormat(param), param.width, param.height, cocos2d::Size(param.width, param.height));
            m_playerView = Sprite::createWithTexture(m_player_tex);
            
            auto visibleSize = Director::getInstance()->getVisibleSize();
            Vec2 origin = Director::getInstance()->getVisibleOrigin();
            
            m_playerView->setPosition(Vec2(100, origin.y + visibleSize.height - m_version_label->getContentSize().height - 60 - m_previewView->getContentSize().height));
            m_playerView->setContentSize(cocos2d::Size(160, 90));
            this->addChild(m_playerView, 1);
        }
        else
        {
            m_player_tex->updateWithData(this->m_player_frame, 0, 0, param.width, param.height);
        }
    });
}



