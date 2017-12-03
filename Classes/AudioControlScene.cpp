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

	
	//  ------------------------------ �����������ֻ�����-------------------------
	// �������������ı�
	auto music_text = Text::create("Music","Arial",32);
	music_text->setPosition(Vec2(visibleSize.width*0.25 , visibleSize.height*0.7));
	this->addChild(music_text);
	// ����һ��������
	auto music_slider = Slider::create();
	// ���û�����������
	music_slider->loadBarTexture("sliderTrack.png");
	// ���û������Ĺ�������
	music_slider->loadSlidBallTextures("sliderThumb.png","sliderThumb.png","");
	// ���ô��������Ľ�������
	music_slider->loadProgressBarTexture("sliderProgress.png");
	// ��ȡ֮ǰ���õı�������
	float musicPercent = UserDefault::getInstance()->getFloatForKey("musicPercent");
	//����ǵ�һ�ν��볡�������ñ���������Ϊ100
	if(musicPercent == 0.0f)
	{
		musicPercent = 100.0f ;
	}
	// ���ñ������ֻ������ĳ�ʼֵ
	music_slider->setPercent(musicPercent);
	music_slider->setPosition(Vec2(visibleSize.width*0.6 , visibleSize.height*0.7));
	// ����¼������������������ֵ�����
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


	//  ------------------------------ ����������Ч������-------------------------
		// �������������ı�
	auto sound_text = Text::create("Effect","Arial",32);
	sound_text->setPosition(Vec2(visibleSize.width*0.25 , visibleSize.height*0.5));
	this->addChild(sound_text);
	// ����һ��������
	auto effects_slider = Slider::create();
	// ���û�����������
	effects_slider->loadBarTexture("sliderTrack.png");
	// ���û������Ĺ�������
	effects_slider->loadSlidBallTextures("sliderThumb.png","sliderThumb.png","");
	// ���ô��������Ľ�������
	effects_slider->loadProgressBarTexture("sliderProgress.png");
	// ��ȡ֮ǰ���õı�������
	float effectPercent = UserDefault::getInstance()->getFloatForKey("effectPercent");
	//����ǵ�һ�ν��볡�������ñ���������Ϊ100
	if(effectPercent == 0.0f)
	{
		effectPercent = 100.0f ;
	}
	// ���ñ������ֻ������ĳ�ʼֵ
	effects_slider->setPercent(effectPercent);
	effects_slider->setPosition(Vec2(visibleSize.width*0.6 , visibleSize.height*0.5));
	// ����¼������������������ֵ�����
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

	// �������ذ�ť
	auto return_button = Button::create("b1.png");
	return_button->setPosition(Vec2(visibleSize.width - return_button->getContentSize().width/2 , return_button->getContentSize().height/2));
	return_button->addTouchEventListener([](Ref* pSender , Widget::TouchEventType type)
	{
		if(type == Widget::TouchEventType::ENDED)
		{
			// �л����˵�����
			auto transition = TransitionSlideInL::create(2.0,GameMenu::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});
	this->addChild(return_button);
	return true ;
}


