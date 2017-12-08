//
//  WebSprite.h
//  ClashOfClouds
//
//
//

#ifndef __ClashOfClouds__WebSprite__
#define __ClashOfClouds__WebSprite__

#include <string>

#include "cocos2d.h"
#include "network/HttpClient.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#include "HttpFileDownload.h"
#else
#include "Http/HttpFileDownload.h"
#endif


class WebSprite : public cocos2d::Node , public HttpListener {
    
private:
    cocos2d::Sprite*	_content;
    std::string			_key;
    std::string			_url;
    std::string			_path;
	cocos2d::Size		_contentSize;
    
	int					_downloadKey;
    bool                _isDownNow;
    std::function<void(void)> _downCompleteCallBack;
    //Node*               _parentNode;
    

public:

	WebSprite();

	~WebSprite();

	static WebSprite *create(const std::string & url, cocos2d::Size limitSize,bool isDown = true);

    bool initWithUrl(const std::string & url, cocos2d::Size limitSize,bool isDown);
    std::string onPathConvertFileName(std::string  path);

	void refresh(const std::string & url);
    
    //void setParentNode(Node* parent) { _parentNode = parent;};

	virtual void onExit() override;

	virtual void onHttpDownloaded(int index, std::vector<char> *pBuffer) override;
    
    void startUpdate(std::function<void(void)> callBack);

private:
    void load();
    

	void befitSize();
    
    
};

#endif /* defined(__ClashOfClouds__WebSprite__) */
