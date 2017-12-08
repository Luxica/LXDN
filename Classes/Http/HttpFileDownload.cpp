#include "HttpFileDownload.h"

HttpFileDownload::HttpFileDownload()
{
	_index = 1;
}

HttpFileDownload* HttpFileDownload::theInstance = NULL;

HttpFileDownload* HttpFileDownload::getInstance()
{
	if (theInstance == NULL)
	{
		theInstance = new HttpFileDownload;
	}

	return theInstance;
}


HttpFileDownload::~HttpFileDownload()
{

}


int HttpFileDownload::download(const std::string& url, HttpListener *pListener)
{
	USING_NS_CC;
	using namespace network;

	int _key = _index;
	_listenerMap[_key] = pListener;

	_index++;

    
	auto request = new HttpRequest();
	request->setRequestType(HttpRequest::Type::GET);
	request->setUrl(url.c_str());
	request->setResponseCallback([=](HttpClient* client, HttpResponse* response) {
        if (response->isSucceed()) {
			std::vector<char> *buffer = response->getResponseData();
			auto listener = _listenerMap.find(_key);
            
			if (listener != _listenerMap.end())
			{
				listener->second->onHttpDownloaded(_key, buffer);
				_listenerMap.erase(_key);
			}
		}
	});

	HttpClient::getInstance()->sendImmediate(request);

	return _key;
}


void HttpFileDownload::removeLisener(int _key)
{
	_listenerMap.erase(_key);
}
