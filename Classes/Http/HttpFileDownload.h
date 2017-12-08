#pragma once

#include "cocos2d.h"
#include "network/HttpClient.h"
USING_NS_CC;

class HttpListener
{
public:
	virtual void onHttpDownloaded(int index, std::vector<char> *) = 0;
};

class HttpFileDownload : public Node
{
public:

	static HttpFileDownload* getInstance();

	~HttpFileDownload();

	int download(const std::string& url, HttpListener *pListener);

	void removeLisener(int key);

private:
	HttpFileDownload();

private:

	std::map<int, HttpListener* >		_listenerMap;

	int _index;

	static HttpFileDownload* theInstance;
};
