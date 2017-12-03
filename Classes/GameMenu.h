#ifndef __GameMenu_H__
#define __GameMenu_H__

#include "cocos2d.h"
USING_NS_CC;

/** 显示游戏菜单场景类*/
class GameMenu : public Layer
{
public:
	static Scene* createScene() ;
	virtual bool init();
	CREATE_FUNC(GameMenu);
};

#endif // __GameMenu_H__