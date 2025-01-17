//
//  DPad.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 2/8/21.
//

#include "DPad.h"

USING_NS_CC;

DPad::DPad()
{
    auto bg = cocos2d::LayerColor::create(Color4B(53, 53, 53, 255));
//    addChild(bg);
    auto upNormal = Sprite::createWithSpriteFrameName("UpNormal");
    auto upSelect =  Sprite::createWithSpriteFrameName("UpSelected");
    auto downNormal =  Sprite::createWithSpriteFrameName("DownNormal");
    auto downSelect =  Sprite::createWithSpriteFrameName("DownSelected");
    auto leftNormal =  Sprite::createWithSpriteFrameName("LeftNormal");
    auto leftSelect =  Sprite::createWithSpriteFrameName("LeftSelected");
    auto rightNormal =  Sprite::createWithSpriteFrameName("RightNormal");
    auto rightSelect =  Sprite::createWithSpriteFrameName("RightSelected");
                                                       
    auto upItem = MenuItemSprite::create(upNormal, upSelect, CC_CALLBACK_1(DPad::dpadCallback, this));
    upItem->setTag(1);
    auto downItem = MenuItemSprite::create(downNormal, downSelect, CC_CALLBACK_1(DPad::dpadCallback, this));
    downItem->setTag(2);
    auto leftItem = MenuItemSprite::create(leftNormal, leftSelect, CC_CALLBACK_1(DPad::dpadCallback, this));
    leftItem->setTag(4);
    auto rightItem = MenuItemSprite::create(rightNormal, rightSelect, CC_CALLBACK_1(DPad::dpadCallback, this));
    rightItem->setTag(8);
    
    auto menu = Menu::create( upItem, downItem, leftItem, rightItem, nullptr);
    
    setContentSize( Size(20 + upNormal->getContentSize().width + leftNormal->getContentSize().width + rightNormal->getContentSize().width, 20 +  upNormal->getContentSize().height + downNormal->getContentSize().height));

    menu->setPosition(Vec2(_contentSize.width, _contentSize.height));
    leftItem->setPosition(-leftItem->getContentSize().width - 20, 0);
    rightItem->setPosition(rightItem->getContentSize().width + 20, 0);
    upItem->setPosition(0, upItem->getContentSize().height);
    downItem->setPosition(0, -downItem->getContentSize().height );
    
    addChild(menu, 10 ,1 );
    autorelease();
}

void DPad::setPosition(const cocos2d::Vec2 &position)
{
    Node::setPosition(position);
}

void DPad::dpadCallback(Ref* sender)
{
    int dir =static_cast<cocos2d::MenuItem*>(sender)->getTag();
    if (_callback != nullptr) {
        _callback(dir);
    }
}

void DPad::setCallback(const padCallback& callback)
{
    _callback = callback;
}
