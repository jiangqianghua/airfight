#include "PreLoadScene.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"
using namespace CocosDenshion;
USING_NS_CC ;

Scene* PreLoad::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = PreLoad::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool PreLoad::init()
{
		//1 ��ʼ��ͼ��
	if(!Layer::init())
	{
		return false ;
	}

	// ��ȡ�豸֧�ֵĿɼ�OpenGL��ͼ��С�������ڴ�С��
	Size visibleSize =Director::getInstance()->getVisibleSize();

	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// ����һ��������
	auto barSprite = Sprite::create("loadingbar.png");
	// ��ʼ��������
	_progress = ProgressTimer::create(barSprite);
	_progress->setPercentage(0.0f);
	_progress->setScale(0.5f);
	_progress->setMidpoint(Vec2(0.0f,0.5f));
	_progress->setBarChangeRate(Vec2(1.0f , 0.0f));
	_progress->setType(ProgressTimer::Type::BAR);
	_progress->setPosition(Vec2(visibleSize.width/2 , visibleSize.height/2));
	this->addChild(_progress);
	return true ;
}

void PreLoad::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();

	// ����plist�ļ�
	ValueMap map = FileUtils::getInstance()->getValueMapFromFile("preloadResources.plist");
	ValueVector spriteSheets = map.at("SpriteSheets").asValueVector();
	ValueVector effects = map.at("Sounds").asValueVector();
	ValueVector musics = map.at("Musics").asValueVector();

	_sourceCount = spriteSheets.size() + effects.size() + musics.size(); 
	_progressInterval = 100 / _sourceCount ;
	// ���μ�����Դ
	loadMusic(musics) ; 
	loadEffect(effects);
	loadSpriteSheets(spriteSheets);
}

// ���ر�������
void PreLoad::loadMusic(ValueVector musicFiles)
{
	for(unsigned int i = 0 ; i != musicFiles.size(); i++)
	{
		Value v = musicFiles.at(i);
		SimpleAudioEngine::getInstance()->preloadBackgroundMusic(v.asString().c_str());
		progressUpdate();
	}
}

// ��������
void PreLoad::loadEffect(ValueVector effectFiles)
{
	for(unsigned int i = 0 ; i != effectFiles.size(); i++)
	{
		Value v = effectFiles.at(i);
		SimpleAudioEngine::getInstance()->preloadEffect(v.asString().c_str());
		progressUpdate();
	}
}

// ���ؾ����
void PreLoad::loadSpriteSheets(ValueVector spriteSheets)
{
	for(unsigned int i = 0 ; i != spriteSheets.size(); i++)
	{
		Value v = spriteSheets.at(i);
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(v.asString().c_str());
		progressUpdate();
	}
}

//���½�����
void PreLoad::progressUpdate()
{
	if(--_sourceCount)
	{
		_progress->setPercentage(100.0f - (_progressInterval*_sourceCount));

	}
	else
	{
		auto pft = ProgressFromTo::create(0.5f , _progress->getPercentage(),100);
		auto callFunc = CallFunc::create([=]
		{
			auto delay = DelayTime::create(2.0f);
			auto callFunc = CallFunc::create([]
			{
				Director::getInstance()->replaceScene(Game::createScene());
			});

			auto action = Sequence::create(delay , callFunc , NULL);
			this->runAction(action);
		});
		auto action = Sequence::create(pft , callFunc , NULL);
		_progress->runAction(action);
	}
}