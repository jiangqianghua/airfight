#include "HelloWorldScene.h"
#include "GameMenu.h"
USING_NS_CC;

// �˵��ṹ��
typedef struct _Controller 
{
	const std::string itemName ;
	std::function<Scene* ()> callback ;
}Controller ;

Controller itemNames[] = 
{

	{"Airfight",[](){return GameMenu::createScene();}}
	
};

// ����˵����ĸ���
static int itemCount = sizeof(itemNames)/sizeof(itemNames[0]);

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    
	//1 ��ʼ��ͼ��
	if(!Layer::init())
	{
		return false ;
	}

	// ��ȡ�豸֧�ֵĿɼ�OpenGL��ͼ��С�������ڴ�С��
	Size visibleSize =Director::getInstance()->getVisibleSize();

	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// MenuItemImageʹ��
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback,this));
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,origin.y+closeItem->getContentSize().height/2 ));
	// ���Թ�����Ļѡ��˵��¼�
	// �����˵�
	_itemMenu = Menu::create();
	// ѭ����Ӳ˵���
	for(int i = 0 ; i < itemCount ; i++)
	{
		auto label = Label::createWithSystemFont(itemNames[i].itemName,"",48);
		auto menuItem = MenuItemLabel::create(label,CC_CALLBACK_1(HelloWorld::menuCallback,this));
		menuItem->setTag(i);
		_itemMenu->addChild(menuItem);
		menuItem->setPosition(Vec2(visibleSize.width/2 ,(visibleSize.height-(i+1)*LINE_SPACE)));
	}

	_itemMenu->setPosition(Vec2::ZERO);

	this->addChild(_itemMenu);

	// �������㴥��������
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	// ���ô����Ļص�����
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);

	return true ;
}
// ��ʼ�����¼�
bool HelloWorld::onTouchBegan(cocos2d::Touch *touch , cocos2d::Event* event)
{
	// ��¼����������
	_beginPos = touch->getLocation();
	return true ;
}
// �ƶ��¼�
void HelloWorld::onTouchMoved(cocos2d::Touch *touch , cocos2d::Event *event)
{
	Size visibileSize = Director::getInstance()->getVisibleSize(); // ��ȡ�ɼ�����ߴ�
	auto touchLocation = touch->getLocation();
	float nextMoveY = touchLocation.y - _beginPos.y ; // ��Ϊ�������ƶ����õ�ǰ�������� - ֮ǰ��¼ ��ȡƫ����
	// ��ȡ��ǰ�˵���λ��
	auto curPos = _itemMenu->getPosition();
	// �������һ��λ��
	auto nexPos = Vec2(curPos.x , curPos.y +nextMoveY);

	if(nexPos.y < 0.0f)
	{
		_itemMenu->setPosition(Vec2::ZERO);
		return ;
	}
	// ���nextpos��y���ڲ˵����ܸ߶� - ��Ļ�߶ȣ������������
	if(nexPos.y > (itemCount+1)*LINE_SPACE - visibileSize.height)
	{
		_itemMenu->setPosition(Vec2(0,(itemCount+1)*LINE_SPACE - visibileSize.height));
		return ;
	}
	_itemMenu->setPosition(nexPos);
	_beginPos = touchLocation;

}

void HelloWorld::menuCallback(cocos2d::Ref * sender)
{
	auto menuItem = static_cast<MenuItem*>(sender);
	int index = menuItem->getTag();
	auto scene = itemNames[index].callback();
	// �л�����
	if(scene)
	{
		Director::getInstance()->replaceScene(scene);
	}
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
	#endif
}

