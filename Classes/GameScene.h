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
	int _count  ;  //��Ϸ֡��ʱ��
	Vector<KFSprite*> _enemyVector ;
	Vector<Sprite*> _bulletVector ; //�ӵ�����
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
	virtual void onEnter() ;


	CREATE_FUNC(Game);

	Animation*  getAnimationByName(std::string animName , float delay , int animatNum) ;

	void update(float delat);
	// ����ͼƬ��������
	void updateBackground();
	//���µл�����
	void enemyPlaneDive(float delta) ; 
	//�л��뿪��Ļɾ������
	void removeEnemy(float delta) ; 

	// ���º�����Ϊ����ӵ��������
	// ��ҷɻ����Ϸ����ӵ�����
	void shootBullet(float delta) ;
	//�ӵ��뿪��Ļɾ������
	void removeBullet(float delta) ;
	//�����ײ
	void collisionDetection(float delta);
	//���ű�ը����
	void playBomAnimate(std::string name , Vec2 postion);
	// ��Ϸ����
	void gameOver(std::string message);
	//���¿�ʼ
	void restart(int tag);
};

#endif // __PreLoad_H__
