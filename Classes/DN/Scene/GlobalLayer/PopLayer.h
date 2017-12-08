#ifndef __POP_LAYER_H__
#define __POP_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;
USING_NS_CC;

class PopLayer : public cocos2d::Layer
{
public:
	PopLayer();
	~PopLayer();
	virtual bool init();

	//设置文本内容，padding 为文字到对话框两边预留的距离，这是可控的，距上方的距离亦是如此
	void setContentText(const std::string& text, int fontsize = 40, int padding = 50, int paddingTop = 100);

	//为了在显示层时的属性生效，选择在onEnter里动态生成
	virtual void onEnter();
	virtual void onExit();

	CREATE_FUNC(PopLayer);

	void stopCallBack();

private:
	

	//文字内容两边的空白区域
	int m_contentPadding;
	int m_contentPaddingTop;

	LabelTTF* m_ltContentText;
};

#endif