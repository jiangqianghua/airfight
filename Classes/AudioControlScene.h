#ifndef __AudioControlScene_H__
#define __AudioControlScene_H__

#include "cocos2d.h"
USING_NS_CC;

/** ��ʾ��Ϸ�˵�������*/
class AudioControlScene : public Layer
{
public:
	static Scene* createScene() ;
	virtual bool init();
	CREATE_FUNC(AudioControlScene);
};

#endif // __GameMenu_H__