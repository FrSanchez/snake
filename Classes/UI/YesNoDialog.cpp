//
//  YesNoDialog.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/10/21.
//

#include "YesNoDialog.h"
#include "Config.h"

USING_NS_CC;

YesNoDialog::YesNoDialog()
{
    
}

YesNoDialog* YesNoDialog::createWithTitle(const std::string &title, const std::string &text, const cocos2d::ccMenuCallback &onYes, const cocos2d::ccMenuCallback &onNo)
{
    YesNoDialog *pRet = new(std::nothrow) YesNoDialog();
        if (pRet && pRet->initWithTitle(title, text, onYes, onNo))
        {
            pRet->autorelease();
            return pRet;
        }
        else
        {
            delete pRet;
            pRet = nullptr;
            return nullptr;
        }
}
bool YesNoDialog::initWithTitle(const std::string &title, const std::string &text, const cocos2d::ccMenuCallback &onYes, const cocos2d::ccMenuCallback &onNo)
{
    if(!PopupNode::init())
    {
        return false;
    }
    if (title.length() > 0) {
        auto label = Label::createWithTTF(title, "Stick-Regular.ttf", 38);
        label->setPosition(Vec2(getContentSize().width /2, getContentSize().height - 32));
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        addChild(label);
    }
    if (text.length() > 0) {
        TTFConfig ttfConfig("Stick-Regular.ttf", 28);
        auto label = Label::createWithTTF(ttfConfig, text, TextHAlignment::CENTER, getContentSize().width * 0.8);
        label->setPosition(Vec2(getContentSize().width / 2, getContentSize().height * 2 / 3));
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        addChild(label);
    }
    
    menu = Menu::create();
    menu->setPosition(Vec2(getContentSize().width / 2, 64));
    menu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(menu);
    
    addButton("Yes", 30, [&, onYes](Ref* p) {
        if (onYes != nullptr) {
            onYes(((Node*)p)->getParent()->getParent());
        }
        ClosePopup();
    });
    addButton("No", 30,  [&, onNo](Ref* p) {
        if (onNo != nullptr) {
            onNo(((Node*)p)->getParent()->getParent());
        }
        ClosePopup();
    });
    
    auto checkbox = cocos2d::ui::CheckBox::create("check_box_normal",
                                                  "check_box_normal_press",
                                                  "check_box_active",
                                                  "check_box_normal_disable",
                                                  "check_box_active_disable", cocos2d::ui::AbstractCheckButton::TextureResType::PLIST);
    checkbox->setSelected(true);

    auto label = Label::createWithTTF("Don't show this again", "Stick-Regular.ttf", 26);
    label->setPosition(Vec2(getContentSize().width / 2 + checkbox->getContentSize().width / 2, getContentSize().height / 3));
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(label);
    
    checkbox->setPosition(Vec2(-checkbox->getContentSize().width, checkbox->getContentSize().height / 2 - 6));
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label->addChild(checkbox);

    checkbox->addEventListener([=](Ref* ref, cocos2d::ui::CheckBox::EventType event){
        if (event == cocos2d::ui::CheckBox::EventType::SELECTED) {
            Config::getInstance()->setShowPurchase(true);
            log("Enabled");
        }else{
            Config::getInstance()->setShowPurchase(false);
            log("Disabled");
        }
    });

    return true;
}

void YesNoDialog::addButton(const std::string& text, float fontSize, const ccMenuCallback &callback)
{
    auto label = Label::createWithTTF(text, "Stick-Regular.ttf", fontSize);
    label->setAnchorPoint(Vec2(0.5, 0.5));
    label->setTextColor(Color4B::WHITE);
    label->enableOutline(Color4B::GRAY, 1);
    
    auto item = MenuItemLabel::create(label, callback);
    item->setTag((int)menu->getChildrenCount());
    menu->addChild(item);
    menu->alignItemsHorizontallyWithPadding(20);
}
