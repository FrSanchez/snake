//
//  Score.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 2/11/21.
//

#include "Score.h"
#include "cocos2d.h"
#include <string.h>

USING_NS_CC;

Score::Score()
{
    _maxLevel = UserDefault::getInstance()->getIntegerForKey("maxLevel", 1);
    _level = _maxLevel;
    CCLOG("maxLevel: %d", _maxLevel);
}

void Score::reset()
{
    for(int i = 1; i <= _maxLevel; i++) {
        std::string key = StringUtils::format("level%d", i);
        UserDefault::getInstance()->deleteValueForKey(key.c_str());
    }
    _maxLevel = 1;
    UserDefault::getInstance()->setIntegerForKey("maxLevel", _maxLevel);
    UserDefault::getInstance()->flush();
}

void Score::loadLevel(int level)
{
    std::string key = StringUtils::format("level%d", level);
    Data data = UserDefault::getInstance()->getDataForKey(key.c_str());
    if (!_scoreByLevel[level].loaded)
    {
        memset(&_scoreByLevel[level], 0, sizeof(ScoreData));
        if (data.getSize() > 0) {
            memcpy(&_scoreByLevel[level], data.getBytes(), data.getSize());
            _scoreByLevel[level].loaded = true;
        }
        _scoreByLevel[level].points = 0;
        CCLOG("loading %s, max: %d, accuracy: %f", key.c_str(), _scoreByLevel[level].max, _scoreByLevel[level].accuracy);
    }
}

void Score::loadAllLevels()
{
    for(int i = 1; i <= _maxLevel; i++)
    {
        loadLevel(i);
    }
}

void Score::setScore(int score)
{
    setScoreByLevel(_level, score);
}

void Score::addScore(int level, int delta)
{
    float score = getScore(level);
    score += delta;
    setScoreByLevel(level, score);
}

void Score::setScoreByLevel(int level, int score)
{
    _level = level;
    if (level > _maxLevel) {
        _maxLevel = level;
    }
    _scoreByLevel[level].points = score;
    if (score > _scoreByLevel[level].max)
    {
        _scoreByLevel[level].max = score;
    }
}

void Score::setAccuracy(int level, float value)
{
    setLevel(_level);
    if (value > _scoreByLevel[level].accuracy) {
        _scoreByLevel[level].accuracy = value;
    }
}

void Score::flush()
{
    std::string log = "";
   
    auto key = StringUtils::format("level%d", _level);
    int score = 0;
    if (_scoreByLevel.find(_level) != _scoreByLevel.end()) {
        score = _scoreByLevel[_level].max;
    }
    Data data;
    data.copy((unsigned char*) &_scoreByLevel[_level], sizeof(ScoreData));

    UserDefault::getInstance()->setDataForKey(key.c_str(), data);

    CCLOG("Flushing %s, max: %d, accuracy: %f", key.c_str(), _scoreByLevel[_level].max, _scoreByLevel[_level].accuracy);
    CCLOG("UserDefault flush");
    UserDefault::getInstance()->flush();
}

void Score::setLevel(int level)
{
    _level = level;
    if (_level > _maxLevel) {
        _maxLevel = level;
    }
    loadLevel(level);
}

int Score::getScore(int level)
{
    if (_scoreByLevel.find(level) != _scoreByLevel.end()) {
        return _scoreByLevel[level].points;
    }
    _scoreByLevel[level].points = 0;
    return 0;
}

int Score::getMaxScore(int level)
{
    if (_scoreByLevel.find(level) != _scoreByLevel.end()) {
        return _scoreByLevel[level].max;
    }
    _scoreByLevel[level].max = 0;
    return 0;
}

void Score::setMaxLevel(int maxLevel)
{
    if (maxLevel > _maxLevel)
    {
        _maxLevel = maxLevel;
        UserDefault::getInstance()->setIntegerForKey("maxLevel", _maxLevel);
        UserDefault::getInstance()->flush();
    }
}

float Score::getAccuracy(int level)
{
    return getScoreData(level).accuracy;
}

Score::ScoreData Score::getScoreData(int level)
{
    setMaxLevel(_maxLevel);
    return _scoreByLevel[level];
}

void Score::setFoodEat(int level, int value)
{
    setLevel(level);
    _scoreByLevel[level].foodEat = value;
}
void Score::setTarget(int level, int value)
{
    setLevel(level);
    _scoreByLevel[level].target = value;
}

void Score::setSnakeLength(int level, int value)
{
    setLevel(level);
    _scoreByLevel[level].snakeLength = value;
}

void Score::setTime(int level, float value)
{
    setLevel(level);
    _scoreByLevel[level].time = value;
}

unsigned char Score::getStars(int level)
{
    setLevel(level);
    return _scoreByLevel[level].stars;
}

void Score::setStars(int level, unsigned char mask)
{
    setLevel(level);
    auto stars = _scoreByLevel[level].stars;
    stars |= mask;
    if (stars != _scoreByLevel[level].stars)
    {
        _scoreByLevel[level].earnedStars++;
    }
    _scoreByLevel[level].stars = stars;
}

void Score::startLevel(int level)
{
    setLevel(level);
    _scoreByLevel[level].earnedStars = 0;
    _scoreByLevel[level].points = 0;
}
