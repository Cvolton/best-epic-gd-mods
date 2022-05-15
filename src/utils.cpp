#include <cocos2d.h>
#include <gd.h>
#include "utils.hpp"

#include <array>

using namespace cocos2d;
using namespace gd;

CCSprite* BetterInfo::createWithBISpriteFrameName(const char* name){
        auto sprite = CCSprite::createWithSpriteFrameName(name);
        if(sprite) return sprite;

        //idea stolen from fod but i made it better
        sprite = CCSprite::create();

        auto subSprite1 = CCSprite::create("square02b_001.png");
        auto subSprite2 = CCSprite::create("square02b_001.png");
        auto subSprite3 = CCSprite::create("square02b_001.png");
        auto subSprite4 = CCSprite::create("square02b_001.png");

        std::array<CCSprite*, 4> sprites = {subSprite1, subSprite2, subSprite3, subSprite4};

        subSprite1->setAnchorPoint({0,0});
        subSprite2->setAnchorPoint({1,0});
        subSprite3->setAnchorPoint({0,1});
        subSprite4->setAnchorPoint({1,1});

        subSprite2->setColor({251,64,249});
        subSprite1->setColor({0,0,0});
        subSprite3->setColor({251,64,249});
        subSprite4->setColor({0,0,0});

        for(auto subSprite : sprites){
                subSprite->setScale(.25f);
                subSprite->setPosition(subSprite->getContentSize() / 2);
                sprite->addChild(subSprite);

        }

        sprite->setContentSize(subSprite1->getContentSize());

        /*auto drawNode = CCDrawNode::create();

        CCPoint rectangle1[] = {CCPoint(10, 10), CCPoint(10, 0), CCPoint(0, 0), CCPoint(0, 10)};

        drawNode->drawPolygon(rectangle1, 4, {0,0,0,255}, 0, {0,0,0,0});
        sprite->addChild(drawNode);*/

        /*void drawPolygon        (       CCPoint *       verts,
                unsigned int    count,
                const ccColor4F &       fillColor,
                float   borderWidth,
                const ccColor4F &       borderColor 
        )       
        //return ButtonSprite::create("Missing\ntexture", 20, true, "bigFont.fnt", "GJ_button_01.png", 30, 0.5);
        //auto font = cocos2d::CCLabelBMFont::create("Missing\ntexture", "bigFont.fnt");
        //font->setScale(.3f);*/

        return sprite;
}