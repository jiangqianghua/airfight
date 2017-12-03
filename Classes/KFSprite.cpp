#include "KFSprite.h"	

KFSprite* KFSprite::createWithSpriteFrameName(const std::string & filename)
	{
		KFSprite * sprite = new KFSprite();
		if(sprite && sprite->initWithSpriteFrameName(filename))
		{
			sprite->autorelease(); //设置自动释放
			return sprite ;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr ;
	}

	void KFSprite::setLifeValue(int lifeValue)
	{
		_lifeValue =lifeValue ;
	}

	int KFSprite::getLifeValue()
	{
		return _lifeValue ;
	}

	void KFSprite::setHP(LoadingBar* HP)
	{
		_HP = HP ;
	}

	LoadingBar* KFSprite::getHP()
	{
		return _HP ;
	}

	void KFSprite::setHPInterval(float HPInterval)
	{
		_HPInterval = HPInterval ;
	}

	float KFSprite::getHPInterval()
	{
		return _HPInterval ;
	}