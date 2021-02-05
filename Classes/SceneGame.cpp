//
//  SceneGame.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 2/2/21.
//

#include "SceneGame.h"
#include "SceneMenu.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;

Scene* SceneGame::createScene()
{
    return SceneGame::create();
}


// on "init" you need to initialize your instance
bool SceneGame::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    srand(time(NULL));
#endif
    
    auto size = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto map = TMXTiledMap::create("ground.tmx");
    addChild(map, 0, 1);
    map->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    map->setPosition(Vec2(0.5 * size.width, 0.5 * size.height));
    layer = map->getLayer("layer0");
    auto s = layer->getLayerSize();
    tileSize = map->getTileSize();
    
    SpriteBatchNode* child = nullptr;
    auto& children = map->getChildren();
    
    for(const auto &node : children) {
        child = static_cast<SpriteBatchNode*>(node);
        child->getTexture()->setAntiAliasTexParameters();
    }
    
    layer->setOpacity(128);
    
    snake = Snake(s.width, s.height);
    snake.setLayer(layer);

    float offsetX = map->getPosition().x - map->getContentSize().width / 2;
    float offsetY = map->getPosition().y - map->getContentSize().height / 2;
    corner = Vec2(offsetX, offsetY);
    
    
    apple = Sprite::createWithSpriteFrameName("fruit.png");
    apple->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    if (apple == nullptr)
    {
        log("Error loading circles.png");
        return false;
    }
    apple->setVisible(false);
    this->addChild(apple, 0);

    scoreLabel = Label::createWithTTF("SCORE: 0", "fonts/Arcade.ttf", 48);
    scoreLabel->setPosition(Vec2(size.width / 2, size.height - scoreLabel->getContentSize().height / 2));
    scoreLabel->setAlignment(TextHAlignment::CENTER);
    scoreLabel->setTextColor( Color4B::RED );
    scoreLabel->enableOutline(Color4B::YELLOW,1);
    scoreLabel->enableShadow(Color4B::GREEN);
    addChild(scoreLabel, 10);
        
    initBody();
    
    // creating a keyboard event listener
    auto kbdLstnr = EventListenerKeyboard::create();
    kbdLstnr->onKeyPressed = CC_CALLBACK_2(SceneGame::onKeyPressed, this);
    kbdLstnr->onKeyReleased = CC_CALLBACK_2(SceneGame::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(kbdLstnr, this);
    
    leftButton = Sprite::createWithSpriteFrameName("shadedDark24.png");
    leftButton->setVisible(false);
    leftButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(leftButton);
    rightButton = Sprite::createWithSpriteFrameName("shadedDark25.png");
    rightButton->setVisible(false);
    rightButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(rightButton);
    
    if (leftButton == nullptr || rightButton == nullptr) {
        log("Error loading left or right buttons");
        return false;
    }
    
    // Register Touch Event
    auto touchLsnr = EventListenerTouchOneByOne::create();
    touchLsnr->setSwallowTouches(true);

    touchLsnr->onTouchBegan = CC_CALLBACK_2(SceneGame::onTouchBegan, this);
    touchLsnr->onTouchMoved = CC_CALLBACK_2(SceneGame::onTouchMoved, this);
    touchLsnr->onTouchEnded = CC_CALLBACK_2(SceneGame::onTouchEnded, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchLsnr, this);

    this->schedule(CC_SCHEDULE_SELECTOR(SceneGame::updateTimer), snakeSpeed, 0, 0);
    lastFood = arc4random() % 10 + 5;

    _audioId = AudioEngine::play2d("Arcade-Puzzler.mp3", true, 0.7f);
    
    if(_audioId != AudioEngine::INVALID_AUDIO_ID) {
        log("Can't play background music");
    }

    return true;
}

void SceneGame::labelCallback(Label* label)
{
    label->setVisible(false);
}


bool SceneGame::onTouchBegan(Touch* touch, Event* event)
{
    CCLOG("SceneGame::onTouchBegan id = %d, x = %f, y = %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
    
    auto f = CallFunc::create([=]() {
        
    });
    
    Sprite* button;
    auto size = Director::getInstance()->getVisibleSize();
    if (touch->getLocation().x < size.width / 2) {
        // left
        snake.turnLeft();
        button = leftButton;
    } else {
        // right
        snake.turnRight();
        button = rightButton;
    }
    button->setVisible(true);
    auto action = Sequence::create(Place::create(touch->getLocation()),
                                   FadeOut::create(0.5f),
                                   Place::create(Vec2(-tileSize.width,-tileSize.height)),
                                   FadeIn::create(0.1f),
                                   nullptr);
    button->runAction(action);

}

bool SceneGame::onTouchMoved(Touch* touch, Event* event)
{
    CCLOG("Paddle::onTouchMoved id = %d, x = %f, y = %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
}

bool SceneGame::onTouchEnded(Touch* touch, Event* event)
{
    auto p0 = touch->getStartLocation();
    auto pf = touch->getDelta();
    CCLOG("SceneGame::onTouchEnded id = %d, x = %f, y = %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
}


// Implementation of the keyboard event callback function prototype
void SceneGame::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
}

void SceneGame::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
        snake.turnLeft();
    }
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
        snake.turnRight();
    }
}

void SceneGame::eat()
{
//    apple->setVisible(false);
    auto action = Spawn::createWithTwoActions(ScaleTo::create(0.5f, 0.5f, 0.5f),
                                              MoveTo::create(0.5f, scoreLabel->getPosition()));
    auto seq = Sequence::create(action,
                                CallFunc::create([&]() {
        auto score_str = StringUtils::format("SCORE: %d", score);
        scoreLabel->setString(score_str);
    }),
                                Hide::create(),
                                ScaleTo::create(0.1f, 1.0f, 1.0f),
                                MoveTo::create(0.1f, Vec2(-64, -64)),
                                Show::create(), nullptr);
    apple->runAction(seq);
    lastFood = arc4random() % 10 + 1;
    food = Vec2::ZERO;
    snake.grow();
    auto sprite = Sprite::createWithSpriteFrameName("body.png");
    sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    auto i = snake.getLength() - 1;
    sprite->setPosition((snake.getPosAt(i) * tileSize.width) + corner);
    body.push_back(sprite);
    this->addChild(sprite, 0);
    snakeSpeed *= 0.95f;
    score++;
    AudioEngine::play2d("munch.wav", false, 1.0f);
}

void SceneGame::update(float delta) {
    log("update %f", delta);
}

void SceneGame::collide()
{
    auto total = body.size();
       for(auto i=1; i < total; i++)
    {
        body.at(i)->runAction(Blink::create(2, 10));
    }
    if (_audioId != AudioEngine::INVALID_AUDIO_ID) {
        AudioEngine::stop(_audioId);
        _audioId = AudioEngine::INVALID_AUDIO_ID;
    }
    AudioEngine::play2d("crash.wav", false, 1.0f);

    if (apple != nullptr && food != Vec2::ZERO) {
        auto fadeOut = FadeOut::create(1.0f);
        apple->runAction(fadeOut);
    }
    auto end = CallFunc::create([=]() {
        closeScene(this);
    });
    auto fadeOut = FadeOut::create(1.0f);
    auto seq = Sequence::create(fadeOut, end, nullptr);
    body.at(0)->runAction(seq);
                  
}

void SceneGame::updateTimer(float dt)
{
    if (lastFood)
        lastFood--;
    if (snake.getPosAt(0) == food) {
        eat();
    }
    if (!snake.advance()) {
        collide();
        return;
    }
    for(auto i = 0; i < snake.getLength(); i++) {
        auto moveTo = MoveTo::create(snakeSpeed, corner + (snake.getPosAt(i) * tileSize.width));
        body.at(i)->runAction(moveTo);
    }
    if (lastFood <= 0 && food == Vec2::ZERO) {
        addFood();
    }
    auto pos = snake.getPosAt(0);
    log("lastFood %d speed: %.3f", lastFood, snakeSpeed);
    this->schedule(CC_SCHEDULE_SELECTOR(SceneGame::updateTimer), snakeSpeed, 0, 0);
}

void SceneGame::addFood()
{
    uint32_t gid;
    do {
        food = Vec2(arc4random() % snake.getWidth() , arc4random() % snake.getHeight() );
        gid = layer->getTileGIDAt(food);
    } while(gid != 5);
    
    auto fadeIn = FadeIn::create(0.5f);
    apple->stopAllActions();
    apple->setPosition((food * tileSize.width) + corner);
    apple->setScale(0.1f);
    apple->setVisible(true);
    apple->runAction(
                     Spawn::createWithTwoActions(ScaleTo::create(0.5f, 1, 1), fadeIn)
                     );
}

void SceneGame::initBody()
{
    auto total = snake.getLength();
       for(auto i=0; i < total; i++)
       {
           Sprite* sprite;
           if (i == 0) {
               sprite = Sprite::createWithSpriteFrameName("head.png");
           } else {
               sprite = Sprite::createWithSpriteFrameName("body.png");
           }
           sprite->setPosition(snake.getPosAt(i) * tileSize.width);
           sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
           body.push_back(sprite);
           this->addChild(sprite, 0);
       }
}

void SceneGame::closeScene(Ref* pSender)
{

    SceneMenu* scene = static_cast<SceneMenu*> (SceneMenu::createScene());
    scene->setScore(score);
    auto fade = TransitionFade::create(1, scene);
    Director::getInstance()->replaceScene(fade);
}
