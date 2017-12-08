#ifndef __QGAME_SCENE_H__
#define __QGAME_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "bean/PocketSprite.h"
#include <vector>
using namespace std;
using namespace cocos2d::ui;

using namespace cocos2d;

class QGameLayer;
class QGameScene : public Scene
{
public:
	QGameScene();
	~QGameScene();

	virtual bool init();
	CREATE_FUNC(QGameScene);

public:
	void initListenter();
	
private:
	QGameLayer *m_pGameLayer_;
};

#endif
