#include "GameMenu.h"
#include "ui/CocosGUI.h"
#include "AudioControlScene.h"
#include "PreLoadScene.h"
using namespace cocos2d::ui;


Button *createButton1(const std::string imgPath ,std::string title , float scale , const std::string fontType ,const int fontSize)
{
	auto button = Button::create(imgPath);
	button->setScale(scale);
	button->setTitleText(title);
	button->setTitleFontName(fontType);
	button->setTitleFontSize(fontSize);
	return button;
}

Scene* GameMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameMenu::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool GameMenu::init()
{
	if(!Layer::init())
		return false ;

	Size visibleSize = Director::getInstance()->getVisibleSize();

	// ��ʼ��Ϸ ��ť
	auto start_button = createButton1("button.png","start",2,"΢���ź�",16);
	start_button->setPosition(Vec2(visibleSize.width/2 , visibleSize.height*0.7));
	// ����¼�
	start_button->addTouchEventListener([](Ref* pSender , Widget::TouchEventType type)
	{
		if(type == Widget::TouchEventType::ENDED)
		{
			// �л���Ԥ���س���
			auto transition = TransitionSlideInL::create(2.0,PreLoad::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});

	this->addChild(start_button);

	// ��Ϸ���ð�ť
	auto set_button = createButton1("button.png","set",2,"΢���ź�",16);
	set_button->setPosition(Vec2(visibleSize.width/2 , visibleSize.height*0.55));
	// ����¼�
	set_button->addTouchEventListener([](Ref* pSender , Widget::TouchEventType type)
	{
		if(type == Widget::TouchEventType::ENDED)
		{
			// �л���Ԥ���س���
			auto transition = TransitionSlideInL::create(2.0,AudioControlScene::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});
	this->addChild(set_button);

	// �˳���Ϸ
	auto close_button = createButton1("button.png","close",2,"΢���ź�",16);
	close_button->setPosition(Vec2(visibleSize.width/2 , visibleSize.height*0.4));
	// ����¼�
	close_button->addTouchEventListener([](Ref* pSender , Widget::TouchEventType type)
	{
		if(type == Widget::TouchEventType::ENDED)
		{
			// ��Ҫ�ж����Ǹ�ƽ̨
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
			MessageBox("you pressed the close button , Windows Store Apps do not implement a close button","Alert");
			return ;
#endif
			Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			exit(0);
#endif
		}
	});

	this->addChild(close_button);


	return true ;
}


