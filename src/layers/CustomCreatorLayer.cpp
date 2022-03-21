#include "CustomCreatorLayer.h"

using namespace gd;
using namespace cocos2d;

CustomCreatorLayer* CustomCreatorLayer::create() {
    auto ret = new CustomCreatorLayer();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool CustomCreatorLayer::init() {
    //big part of the code here is stolen from mat's gd mod example because i couldn't be bothered
    //auto backgroundSprite = CCSprite::create("GJ_gradientBG.png");
    /*const int backgrounds[] = {14, 15, 17, 18};
    std::srand(std::time(nullptr));
    int bgNumber = std::rand() % 4;
    std::ostringstream bgStream;
    bgStream << "game_bg_" << backgrounds[bgNumber] << "_001.png";*/

    auto backgroundSprite = CCSprite::create("game_bg_14_001.png");
    //auto backgroundSprite = CCSprite::create(bgStream.str().c_str());
    //auto backgroundSprite = CCSprite::createWithSpriteFrameName("GJ_GameSheet04-uhd.png");
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto size = backgroundSprite->getContentSize();
    
    backgroundSprite->setScale(winSize.width / size.width);
    //backgroundSprite->setScaleY(winSize.height / size.height);
    
    backgroundSprite->setAnchorPoint({0, 0});
    backgroundSprite->setPosition({0,-75});
    
    backgroundSprite->setColor({164, 0, 255});
    
    backgroundSprite->setZOrder(-2);
    addChild(backgroundSprite);

    auto backBtn = gd::CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"),
        this,
        menu_selector(CustomCreatorLayer::onBack)
    );

    auto menuBack = CCMenu::create();
    menuBack->addChild(backBtn);
    menuBack->setPosition({25, winSize.height - 25});

    addChild(menuBack);

    setKeypadEnabled(true);

    auto menu = CCMenu::create();
    //menu->setPosition({winSize.width / 2, winSize.height / 2});
    addChild(menu);
    auto label = CCLabelBMFont::create("Epic GMD Mod :D", "bigFont.fnt");

    label->setPosition({winSize.width / 2, winSize.height - 25});

    addChild(label);

    auto featuredBtn = gd::CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_featuredBtn_001.png"),
        this,
        menu_selector(CustomCreatorLayer::onFeatured)
    );
    menu->addChild(featuredBtn);
    featuredBtn->setPosition({-60,0});
    featuredBtn->setSizeMult(1.2f);

    //CCTextureCache::sharedTextureCache()->addImage("mostliked.png", 0);

    auto mostLikedBtn = gd::CCMenuItemSpriteExtra::create(
        CCSprite::create("mostliked.png"),
        this,
        menu_selector(CustomCreatorLayer::onMostLiked)
    );
    menu->addChild(mostLikedBtn);
    mostLikedBtn->setPosition({60,-2.5f}); //i have no idea why the texture is misaligned theyre the same res
    mostLikedBtn->setSizeMult(1.2f);

    auto cornerBL = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    cornerBL->setPosition({0,0});
    cornerBL->setAnchorPoint({0,0});
    addChild(cornerBL, -1);

    auto cornerUL = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    cornerUL->setPosition({0,winSize.height});
    cornerUL->setAnchorPoint({0,0});
    cornerUL->setRotation(90);
    addChild(cornerUL, -1);

    auto cornerUR = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    cornerUR->setPosition({winSize.width,winSize.height});
    cornerUR->setAnchorPoint({0,0});
    cornerUR->setRotation(180);
    addChild(cornerUR, -1);

    auto cornerBR = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    cornerBR->setPosition({winSize.width,0});
    cornerBR->setAnchorPoint({0,0});
    cornerBR->setRotation(270);
    addChild(cornerBR, -1);

    return true;
}

void CustomCreatorLayer::keyBackClicked() {
    CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}


void CustomCreatorLayer::onBack(CCObject* object) {
    keyBackClicked();
}

void CustomCreatorLayer::onFeatured(CCObject* object) {
    auto searchObject = gd::GJSearchObject::create(gd::SearchType::kGJSearchTypeFeaturedGDW);
    auto browserLayer = LevelBrowserLayer::scene(searchObject);

    auto transitionFade = CCTransitionFade::create(0.5, browserLayer);

    CCDirector::sharedDirector()->pushScene(transitionFade);
}

void CustomCreatorLayer::onMostLiked(CCObject* object) {
    auto searchObject = gd::GJSearchObject::create(gd::SearchType::kGJSearchTypeLikedGDW);
    auto browserLayer = LevelBrowserLayer::scene(searchObject);

    auto transitionFade = CCTransitionFade::create(0.5, browserLayer);

    CCDirector::sharedDirector()->pushScene(transitionFade);
}

CCScene* CustomCreatorLayer::scene() {
    auto layer = CustomCreatorLayer::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}