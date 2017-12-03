#ifndef __KFSprite_H__
#define __KFSprite_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC ; 
using namespace cocos2d::ui ;


class KFSprite : public cocos2d::Sprite
{
private :
	int _lifeValue ; //生命值
	LoadingBar* _HP ; //血条
	float _HPInterval ; // 血条的更新量
public:
	static KFSprite* createWithSpriteFrameName(const std::string & filename);

	void setLifeValue(int lifeValue);

	int getLifeValue();

	void setHP(LoadingBar* HP);
	LoadingBar* getHP();

	void setHPInterval(float HPInterval);

	float getHPInterval();
};

#endif 
