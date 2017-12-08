#ifndef __QSTART_SCENE_H__
#define __QSTART_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class QStartScene : public Scene
{
public:
	QStartScene();
	~QStartScene();

	CREATE_FUNC(QStartScene);

	virtual bool init();

	std::string getSceneName()	{ return "QStartScene"; }
};

#endif