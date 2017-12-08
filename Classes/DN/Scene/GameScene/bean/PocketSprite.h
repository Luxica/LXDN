#ifndef  _POCKET_SPRITE_H_ 
#define  _POCKET_SPRITE_H_

#include "cocos2d.h"
#include "2d/CCSprite.h"

#include <vector>
using namespace std;

USING_NS_CC;
using namespace cocos2d::ui;

class PocketSprite : public Sprite
{
public:
	//扑克牌的花型
	enum PocketType
	{
		PT_FP,		//方片
		PT_MH,		//梅花
		PT_RED,		//红桃
		PT_BLACK,	//黑桃
	};
	//扑克牌的点数
	enum PocketNum
	{
		PN_A    =1,	//A
		PN_TWO,		//2
		PN_THR,		//3
		PN_FOU,		//4
		PN_FIV,		//5
		PN_SIX,		//6
		PN_SEV,		//7
		PN_EIG,		//8
		PN_NIG,		//9
		PN_TEN,		//10
		PN_J,
		PN_Q,
		PN_K,
	};
	enum PocketState
	{
		PS_INIT,
		PS_ISUP,
	};
public:
	PocketSprite(){};
	virtual~PocketSprite(){};

public:

	static PocketSprite* create(const std::string& filename, PocketType pt, PocketNum pn);

	virtual bool init(PocketType pt, PocketNum pn);

	virtual void update(float dt);
	void setIsUp(bool isUp)
	{
		m_isUP = isUp;
		int posY = getPositionY();
		setPositionY(isUp ? posY + 20 : posY - 20);
	}
	bool getIsUp(){ return m_isUP; }

private:                                    

	CC_SYNTHESIZE(PocketType, m_pocketType, PType);	//牌花型
	CC_SYNTHESIZE(PocketNum, m_pocketNum, PNum);	//牌点数

	//CC_SYNTHESIZE(bool, m_isUP, isUP);				//牌弹出
	bool m_isUP;
	void addBackSprite();
	void setisBack(bool value);

};
#endif //_GENERAL_SPRITE_H_
