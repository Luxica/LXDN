//
//  NetSprite.h
//  ...
//
//  Created by Sam on 14-6-24.
//
// 加载网络图片类 NetSprite
//
// 接口 createWithUrl。入参一:图片url地址，入参二:默认图片(暂为图片文件，非缓存图片) 。返回图片精灵

#ifndef __testPng__WWNetSprite__
#define __testPng__WWNetSprite__

#include "cocos2d.h"
#include "network/HttpClient.h"

class WWNetSprite: public cocos2d::Sprite 
{
    
public:
	WWNetSprite();
    static WWNetSprite* createWithUrl( const char *url,  const char* defaultPic, cocos2d::Size limitSize);
    virtual void onEnter();
    virtual void onExit();

private:
    void initWithUrl( const char* url, cocos2d::Size limitSize);

    void onHttpRequestCompleted( cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response, cocos2d::Size limitSize);

private:
	cocos2d::network::HttpRequest *m_pRequest;
    cocos2d::Sprite*	_content;
};

#endif /* defined(__testPng__NetSprite__) */
