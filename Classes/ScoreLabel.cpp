//
//  ScoreLabel.cpp
//  cocos2d
//
//  Created by Francisco Sanchez on 2/8/21.
//

#include "ScoreLabel.h"

USING_NS_CC;

Label* ScoreLabel::create()
{
    auto size = Director::getInstance()->getVisibleSize();
    return ScoreLabel::_create(Vec2( 64, size.height - 64), Vec2::ANCHOR_MIDDLE_LEFT, TextHAlignment::LEFT, "SCORE: 0");
}

Label* ScoreLabel::createHS()
{
    auto size = Director::getInstance()->getVisibleSize();
    auto label = ScoreLabel::_create(Vec2(size.width - 64, 96), Vec2::ANCHOR_MIDDLE_RIGHT, TextHAlignment::RIGHT, "HIGH SCORE: 0");
    return label;
}

Label* ScoreLabel::_create(Vec2 pos, Vec2 anchorPoint, TextHAlignment alignment, const std::string& text)
{
    auto size = Director::getInstance()->getVisibleSize();
    auto label = Label::createWithTTF(text, "fonts/Arcade.ttf", 64, Size::ZERO, alignment);
    label->setAnchorPoint(anchorPoint);
    label->setPosition(pos);
    auto blue = Color4B(88, 157, 214, 255);
    label->setTextColor(blue);
    return label;
}
