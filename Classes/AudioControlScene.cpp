#include "AudioControlScene.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "GameMenu.h"
using namespace CocosDenshion;
using namespace cocos2d::ui;


Scene* AudioControlScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = AudioControlScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool AudioControlScene::init()
{
	if(!Layer::init())
		return false ;

	Size visibleSize = Director::getInstance()->getVisibleSize();

	
	//  ------------------------------ 创建背景音乐滑动条-------------------------
	// 创建背景音乐文本
	auto music_text = Text::create("Music","Arial",32);
	music_text->setPosition(Vec2(visibleSize.width*0.25 , visibleSize.height*0.7));
	this->addChild(music_text);
	// 创建一个滑动条
	auto music_slider = Slider::create();
	// 设置滑动条的纹理
	music_slider->loadBarTexture("sliderTrack.png");
	// 设置滑动条的滚动纹理
	music_slider->loadSlidBallTextures("sliderThumb.png","sliderThumb.png","");
	// 设置处理滑动条的进度纹理
	music_slider->loadProgressBarTexture("sliderProgress.png");
	// 获取之前设置的背景音量
	float musicPercent = UserDefault::getInstance()->getFloatForKey("musicPercent");
	//如果是第一次进入场景，设置背景滑动条为100
	if(musicPercent == 0.0f)
	{
		musicPercent = 100.0f ;
	}
	// 设置背景音乐滑动条的初始值
	music_slider->setPercent(musicPercent);
	music_slider->setPosition(Vec2(visibleSize.width*0.6 , visibleSize.height*0.7));
	// 添加事件监听，调整背景音乐的音量
	music_slider->addEventListener([=](Ref* pSender , Slider::EventType type)
	{
		if(type == Slider::EventType::ON_PERCENTAGE_CHANGED)
		{
			int percent = music_slider->getPercent();
			SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(float(percent)/100);
			UserDefault::getInstance()->setFloatForKey("musicPercent",percent);
		}
	});
	this->addChild(music_slider);


	//  ------------------------------ 创建背景音效滑动条-------------------------
		// 创建背景音乐文本
	auto sound_text = Text::create("Effect","Arial",32);
	sound_text->setPosition(Vec2(visibleSize.width*0.25 , visibleSize.height*0.5));
	this->addChild(sound_text);
	// 创建一个滑动条
	auto effects_slider = Slider::create();
	// 设置滑动条的纹理
	effects_slider->loadBarTexture("sliderTrack.png");
	// 设置滑动条的滚动纹理
	effects_slider->loadSlidBallTextures("sliderThumb.png","sliderThumb.png","");
	// 设置处理滑动条的进度纹理
	effects_slider->loadProgressBarTexture("sliderProgress.png");
	// 获取之前设置的背景音量
	float effectPercent = UserDefault::getInstance()->getFloatForKey("effectPercent");
	//如果是第一次进入场景，设置背景滑动条为100
	if(effectPercent == 0.0f)
	{
		effectPercent = 100.0f ;
	}
	// 设置背景音乐滑动条的初始值
	effects_slider->setPercent(effectPercent);
	effects_slider->setPosition(Vec2(visibleSize.width*0.6 , visibleSize.height*0.5));
	// 添加事件监听，调整背景音乐的音量
	effects_slider->addEventListener([=](Ref* pSender , Slider::EventType type)
	{
		if(type == Slider::EventType::ON_PERCENTAGE_CHANGED)
		{
			int percent = effects_slider->getPercent();
			SimpleAudioEngine::getInstance()->setEffectsVolume(float(percent)/100);
			UserDefault::getInstance()->setFloatForKey("effectPercent",percent);
		}
	});
	this->addChild(effects_slider);

	// 创建返回按钮
	auto return_button = Button::create("b1.png");
	return_button->setPosition(Vec2(visibleSize.width - return_button->getContentSize().width/2 , return_button->getContentSize().height/2));
	return_button->addTouchEventListener([](Ref* pSender , Widget::TouchEventType type)
	{
		if(type == Widget::TouchEventType::ENDED)
		{
			// 切换到菜单场景
			auto transition = TransitionSlideInL::create(2.0,GameMenu::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});
	this->addChild(return_button);
	return true ;
}


