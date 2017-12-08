#include "PocketSprite.h"

PocketSprite* PocketSprite::create(const std::string& filename, PocketType pt, PocketNum pn)
{
	PocketSprite* pocketSprite = new PocketSprite;

	if (pocketSprite&&pocketSprite->initWithFile(filename))
	{
		pocketSprite->init(pt,pn);
		pocketSprite->autorelease();
		return pocketSprite;
	}
	CC_SAFE_DELETE(pocketSprite);
	
	return nullptr;
}

bool PocketSprite::init(PocketType pt, PocketNum pn)
{
	m_pocketType = pt;
	m_pocketNum = pn;
	m_isUP = false;

	return true;
}

void PocketSprite::update(float dt)
{

}

void PocketSprite::setisBack(bool value)
{
	Sprite *sprite = dynamic_cast<Sprite*>(getChildByTag(1));
	if (sprite)
	{
		sprite->setVisible(value);
	}
}

void PocketSprite::addBackSprite()
{
	Sprite *sprite = dynamic_cast<Sprite*>(getChildByTag(1));
	if (sprite==nullptr)
	{
		Sprite *sprite = Sprite::create("Pocke/Back.png");
		sprite->setAnchorPoint(Vec2::ZERO);
		sprite->setPosition(Vec2::ZERO);
		addChild(sprite, 1, 1);
	}
}

