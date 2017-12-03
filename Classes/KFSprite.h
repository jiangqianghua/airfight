#ifndef __KFSprite_H__
#define __KFSprite_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC ; 
using namespace cocos2d::ui ;


class KFSprite : public cocos2d::Sprite
{
private :
	int _lifeValue ; //����ֵ
	LoadingBar* _HP ; //Ѫ��
	float _HPInterval ; // Ѫ���ĸ�����
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
