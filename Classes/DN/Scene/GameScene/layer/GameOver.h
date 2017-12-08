#ifndef __GAMEOVER_LAYER_H__
#define __GAMEOVER_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "../data/QGameBaseData.h"
#include "../data/QGameManager.h"

#include <vector>
using namespace std;
using namespace cocos2d::ui;

class GameOverLayer: public cocos2d::Layer
{
public:

	void touchShareEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchReplayEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    
    void onInviteCB(int errorCode);

public:
 
    virtual bool init();
    
    // implement the "static create()" method manually
	CREATE_FUNC(GameOverLayer);

	void onRefreshData(QUserData* m_player[]);
	
private:
	ListView* m_listView1;
	ListView* m_listView2;
	ListView* m_listView3;
	Layout* m_tempLayout;
	Layout* layout;
};

#endif // __GAMEOVER_LAYER_H__
