//
//  WebSprite.cpp


#include "WebSprite.h"
#include "md5.h"

USING_NS_CC;

WebSprite::WebSprite() :
	_content(NULL),
	_downloadKey(-1)
{
}

WebSprite::~WebSprite()
{
}

void WebSprite::onExit()
{
	if (_downloadKey != -1)
	{
		HttpFileDownload::getInstance()->removeLisener(_downloadKey);
	}

	Node::onExit();
}



WebSprite * WebSprite::create(const std::string & url, cocos2d::Size limitSize,bool isDown)
{
	WebSprite* pRet = new (std::nothrow) WebSprite();
	if (pRet && pRet->initWithUrl(url, limitSize ,isDown))
	{
		pRet->autorelease();

		return pRet;
	}

	delete	pRet;
	pRet = nullptr;
	return nullptr;
}


bool WebSprite::initWithUrl(const std::string & url, cocos2d::Size limitSize ,bool isDown){
    USING_NS_CC;
    
    if(!Node::init()) return false;
   
    _isDownNow = isDown;
    _downCompleteCallBack = nullptr;
	setContentSize(limitSize);
	_contentSize = limitSize;

	refresh(url);

    return true;
}


void WebSprite::refresh(const std::string & url)
{
	_url = url;
	auto textureCache = Director::getInstance()->getTextureCache();
	if (_content) {
		if (_url.empty() || _url == "") {
			_content->setTexture("MainLayer/default_icon.png");
			befitSize();
			return;
		}
	}
	else {
		_content = Sprite::create("MainLayer/default_icon.png");
		addChild(_content);
		befitSize();
		if (_url.empty() || _url == "") {
			return;
		}
	}

	MD5 md5(_url);
	_key = md5.md5();
	auto fileUtils = FileUtils::getInstance();

	//_path = fileUtils->getWritablePath() + "websprite_" + onPathConvertFileName(_url) + ".jpg";

    _path = fileUtils->getWritablePath() + "websprite_" + _key + ".jpg";
    
	if (fileUtils->isFileExist(_path))
	{
		_content->setTexture(_path);
        if (_downCompleteCallBack != nullptr)
        {
            _downCompleteCallBack();
            _downCompleteCallBack = nullptr;
        }
    }
	else
	{
		_content->setTexture("MainLayer/default_icon.png");
		load();
	}
	befitSize();
}


std::string WebSprite::onPathConvertFileName(std::string path) {
    std::string ConvertStr = "externalImage" ;
    ConvertStr = ConvertStr + path;
    std::replace(ConvertStr.begin(), ConvertStr.end(), '\\', '#');
    std::replace(ConvertStr.begin(), ConvertStr.end(), ':', '#');
    std::replace(ConvertStr.begin(), ConvertStr.end(), '/', '#');
    std::replace(ConvertStr.begin(), ConvertStr.end(), '>', '#');
    std::replace(ConvertStr.begin(), ConvertStr.end(), '<', '#');
    std::replace(ConvertStr.begin(), ConvertStr.end(), '|', '#');
    std::replace(ConvertStr.begin(), ConvertStr.end(), '\"', '#');
    std::replace(ConvertStr.begin(), ConvertStr.end(), '?', '#');
    std::replace(ConvertStr.begin(), ConvertStr.end(), '.', '#');
    return ConvertStr ;
}

void WebSprite::onHttpDownloaded(int index, std::vector<char> *pBuffer)
{
	USING_NS_CC;
	using namespace network;

	auto textureCache = Director::getInstance()->getTextureCache();
    std::string getData;
    getData.assign(pBuffer->begin(), pBuffer->end());
    
	FileUtils::getInstance()->writeStringToFile(getData, _path);
	if (FileUtils::getInstance()->isFileExist(_path)) {
		if (_content) {
			_content->setTexture(_path);
		} else {
			_content = Sprite::create(_path);
			addChild(_content);
		}
		befitSize();
		_downloadKey = -1;
		if (_downCompleteCallBack != nullptr) {
			_downCompleteCallBack();
			_downCompleteCallBack = nullptr;
		}
	}
    
	//std::shared_ptr<Image> image(new Image);
	//image->initWithImageData((unsigned char*)getData.c_str(), getData.length());
 //   textureCache->addImage(image.get(), _url);
 //   Texture2D::PixelFormat format = image->getRenderFormat();
 //   if (format != Texture2D::PixelFormat::I8)
 //   {
 //       image->saveToFile(_path);
 //   }
}


void WebSprite::load() {
    USING_NS_CC;
    using namespace network;
    
    if (_isDownNow )
    {
        _downloadKey = HttpFileDownload::getInstance()->download(_url, this);
    }
}

void WebSprite::startUpdate(std::function<void(void)> callBack)
{
	auto textureCache = Director::getInstance()->getTextureCache();
    _downCompleteCallBack = callBack;
    if (_content) {
		if (_url.empty() || _url == "") {
			_content->setTexture(textureCache->addImage("MainLayer/default_icon.png"));
			befitSize();
			return;
		}
	} else {
		_content = Sprite::create("MainLayer/default_icon.png");
		addChild(_content);
		befitSize();
		if (_url.empty() || _url == "") {
			return;
		}
	}
    
    MD5 md5(_url);
    _key = md5.md5();
    auto fileUtils = FileUtils::getInstance();
    _path = fileUtils->getWritablePath() + "websprite_" + _key + ".jpg";
    if (fileUtils->isFileExist(_path))
    {
		_content->setTexture(textureCache->addImage(_path));
        if (_downCompleteCallBack != nullptr)
        {
            _downCompleteCallBack();
            _downCompleteCallBack = nullptr;
        }
    }
    else
    {
		_content->setTexture(textureCache->addImage("MainLayer/default_icon.png"));
        _downloadKey = HttpFileDownload::getInstance()->download(_url, this);
    }
	befitSize();
}

void WebSprite::befitSize()
{
	if (_content == NULL)
	{
		return;
	}

	float scaleX = (_contentSize.width - 4) / _content->getContentSize().width;
	float scaleY = (_contentSize.height - 4) / _content->getContentSize().height;

	_content->setScaleX(scaleX);
	_content->setScaleY(scaleY);
}
