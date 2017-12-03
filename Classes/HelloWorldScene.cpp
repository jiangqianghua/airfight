#include "HelloWorldScene.h"
#include "GameMenu.h"
USING_NS_CC;

// 菜单结构体
typedef struct _Controller 
{
	const std::string itemName ;
	std::function<Scene* ()> callback ;
}Controller ;

Controller itemNames[] = 
{

	{"Airfight",[](){return GameMenu::createScene();}}
	
};

// 计算菜单量的个数
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
    
	//1 初始化图层
	if(!Layer::init())
	{
		return false ;
	}

	// 获取设备支持的可见OpenGL视图大小（即窗口大小）
	Size visibleSize =Director::getInstance()->getVisibleSize();

	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// MenuItemImage使用
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback,this));
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,origin.y+closeItem->getContentSize().height/2 ));
	// 测试滚动屏幕选择菜单事件
	// 创建菜单
	_itemMenu = Menu::create();
	// 循环添加菜单项
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

	// 创建单点触碰监听器
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	// 设置触碰的回掉函数
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);

	return true ;
}
// 开始触碰事件
bool HelloWorld::onTouchBegan(cocos2d::Touch *touch , cocos2d::Event* event)
{
	// 记录触碰的坐标
	_beginPos = touch->getLocation();
	return true ;
}
// 移动事件
void HelloWorld::onTouchMoved(cocos2d::Touch *touch , cocos2d::Event *event)
{
	Size visibileSize = Director::getInstance()->getVisibleSize(); // 获取可见区域尺寸
	auto touchLocation = touch->getLocation();
	float nextMoveY = touchLocation.y - _beginPos.y ; // 因为是上下移动，用当前触碰坐标 - 之前记录 获取偏移量
	// 获取当前菜单的位置
	auto curPos = _itemMenu->getPosition();
	// 计算出下一个位置
	auto nexPos = Vec2(curPos.x , curPos.y +nextMoveY);

	if(nexPos.y < 0.0f)
	{
		_itemMenu->setPosition(Vec2::ZERO);
		return ;
	}
	// 如果nextpos的y大于菜单的总高度 - 屏幕高度，即滚动到最后
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
	// 切换场景
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

