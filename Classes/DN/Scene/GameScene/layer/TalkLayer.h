#ifndef __TALK_LAYER_H__
#define __TALK_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

class TalkLayer : public cocos2d::Layer
{
public:

    virtual bool init();
    
    CREATE_FUNC(TalkLayer);
    void setRoomID(int roomId);
    void showDialog(Node *parent);
  
	void touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
  
    void touchQuickTalkEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void touchExpressEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void touchChatEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void touchButtonExpressEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
private:
    void closeSelf();
private:
    Node    *m_pRootNode;
    int     m_nRoomId;
    Node    *m_pParent;

	Button* chatButton;
	Button* expressButton;

	Layout* m_layout;

	Layout* m_eLayout;

	std::vector<ImageView*> m_imageView;

};
#endif // __TALK_LAYER_H__
