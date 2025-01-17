//
//  Fruit.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/18/21.
//

#include "Fruit.h"

USING_NS_CC;

bool Fruit::init()
{
    if (!Sprite::initWithSpriteFrameName("fruit"))
    {
        return false;
    }
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setTag(0xf02);
    auto pb = PhysicsBody::createCircle(getContentSize().height / 2);
    pb->setDynamic(false);
    pb->setCategoryBitmask(0x01);
    pb->setContactTestBitmask(0x03);
    addComponent(pb);
    return true;
}

void Fruit::setDuration(float duration)
{
    auto fade = FadeTo::create(duration, 64);
    auto remove = CallFunc::create([&](){
        removeFruit();
    });
    runAction(Sequence::create(fade, remove, nullptr));
}

void Fruit::setParent( Node* parent)
{
    Node::setParent(parent);
    if (parent != nullptr) {
        auto size = parent->getContentSize();
        
        setPosition(Vec2(CCRANDOM_0_1() * (size.width - 128) + 64, CCRANDOM_0_1() * (size.height - 160) + 96));
    }
}


void Fruit::removeFruit(float dt)
{
    auto parent = getParent();
    if (parent != nullptr)
    {
        auto _emitter = ParticleSystemQuad::create("particles/ExplodingRing.plist");
        _emitter->retain();
        _emitter->setPosition(getPosition());
        _emitter->setAutoRemoveOnFinish(true);
        parent->addChild(_emitter, 10);

        removeFromParentAndCleanup(true);
    }
}
