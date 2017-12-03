#include "GameScene.h"
#include "ui/CocosGUI.h"
#include<stdlib.h>
#include "KFSprite.h"
#include "SimpleAudioEngine.h"
#include "GameMenu.h"
using namespace CocosDenshion;
USING_NS_CC;
using namespace ui;

// ��ͬ�л���tag
static int E0TAG = 10 ; 
static int E1TAG = 11 ; 
static int E2TAG = 12 ; 
static int BOSSTAG = 100 ; 

Button *createButton2(const std::string imgPath ,std::string title , float scale , const std::string fontType ,const int fontSize)
{
	auto button = Button::create(imgPath);
	button->setScale(scale);
	button->setTitleText(title);
	button->setTitleFontName(fontType);
	button->setTitleFontSize(fontSize);
	return button;
}


Scene* Game::createScene()
{
    // ����һ���������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
    auto scene = Scene::create();
    
    // ��������󣬸ö��󽫻����Զ��ͷųع����ڴ���ͷ�
    auto layer = Game::create();
    
    // ��HelloWorld����Ϊ�ӽڵ���ӵ�����
    scene->addChild(layer);
    
    // ���س�������
    return scene;
}

bool Game::init()
{
	if(!Layer::init())
	{
		return false ;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	_screenWidth = visibleSize.width ; 
	_screenHeight = visibleSize.height ;
	// ��������ӵ�����֡
	AnimationCache::getInstance()->addAnimation(getAnimationByName("plane",0.08 ,2),"fly");
	AnimationCache::getInstance()->addAnimation(getAnimationByName("blast",0.09 ,4),"blast");
	AnimationCache::getInstance()->addAnimation(getAnimationByName("bomb",10.9 ,4),"bomb");
	return true ;

}
void Game::onEnter()
{
	Layer::onEnter();
	// ��ӱ���ͼƬ
	/*Sprite *bg1 = Sprite::create("bg1.png");
	bg1->setAnchorPoint(Vec2(0,0));
	this->addChild(bg1,-1);*/
	SimpleAudioEngine::getInstance()->playBackgroundMusic("s3.wav",true);
	std::string bgName ;
	if(_screenHeight == 960)
	{
		bgName = "bg1.png";
	}
	else
	{
		bgName = "bg2.png";
	}
	// ��һ�±���ͼ
	_bg1 = Sprite::create(bgName);
	_bg1->setAnchorPoint(Vec2::ZERO);
	_bg1->getTexture()->setAliasTexParameters(); //���ڽ��ƴ�ӵĵ�ͼ�����ӹ���ʱ���׳��ֺ�ɫ��϶������
	this->addChild(_bg1);
	// �ڶ��±���ͼ
	_bg2 = Sprite::create(bgName);
	_bg2->setAnchorPoint(Vec2::ZERO);
	_bg2->getTexture()->setAliasTexParameters(); //���ڽ��ƴ�ӵĵ�ͼ�����ӹ���ʱ���׳��ֺ�ɫ��϶������
	_bg2->setPosition(Vec2(0,_bg1->getContentSize().height));
	this->addChild(_bg2);


	// �����ҷɻ�����
	_plane =Sprite::createWithSpriteFrameName("plane0.png");
	_plane->setPosition(Vec2(_screenWidth/2 , _plane->getContentSize().height/2+5));
	this->addChild(_plane);

	// ��ӷɻ�����
	auto planeFlyAnimation = AnimationCache::getInstance()->getAnimation("fly");
	auto animate = Animate::create(planeFlyAnimation);
	auto planeFlyAction = RepeatForever::create(animate);
	_plane->runAction(planeFlyAction);

	// ���touch�¼�
	auto planeListener = EventListenerTouchOneByOne::create();
	planeListener->onTouchBegan = [](Touch * touch , Event *event)
	{
		auto target = event->getCurrentTarget();
		Vec2 location = touch->getLocation();
		Vec2 locationInNode = target->convertToNodeSpace(location);
		Size s =target->getContentSize();
		Rect rect = Rect(0,0,s.width,s.height);
		if(rect.containsPoint(locationInNode))
		{
			return true ;
		}
		return false ;
	};

	planeListener->onTouchMoved = [](Touch* touch , Event* event)
	{
		auto target = event->getCurrentTarget();
		target->setPosition(target->getPosition() +touch->getDelta());
	};
	// ����¼�������
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(planeListener , _plane);

	this->scheduleUpdate();
}

Animation*  Game::getAnimationByName(std::string animName , float delay , int animatNum) 
{
	auto animation = Animation::create();
	for(unsigned int i = 0 ; i < animatNum ; i++)
	{
		std::string frameName = animName ; 
		frameName.append(StringUtils::format("%d",i)).append(".png");
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName.c_str()));
	}
	// ���ö�����������
	animation->setDelayPerUnit(delay);
	// �þ�������ڶ���ִ����֮��ָ������״̬
	animation->setRestoreOriginalFrame(true);
	return animation ;
}

void Game::updateBackground()
{
	Vec2 p1= _bg1->getPosition();
	p1.y = p1.y - 5; 
	p1.y = p1.y -5 ;
	if(p1.y <= -_bg1->getContentSize().height)
		p1.y = _bg1->getContentSize().height ;
	_bg1->setPosition(p1);

	Vec2 p2= _bg2->getPosition();
	p2.y = p2.y - 5; 
	p2.y = p2.y -5 ;
	if(p2.y <= -_bg2->getContentSize().height)
		p2.y = _bg2->getContentSize().height ;
	_bg2->setPosition(p2);
}

void Game::update(float delat)
{
	updateBackground();
	_count++ ; 
	enemyPlaneDive(delat);
	removeEnemy(delat);

	shootBullet(delat);
	removeBullet(delat);

	collisionDetection(delat);
}

// ���Ƶл��Ĵ���������
void Game::enemyPlaneDive(float delta)
{
	bool flag = false ; 
	int lifeValue ; 
	KFSprite* enemyPlane ;
	int randX  = random() % (_screenWidth - 40) + 20 ;
	float durationTime = random() % 2 + 2 ; 

	if(_count % 30 == 0) // countΪ30�ı��������һ�ܵл�
	{
		int rand = random() % 2 ; 
		switch (rand)
		{
		case 0:
			enemyPlane = KFSprite::createWithSpriteFrameName("e0.png");
			enemyPlane->setTag(E0TAG);
			break ; 
		case 1:
			enemyPlane = KFSprite::createWithSpriteFrameName("e2.png");
			enemyPlane->setTag(E2TAG);
			break ;  
		default:
			break;
		}

		lifeValue = 1 ; 
		enemyPlane->setLifeValue(lifeValue);
		enemyPlane->setScale(0.6);
		flag = true ;
	}
	else
	{
		if(_count % 200 == 0)
		{
			enemyPlane = KFSprite::createWithSpriteFrameName("e1.png");
			enemyPlane->setTag(E1TAG);
			lifeValue = 10 ;
			enemyPlane->setLifeValue(lifeValue);
			enemyPlane->setScale(0.8);
			flag = true ;
		}
	}

	if(flag)
	{
		enemyPlane->setPosition(randX , _screenHeight+enemyPlane->getContentSize().height);
		auto moveBy = MoveBy::create(durationTime , Vec2(0,-enemyPlane->getPosition().y - enemyPlane->getContentSize().height));
		enemyPlane->runAction(moveBy);
		_enemyVector.pushBack(enemyPlane);
		this->addChild(enemyPlane);
		if(enemyPlane->getTag() == E1TAG)
		{
			auto loadingBar = LoadingBar::create("planeHP.png");
			loadingBar->setScale(0.15);
			loadingBar->setDirection(LoadingBar::Direction::LEFT);
			loadingBar->setPercent(100);
			Point pos = enemyPlane->getPosition();
			loadingBar->setPosition(Vec2(pos.x , pos.y+48));
			this->addChild(loadingBar,1);
			auto moveBy2 = MoveBy::create(durationTime , Vec2(0,-pos.y-enemyPlane->getContentSize().height));
			loadingBar->runAction(moveBy2);
			enemyPlane->setHP(loadingBar);
			enemyPlane->setHPInterval(100.0f/enemyPlane->getLifeValue());
		}
	}
}

void Game::removeEnemy(float delta)
{
	for(unsigned int i = 0 ; i < _enemyVector.size() ; i++)
	{
		auto enemy = _enemyVector.at(i);
		if(enemy->getPosition().y <= -enemy->getContentSize().height)
		{
			this->removeChild(enemy , true );
			_enemyVector.eraseObject(enemy);
		}
	}
}

// �ӵ����Ϸ��亯��
void Game::shootBullet(float delta)
{
	// ��ȡ�ɻ�����
	Vec2 pos = _plane->getPosition();
	if(_count % 8 == 0)
	{
		auto bullet = Sprite::createWithSpriteFrameName("bullet.png");
		// �����ӵ�������
		bullet->setPosition(pos.x,pos.y+_plane->getContentSize().height/2 + bullet->getContentSize().height);
		// �ӵ��ƶ�ʱ����0.4�� ,�ƶ�����Ϊ��Ļ�ĸ߶� - �ӵ���y
		auto moveBy = MoveBy::create(0.4f, Vec2(0,_screenHeight-bullet->getPosition().y));
		bullet->runAction(moveBy);
		this->addChild(bullet);
		_bulletVector.pushBack(bullet);
	}
}

void Game::removeBullet(float delta)
{
	for(unsigned int i = 0 ; i < _bulletVector.size() ; i++)
	{
		auto bullet = _bulletVector.at(i);
		if(bullet->getPosition().y >= _screenHeight)
		{
			this->removeChild(bullet,true);
			_bulletVector.eraseObject(bullet);
		}
	}
}

//�����ײ
void Game::collisionDetection(float dalta)
{
	for(unsigned int i = 0 ; i < _enemyVector.size(); i++)
	{
		KFSprite* enemy = _enemyVector.at(i);
		if(_plane->getBoundingBox().intersectsRect(enemy->getBoundingBox()))
		{
			// �����Һ͵л��Ƿ�����ײ
			// ������ײ����
			playBomAnimate("blast",enemy->getPosition());
			this->removeChild(enemy);
			_plane->stopAllActions();
			_plane->setVisible(false);
			gameOver("restart");
		}else
		{
			// �����ӵ�����
			for(unsigned int i = 0 ; i < _bulletVector.size(); i++)
			{
				auto bullet = _bulletVector.at(i);
				// ����л����ӵ���ײ
				if(enemy->getBoundingBox().intersectsRect(bullet->getBoundingBox()))
				{
					// �����ӵ���Ч
					SimpleAudioEngine::getInstance()->playEffect("bullet.mp3");
					// ɾ���ӵ�����
					_bulletVector.eraseObject(bullet);
					this->removeChild(bullet,true);
					enemy->setLifeValue(enemy->getLifeValue() - 1);
					// Ѫ����С
					if(enemy->getTag() == E1TAG)
					{
						if(enemy->getHP() != nullptr)
						{
							enemy->getHP()->setPercent(enemy->getHPInterval()*enemy->getLifeValue());
						}
					}
					if(enemy->getLifeValue() <= 0)
					{
						SimpleAudioEngine::getInstance()->playEffect("b0.mp3");
						playBomAnimate("blast",enemy->getPosition());
						_enemyVector.eraseObject(enemy);
						this->removeChild(enemy);
					}
					break ;
				}
			}
		}
	}
}

// ���ű�ը����
void Game::playBomAnimate(std::string name , Vec2 postion)
{
	Animation* blastAnimation ; 
	//if(name.compare("bomb") == 0)
	//{
	//	blastAnimation = AnimationCache::getInstance()->getAnimation("bomb");
	//}
	//else
	//{
	//	blastAnimation = AnimationCache::getInstance()->getAnimation("blast");
	//}
	std::string bombName = name+"0.png";
	// ����һ������
	auto blast = Sprite::createWithSpriteFrameName(bombName);
	blast->setPosition(postion);
	// ��ȡ����֡

	blastAnimation = getAnimationByName(name , 0.5f , 4);
	auto animate = Animate::create(blastAnimation);
	auto callFunc = CallFunc::create([=]
	{
		this->removeChild(blast , true);
	});

	auto action = Sequence::create(animate , callFunc , NULL);

	blast->runAction(action);
	this->addChild(blast);
}

// ��Ϸ����
void Game::gameOver(std::string message)
{
	this->unscheduleUpdate();
	auto restart_button = createButton2("button.png",message,2,"΢���ź�",16);
	restart_button->setPosition(Vec2(_screenWidth/2 , _screenHeight/2));
	restart_button->addTouchEventListener([=](Ref * pSender ,Widget::TouchEventType type )
	{
		if(type == Widget::TouchEventType::ENDED)
		{
			this->restart(1);
		}
	});

	this->addChild(restart_button,1);
	// ��ӷ������˵���ť
	auto back_button = createButton2("button.png","menu",2 , "΢���ź�",16);
	back_button->setPosition(Vec2(_screenWidth/2 , _screenHeight*0.4f));
	back_button->addTouchEventListener([=](Ref *pSender , Widget::TouchEventType type)
	{
		if(type == Widget::TouchEventType::ENDED)
		{
			this->restart(2);
		}
	});
	this->addChild(back_button);
}

void Game::restart(int tag)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	if(tag == 1)
		Director::getInstance()->replaceScene(Game::createScene());
	else
		Director::getInstance()->replaceScene(GameMenu::createScene());
}

