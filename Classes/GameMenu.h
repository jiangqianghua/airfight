#ifndef __GameMenu_H__
#define __GameMenu_H__

#include "cocos2d.h"
USING_NS_CC;

/** ��ʾ��Ϸ�˵�������*/
class GameMenu : public Layer
{
public:
	static Scene* createScene() ;
	virtual bool init();
	CREATE_FUNC(GameMenu);
};

#endif // __GameMenu_H__