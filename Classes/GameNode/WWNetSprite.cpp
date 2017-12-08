//
//  NetSprite.cpp
//  ...
//
//  Created by Sam on 14-6-24.
//
//

#include "WWNetSprite.h"

USING_NS_CC;
#include "md5.h"
using namespace network;

WWNetSprite::WWNetSprite():
	m_pRequest(nullptr)
{
	
}

WWNetSprite* WWNetSprite::createWithUrl(const char* url, const char* defaultPic, cocos2d::Size limitSize)
{
    WWNetSprite *pSprite = new WWNetSprite();
    
	if (defaultPic != nullptr && strcmp(defaultPic, "") != 0)
	{
		if (pSprite && pSprite->initWithFile(defaultPic))
		{
			pSprite->initWithUrl(url, limitSize);
			pSprite->autorelease();
			return pSprite;
		}
	}
	else
	{
		if (pSprite && pSprite->init()) 
		{
			pSprite->initWithUrl(url, limitSize);
			pSprite->autorelease();
			return pSprite;
		}
	}
	
    CC_SAFE_DELETE(pSprite);
    return NULL;
}

void WWNetSprite::initWithUrl(const char* url, cocos2d::Size limitSize)
{
	if (url == nullptr || strcmp(url, "") == 0)
	{
		CCLOG("WWNetSprite::initWithUrl url is empty");
		return;
	}
	
	//判断是否已经获取过此url
	std::string strUrl = url;
	MD5 md5(strUrl);
	//iMD5.GenerateMD5((unsigned char *)strUrl.c_str(), strUrl.length());
	std::string strMd5 = "NET_SPRITE";
	//strMd5 += iMD5.ToString();
    strMd5 = md5.md5();
	CCLOG("WWNetSprite::initWithUrl url Md5 TextureKey = %s", strMd5.c_str());
	Texture2D *texture = Director::getInstance()->getTextureCache()->getTextureForKey(strMd5);
	if (texture != nullptr)
	{
		setTexture(texture);
		return;
	}
    setContentSize(limitSize);
    m_pRequest = new HttpRequest();
	m_pRequest:retain();
    m_pRequest->setUrl(url);
    m_pRequest->setRequestType(HttpRequest::Type::GET);
    m_pRequest->setResponseCallback( CC_CALLBACK_2(WWNetSprite::onHttpRequestCompleted, this, limitSize));
    HttpClient::getInstance()->send(m_pRequest);
	CCLOG("WWNetSprite::initWithUrl request url = %s", url);
}

void WWNetSprite::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response, cocos2d::Size limitSize)
{
    if (!response) 
	{
        return;
    }
    
    if (!response->isSucceed()) 
	{
        log("NetSprite response failed");
        return;
    }

    std::vector<char> *buffer = response->getResponseData();
    Image *img = new Image();
    img->autorelease();
    char *tmp = new char[buffer->size()];
    for (int i = 0;  i < buffer->size(); ++ i) 
	{
    	tmp[i] = buffer->at(i);
	}
    if (img->initWithImageData((unsigned char *)tmp, buffer->size())) 
	{
		std::string strUrl = m_pRequest->getUrl();
		if (!strUrl.empty())
		{
            MD5 md5(strUrl);
            //iMD5.GenerateMD5((unsigned char *)strUrl.c_str(), strUrl.length());
            std::string strMd5 = "NET_SPRITE";
            //strMd5 += iMD5.ToString();
            strMd5 = md5.md5();
			Director::getInstance()->getTextureCache()->addImage(img, strMd5);
			Texture2D *texture = Director::getInstance()->getTextureCache()->getTextureForKey(strMd5);
			setTexture(texture);
            setContentSize(limitSize);
		}
		
		/*Texture2D *texture = new Texture2D();
		texture->autorelease();
		if (texture->initWithImage(img))
		{
		setTexture(texture);
		}*/
    }

	if( m_pRequest )
	{
		m_pRequest->setResponseCallback( nullptr );
		m_pRequest->release();
		m_pRequest = nullptr;
	}
    delete []tmp;
}

void WWNetSprite::onEnter()
{
    Sprite::onEnter();
}

void WWNetSprite::onExit()
{
    Sprite::onExit();
	if( m_pRequest)
	{
		m_pRequest->setResponseCallback( nullptr );
		m_pRequest->release();
		m_pRequest = nullptr;
	}
}
