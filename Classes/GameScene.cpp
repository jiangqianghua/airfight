#include "GameScene.h"
#include "ui/CocosGUI.h"
#include<stdlib.h>
#include "KFSprite.h"
#include "SimpleAudioEngine.h"
#include "GameMenu.h"
using namespace CocosDenshion;
USING_NS_CC;
using namespace ui;

// 不同敌机的tag
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
    // 创建一个场景对象，该对象将会由自动释放池管理内存的释放
    auto scene = Scene::create();
    
    // 创建层对象，该对象将会由自动释放池管理内存的释放
    auto layer = Game::create();
    
    // 将HelloWorld层作为子节点添加到场景
    scene->addChild(layer);
    
    // 返回场景对象
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
	// 将动画添加到缓存帧
	AnimationCache::getInstance()->addAnimation(getAnimationByName("plane",0.08 ,2),"fly");
	AnimationCache::getInstance()->addAnimation(getAnimationByName("blast",0.09 ,4),"blast");
	AnimationCache::getInstance()->addAnimation(getAnimationByName("bomb",10.9 ,4),"bomb");
	return true ;

}
void Game::onEnter()
{
	Layer::onEnter();
	// 添加背景图片
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
	// 第一章背景图
	_bg1 = Sprite::create(bgName);
	_bg1->setAnchorPoint(Vec2::ZERO);
	_bg1->getTexture()->setAliasTexParameters(); //用于解决拼接的地图在连接滚动时容易出现黑色缝隙的问题
	this->addChild(_bg1);
	// 第二章背景图
	_bg2 = Sprite::create(bgName);
	_bg2->setAnchorPoint(Vec2::ZERO);
	_bg2->getTexture()->setAliasTexParameters(); //用于解决拼接的地图在连接滚动时容易出现黑色缝隙的问题
	_bg2->setPosition(Vec2(0,_bg1->getContentSize().height));
	this->addChild(_bg2);


	// 添加玩家飞机精灵
	_plane =Sprite::createWithSpriteFrameName("plane0.png");
	_plane->setPosition(Vec2(_screenWidth/2 , _plane->getContentSize().height/2+5));
	this->addChild(_plane);

	// 添加飞机动画
	auto planeFlyAnimation = AnimationCache::getInstance()->getAnimation("fly");
	auto animate = Animate::create(planeFlyAnimation);
	auto planeFlyAction = RepeatForever::create(animate);
	_plane->runAction(planeFlyAction);

	// 添加touch事件
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
	// 添加事件监听器
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
	// 设置动画播放属性
	animation->setDelayPerUnit(delay);
	// 让精灵对象在动画执行完之后恢复到最初状态
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

// 控制敌机的创建，俯冲
void Game::enemyPlaneDive(float delta)
{
	bool flag = false ; 
	int lifeValue ; 
	KFSprite* enemyPlane ;
	int randX  = random() % (_screenWidth - 40) + 20 ;
	float durationTime = random() % 2 + 2 ; 

	if(_count % 30 == 0) // count为30的倍数，添加一架敌机
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

// 子弹不断发射函数
void Game::shootBullet(float delta)
{
	// 获取飞机坐标
	Vec2 pos = _plane->getPosition();
	if(_count % 8 == 0)
	{
		auto bullet = Sprite::createWithSpriteFrameName("bullet.png");
		// 设置子弹的坐标
		bullet->setPosition(pos.x,pos.y+_plane->getContentSize().height/2 + bullet->getContentSize().height);
		// 子弹移动时间是0.4秒 ,移动距离为屏幕的高度 - 子弹的y
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

//检测碰撞
void Game::collisionDetection(float dalta)
{
	for(unsigned int i = 0 ; i < _enemyVector.size(); i++)
	{
		KFSprite* enemy = _enemyVector.at(i);
		if(_plane->getBoundingBox().intersectsRect(enemy->getBoundingBox()))
		{
			// 检测玩家和敌机是否发生碰撞
			// 播放碰撞动画
			playBomAnimate("blast",enemy->getPosition());
			this->removeChild(enemy);
			_plane->stopAllActions();
			_plane->setVisible(false);
			gameOver("restart");
		}else
		{
			// 遍历子弹数组
			for(unsigned int i = 0 ; i < _bulletVector.size(); i++)
			{
				auto bullet = _bulletVector.at(i);
				// 如果敌机和子弹碰撞
				if(enemy->getBoundingBox().intersectsRect(bullet->getBoundingBox()))
				{
					// 播放子弹音效
					SimpleAudioEngine::getInstance()->playEffect("bullet.mp3");
					// 删除子弹精灵
					_bulletVector.eraseObject(bullet);
					this->removeChild(bullet,true);
					enemy->setLifeValue(enemy->getLifeValue() - 1);
					// 血条减小
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

// 播放爆炸动画
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
	// 创建一个精灵
	auto blast = Sprite::createWithSpriteFrameName(bombName);
	blast->setPosition(postion);
	// 获取动画帧

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

// 游戏结束
void Game::gameOver(std::string message)
{
	this->unscheduleUpdate();
	auto restart_button = createButton2("button.png",message,2,"微软雅黑",16);
	restart_button->setPosition(Vec2(_screenWidth/2 , _screenHeight/2));
	restart_button->addTouchEventListener([=](Ref * pSender ,Widget::TouchEventType type )
	{
		if(type == Widget::TouchEventType::ENDED)
		{
			this->restart(1);
		}
	});

	this->addChild(restart_button,1);
	// 添加返回主菜单按钮
	auto back_button = createButton2("button.png","menu",2 , "微软雅黑",16);
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

