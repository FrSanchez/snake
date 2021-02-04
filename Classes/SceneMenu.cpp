//
//  SceneMenu.cpp
//  cocos2d
//
//  Created by Francisco Sanchez on 2/2/21.
//

#include "SceneMenu.h"
#include "SceneGame.h"
#include "audio/include/AudioEngine.h"


USING_NS_CC;

Scene* SceneMenu::createScene()
{
    return SceneMenu::create();
}

void SceneMenu::startGame()
{
    if (_audioID != AudioEngine::INVALID_AUDIO_ID) {
        AudioEngine::stop(_audioID);
        _audioID = AudioEngine::INVALID_AUDIO_ID;
    }
    
    auto audioId = AudioEngine::play2d("select_001.ogg", false, 1.0f);
    if (audioId == AudioEngine::INVALID_AUDIO_ID) {
        log("Failed to play select_001.ogg");
    }
    auto scene = SceneGame::createScene();
    auto fade = TransitionFade::create(2, scene);
    Director::getInstance()->replaceScene(fade);
}

// on "init" you need to initialize your instance
bool SceneMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    auto size = Director::getInstance()->getVisibleSize();;
    
    auto bg = Sprite::create("background.png");
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bg->setPosition(Vec2(0.5 * size.width, 0.5 * size.height));
    this->addChild(bg);
    
    label = Label::createWithTTF("Play", "fonts/Arcade.ttf", 180);
    label->setAnchorPoint(Vec2(0.5, 0.5));
    label->setPosition(Vec2(0.5 * size.width, 0.5 * size.height));
    auto black = Color4B( Color4B::BLACK);
    auto grey = Color4B(128, 128, 128, 128);
    auto blue = Color4B(88, 157, 214, 255);
    label->setTextColor(blue);
    label->enableShadow(grey);
    this->addChild(label);
    
    auto normal = Sprite::createWithSpriteFrameName("CloseNormal.png");
    auto select = Sprite::createWithSpriteFrameName("CloseSelected.png");
    auto closeItem = MenuItemSprite::create(normal, select, [=](Ref *pSender)  { Director::getInstance()->end();});
    
//    auto closeItem = MenuItemImage::create(
//                                           "CloseNormal.png",
//                                           "CloseSelected.png",
//                                           [=](Ref *pSender)  { Director::getInstance()->end();
//        } );

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        log("'CloseNormal.png' and 'CloseSelected.png'");
        return false;
    }
    else
    {
        float x = size.width - closeItem->getContentSize().width/2;
        float y = closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    _dt = 0;
    schedule([&](float time){
        _dt += time;
        if (_dt > 1.0) {
            _dt -= 1.0;
        }
        _magnitude = _dt * _extent;
        auto offset = _direction;
        offset.x *= _magnitude;
        offset.y *= _magnitude;
        grey.a = (uint8_t) (128.0 * _dt);
        label->enableShadow(grey, Size(offset.x, offset.y), 50);
    }, "shadow");
        
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
        log("Key with keycode %d pressed", keyCode);
        startGame();
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [=](Touch* touch, Event* event){
        startGame();
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    _audioID = AudioEngine::play2d("Bonkers-for-Arcades.mp3", true, 0.7f);
    
    if(_audioID != AudioEngine::INVALID_AUDIO_ID) {
        log("Can't play background music");
    }
    
    return true;
}

// Implementation of the keyboard event callback function prototype
void SceneMenu::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
}

void SceneMenu::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
}