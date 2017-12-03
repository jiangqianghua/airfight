#ifndef __PreLoad_H__
#define __PreLoad_H__

#include "cocos2d.h"
USING_NS_CC;
class PreLoad : public cocos2d::Layer
{
protected:
	int _sourceCount ;  //����������Ϸ��Ҫ������Դ������
	ProgressTimer* _progress ; // ������
	float _progressInterval ; //���������µĴ���
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
   //���ڵ������̨����
	virtual void onEnterTransitionDidFinish();

	// ���ر�������
	void loadMusic(ValueVector musicFiles);

	// ������Ϸ��Ч
	void loadEffect(ValueVector effectFiles);

	// ���ؾ����
	void loadSpriteSheets(ValueVector spriteSheets);
	// ������Ϸ������
	void progressUpdate();

	CREATE_FUNC(PreLoad);
};

#endif // __PreLoad_H__
