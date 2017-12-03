#ifndef __PreLoad_H__
#define __PreLoad_H__

#include "cocos2d.h"
USING_NS_CC;
class PreLoad : public cocos2d::Layer
{
protected:
	int _sourceCount ;  //用来保存游戏需要加载资源的总数
	ProgressTimer* _progress ; // 进度条
	float _progressInterval ; //进度条更新的次数
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
   //当节点加入舞台调用
	virtual void onEnterTransitionDidFinish();

	// 加载背景音乐
	void loadMusic(ValueVector musicFiles);

	// 加载游戏特效
	void loadEffect(ValueVector effectFiles);

	// 加载精灵表单
	void loadSpriteSheets(ValueVector spriteSheets);
	// 更新游戏进度条
	void progressUpdate();

	CREATE_FUNC(PreLoad);
};

#endif // __PreLoad_H__
