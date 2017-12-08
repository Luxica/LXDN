#ifndef __SORT_SCENE_H__
#define __SORT_SCENE_H__

#include <vector>
using namespace std;

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "../data/QHallManager.h"

using namespace cocos2d::ui;
USING_NS_CC;

class SortScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
	CREATE_FUNC(SortScene);


public:

	void touchBackEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void touchRankListEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void updateRank();
    
    void updateRankUrlIcon();
    int m_RankCounts;
    int m_startDownIndex;

private:

	Node* rootNode;
	Layout* today_Layout;
	Layout* tomorrow_Layout;
	Layout* tsWeek_Layout;
	Layout* lastWeek_Layout;

	ListView* listView;
	Layout *m_signleTempLayout;

	Vector<Node*> vecPanel;
    Text*   m_selfNameText;
    Layout* m_selfHeadLayout;
    Text*   m_selfWinText;
    Text*   m_selfScoreText;
    Text*   m_selfSortText;
};

#endif // __SORT_SCENE_H__
