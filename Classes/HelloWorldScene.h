#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

#define LINE_SPACE 150   // ÿ���˵���֮��ļ��
class HelloWorld : public cocos2d::Layer
{
protected:
	cocos2d::Vec2 _beginPos ;  // ��¼�����Ŀ�ʼ����
	cocos2d::Menu* _itemMenu ;  //�˵�
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    void menuCloseCallback(cocos2d::Ref* pSender);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
	void menuCallback(Ref *sender);
	// �����¼��ص�����
	bool onTouchBegan(cocos2d::Touch* touch , cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch , cocos2d::Event* event);
};

#endif // __HELLOWORLD_SCENE_H__
