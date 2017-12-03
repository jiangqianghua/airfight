#ifndef __Game_H__
#define __Game_H__
#include <iostream>
#include "cocos2d.h"
#include "KFSprite.h"
USING_NS_CC;

class Game : public cocos2d::Layer
{
private:
	Sprite* _plane ;
	int _screenWidth , _screenHeight ;
	Sprite* _bg1 , *_bg2 ;
	int _count  ;  //游戏帧计时器
	Vector<KFSprite*> _enemyVector ;
	Vector<Sprite*> _bulletVector ; //子弹数组
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
	virtual void onEnter() ;


	CREATE_FUNC(Game);

	Animation*  getAnimationByName(std::string animName , float delay , int animatNum) ;

	void update(float delat);
	// 背景图片滚动函数
	void updateBackground();
	//更新敌机函数
	void enemyPlaneDive(float delta) ; 
	//敌机离开屏幕删除函数
	void removeEnemy(float delta) ; 

	// 以下函数是为玩家子弹函数设计
	// 玩家飞机不断发射子弹函数
	void shootBullet(float delta) ;
	//子弹离开屏幕删除函数
	void removeBullet(float delta) ;
	//检测碰撞
	void collisionDetection(float delta);
	//播放爆炸动画
	void playBomAnimate(std::string name , Vec2 postion);
	// 游戏结束
	void gameOver(std::string message);
	//重新开始
	void restart(int tag);
};

#endif // __PreLoad_H__
