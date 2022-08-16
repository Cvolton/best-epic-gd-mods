#include <cocos2d.h>
#include <gd.h>
#include <fstream>
#include "utils.hpp"
#include "managers/CvoltonManager.h"

#include "support/base64.h"
#include "support/zip_support/ZipUtils.h"

#include <array>

using namespace cocos2d;
using namespace gd;

constexpr int levelsPerPageLow = 10;
constexpr int levelsPerPageHigh = 20;

CCSprite* BetterInfo::createWithBISpriteFrameName(const char* name){
        auto sprite = CCSprite::createWithSpriteFrameName(name);
        if(sprite) return sprite;

        return createPlaceholder();
}

CCSprite* BetterInfo::createBISprite(const char* name){
        auto sprite = CCSprite::create(name);
        if(sprite) return sprite;

        return createPlaceholder();
}

CCSprite* BetterInfo::createPlaceholder(){
        auto sprite = CCSprite::create();

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

        return sprite;
}

bool BetterInfo::isLocal(GJSearchObject* searchObject){
        return searchObject->m_nScreenID == SearchType::kSearchTypeMyLevels 
                || searchObject->m_nScreenID == SearchType::kSearchTypeSavedLevels 
                || searchObject->m_nScreenID == SearchType::kSearchTypeFavorite;
}

bool BetterInfo::isFalseTotal(GJSearchObject* searchObject){
        return searchObject->m_nScreenID == SearchType::kSearchType19
                || searchObject->m_nScreenID == SearchType::kSearchTypeFeatured
                || searchObject->m_nScreenID == SearchType::kSearchTypeHallOfFame;
}

bool BetterInfo::isStarUseless(GJSearchObject* searchObject){
        return searchObject->m_nScreenID == SearchType::kSearchTypeFeatured
                || searchObject->m_nScreenID == SearchType::kSearchTypeMagic
                || searchObject->m_nScreenID == SearchType::kSearchTypeMapPacks
                || searchObject->m_nScreenID == SearchType::kSearchTypeMapPackList
                || searchObject->m_nScreenID == SearchType::kSearchTypeAwarded
                || searchObject->m_nScreenID == SearchType::kSearchTypeFindUsers
                || searchObject->m_nScreenID == SearchType::kSearchTypeHallOfFame
                || searchObject->m_nScreenID == SearchType::kGJSearchTypeFeaturedGDW
                || searchObject->m_nScreenID == SearchType::kSearchTypeSimilar
                || searchObject->m_nScreenID == SearchType::kSearchTypeDaily
                || searchObject->m_nScreenID == SearchType::kSearchTypeWeekly
                || isLocal(searchObject);
}

bool BetterInfo::isAdvancedEnabled(GJSearchObject* searchObject) {
        return searchObject->m_nScreenID == SearchType::kSearchTypeSearch
                || searchObject->m_nScreenID == SearchType::kSearchTypeMostDownloaded
                || searchObject->m_nScreenID == SearchType::kSearchTypeMostLiked
                || searchObject->m_nScreenID == SearchType::kSearchTypeTrending
                || searchObject->m_nScreenID == SearchType::kSearchTypeRecent
                || searchObject->m_nScreenID == SearchType::kSearchTypeAwarded
                || searchObject->m_nScreenID == SearchType::kSearchTypeFollowed
                || searchObject->m_nScreenID == SearchType::kSearchTypeFriends;
}

int BetterInfo::levelsPerPage(GJSearchObject* searchObject){
        return ((isLocal(searchObject) && GameManager::sharedState()->getGameVariable("0093")) ? levelsPerPageHigh : levelsPerPageLow);
}

const char* BetterInfo::rankIcon(int position){
        if (position == 1) return "rankIcon_1_001.png";
        else if (position > 1000 || position <= 0) return "rankIcon_all_001.png";
        else if (position <= 10) return "rankIcon_top10_001.png";
        else if (position <= 50) return "rankIcon_top50_001.png";
        else if (position <= 100) return "rankIcon_top100_001.png";
        else if (position <= 200) return "rankIcon_top200_001.png";
        else if (position <= 500) return "rankIcon_top500_001.png";
        return "rankIcon_top1000_001.png";
}

void BetterInfo::showBIExclamationMark(CCLayer* creator){
    return;

    //auto menu = dynamic_cast<CCMenu*>(creator->getChildren()->objectAtIndex(1));
    //if(menu == nullptr) dynamic_cast<CCMenu*>(creator->getChildren()->objectAtIndex(5));
    auto CM = CvoltonManager::sharedState();
    CCMenu* menu = nullptr;

    for(unsigned int i = 0; i < creator->getChildrenCount(); i++){
        menu = dynamic_cast<CCMenu*>(creator->getChildren()->objectAtIndex(i));
        if(menu != nullptr && menu->getChildrenCount() > 5) break;
    }

    if(menu == nullptr || CM->isUpToDate()) return;

    auto mainBtn = reinterpret_cast<CCMenuItemSpriteExtra*>(menu->getChildByTag(mainBtnTag));
    if(mainBtn == nullptr) return;

    auto existingNode = mainBtn->getChildByTag(mainBtnExMarkTag);
    if(existingNode != nullptr){
        existingNode->setVisible(true);
        return;
    }

    auto exMark = CCSprite::createWithSpriteFrameName("exMark_001.png");
    exMark->setPosition({14,40});
    exMark->setScale(0.5f);
    exMark->setTag(mainBtnExMarkTag);
    mainBtn->addChild(exMark);
}

/*
        Windows only code :D
*/
void BetterInfo::copyToClipboard(const char* text){
        const size_t len = strlen(text) + 1;
        HGLOBAL hMem =  GlobalAlloc(GMEM_MOVEABLE, len);
        memcpy(GlobalLock(hMem), text, len);
        GlobalUnlock(hMem);
        OpenClipboard(0);
        EmptyClipboard();
        SetClipboardData(CF_TEXT, hMem);
        CloseClipboard();
}

void BetterInfo::copyToClipboard(const char* text, CCLayer* parent){
        copyToClipboard(text);

        parent->addChild(TextAlertPopup::create("Copied to clipboard", 0.5f, 0.6f), 100);
}

CCMenuItemSpriteExtra* BetterInfo::createTextButton(CCLayer* parent, const char* text, cocos2d::SEL_MenuHandler handler, int width, float height, float scale){
    auto buttonSprite = gd::ButtonSprite::create(text, width, true, "bigFont.fnt", "GJ_button_01.png", height, scale);
    auto buttonButton = gd::CCMenuItemSpriteExtra::create(
        buttonSprite,
        parent,
        handler
    );
    buttonButton->setSizeMult(1.2f);

    return buttonButton;
}

std::string BetterInfo::decodeBase64Gzip(const std::string& input) {
        unsigned char* levelString;
        unsigned char* levelStringFull;
        int levelStringSize = base64Decode((unsigned char *)(input.c_str()), input.size(), &levelString);
        int levelStringSizeDeflated = ZipUtils::ccInflateMemory(levelString, levelStringSize, &levelStringFull);

        std::string levelStringFullStd((char*)levelStringFull, levelStringSizeDeflated);

        delete levelString;
        delete levelStringFull;

        return levelStringFullStd;
}

std::string BetterInfo::fileSize(size_t bytes) {
        std::stringstream size;
        size << std::setprecision(4);

        if(bytes > (1024*1024)) size << ( bytes / (float)(1024*1024) ) << "MB";
        else if(bytes > (1024)) size << ( bytes / (float)(1024) ) << "KB";
        else size << bytes << "B";

        return size.str();
}

std::string BetterInfo::fixColorCrashes(std::string input) {
        int tags = 0;

        std::string openingTag = "<c";
        for(std::string::size_type pos = 0; (pos = input.find(openingTag, pos)) != std::string::npos; tags++) {
                pos += openingTag.length();
        }

        std::string closingTag = "</c>";
        for(std::string::size_type pos = 0; (pos = input.find(closingTag, pos)) != std::string::npos; tags--) {
                pos += closingTag.length();
        }

        for(int i = 0; i < tags; i++) {
                input.append("  </c>");
        }

        return input;
}

std::string BetterInfo::timeToString(time_t input) {
        struct tm time3;
        localtime_s(&time3, &input);
        return std::format("{}-{:02}-{:02} {:02}:{:02}", time3.tm_year + 1900, time3.tm_mon + 1, time3.tm_mday, time3.tm_hour, time3.tm_min);
}

bool BetterInfo::isSavedFiltered() {
        auto CM = CvoltonManager::sharedState();

        for(unsigned int i = 0; i <= 4; i++){
                if(
                        CM->getOption(
                                CCString::createWithFormat("user_search_len_%02u", i)->getCString()
                        )
                ) return true;
        }

        if(CM->getOption("user_search_diff_auto")) return true;

        for(int i = 0; i <= 6; i++){
                if(
                        CM->getOption(
                                CCString::createWithFormat("user_search_diff_%02i", i)->getCString()
                        )
                ) return true;
        }

        if(CM->getOption("user_search_diff_06"))
                for(int i = 0; i <= 5; i++){
                        if(
                                CM->getOption(
                                        CCString::createWithFormat("user_search_demon_%02i", i)->getCString()
                                )
                        ) return true;
                }

        const char* options[] = {
                "user_search_star", "user_search_uncompleted", "user_search_completed",
                "user_search_featured", "user_search_nofeatured", "user_search_original",
                "user_search_epic", "user_search_noepic", "user_search_song",
                "user_search_nostar", "user_search_coins", "user_search_twoplayer",
                "user_search_copied", "user_search_downloaded", "user_search_ldm",
                "user_search_copy", "user_search_copy_free", "user_search_idrange",
                "user_search_completedorbs", "user_search_completedleaderboard", "user_search_uncompletedorbs",
                "user_search_uncompletedleaderboard", "user_search_percentage", "user_search_percentageorbs",
                "user_search_percentageleaderboard", "user_search_starrange"
        };

        for(auto option : options) {
                if(CM->getOption(option)) return true;
        }

        return false;
}

void BetterInfo::debugObjectIndexes(CCNode* node) {
        auto menu = CCMenu::create();
        menu->setPosition({0, 0});
        menu->setZOrder(1000);
        for (size_t i = 0; i < node->getChildrenCount(); i++) {
                auto text = CCLabelBMFont::create(std::to_string(i).c_str(), "bigFont.fnt");
                auto child = static_cast<CCNode*>(node->getChildren()->objectAtIndex(i));
                text->setAnchorPoint(child->getAnchorPoint());
                text->setScale(0.5f);
                text->setPosition(child->getPosition());
                text->setZOrder(1000);
                menu->addChild(text);
        }
        node->addChild(menu);
}

bool BetterInfo::validateRangeItem(const BISearchObject::RangeItem& rangeItem, int value) {
        if(!rangeItem.enabled) return true;
        if(rangeItem.min != 0 && rangeItem.min > value) return false;
        if(rangeItem.max != 0 && rangeItem.max < value) return false;
        return true;
}

bool BetterInfo::levelMatchesObject(GJGameLevel* level, const BISearchObject& searchObj) {

        if(!searchObj.difficulty.empty() && searchObj.difficulty.find(levelDifficultyAsInt(level)) == searchObj.difficulty.end()) return false;
        if(!searchObj.length.empty() && searchObj.length.find(level->levelLength) == searchObj.length.end()) return false;
        if(!searchObj.demonDifficulty.empty() && level->demon != 0
                && searchObj.difficulty.find(6) != searchObj.difficulty.end()
                && searchObj.demonDifficulty.find(levelDemonDifficultyAsInt(level)) == searchObj.demonDifficulty.end()
                ) return false;

        std::string query = searchObj.str;
        std::string name = level->levelName;
        strToLower(query);
        strToLower(name);

        if(name.find(query) == std::string::npos) return false;


        if(searchObj.star && level->stars == 0) return false;
        if(searchObj.noStar && level->stars != 0) return false;
        if(searchObj.featured && level->featured <= 0) return false;
        if(searchObj.original && level->originalLevel > 0) return false;
        if(searchObj.twoPlayer && !level->twoPlayerMode) return false;
        if(searchObj.coins && (level->coins == 0 || level->coinsVerified == 0)) return false;
        if(searchObj.epic && !level->isEpic) return false;
        //TODO: searchObj.folder
        if(searchObj.song) {
                if(!searchObj.songCustom && level->audioTrack != searchObj.songID) return false;
                if(searchObj.songCustom && level->songID != searchObj.songID) return false;
        }
        if(searchObj.copied && level->originalLevel <= 0) return false;
        //TODO: searchObj.ldm
        if(!validateRangeItem(searchObj.idRange, level->levelID)) return false;
        //TODO: searchObj.copyable
        //TODO: searchObj.freeCopy
        if(searchObj.unfeatured && level->featured > 0) return false;
        if(searchObj.unepic && level->isEpic) return false;
        if(!validateRangeItem(searchObj.starRange, level->stars)) return false;
        if(!validateRangeItem(searchObj.gameVersion, level->gameVersion)) return false;

        auto levelFromSaved = static_cast<GJGameLevel*>(GameLevelManager::sharedState()->m_onlineLevels->objectForKey(std::to_string(level->levelID)));
        if(searchObj.uncompleted && (levelFromSaved && levelFromSaved->normalPercent == 100)) return false;
        if(searchObj.uncompletedOrbs && (!levelFromSaved || levelFromSaved->orbCompletion == 100)) return false;
        if(searchObj.uncompletedLeaderboard && (!levelFromSaved || levelFromSaved->newNormalPercent2 == 100)) return false;

        if(searchObj.completed && (!levelFromSaved || levelFromSaved->normalPercent != 100)) return false;
        if(searchObj.completedOrbs && (!levelFromSaved || levelFromSaved->orbCompletion != 100)) return false;
        if(searchObj.completedLeaderboard && (!levelFromSaved || levelFromSaved->newNormalPercent2 != 100)) return false;

        if(!validateRangeItem(searchObj.percentage, (levelFromSaved ? levelFromSaved->normalPercent : 0))) return false;
        if(!validateRangeItem(searchObj.percentageOrbs, (levelFromSaved ? levelFromSaved->orbCompletion : 0))) return false;
        if(!validateRangeItem(searchObj.percentageLeaderboard, (levelFromSaved ? levelFromSaved->newNormalPercent2 : 0))) return false;

        if(searchObj.downloaded && (!levelFromSaved || levelFromSaved->levelString.empty())) return false;

        return true;
}

void BetterInfo::writeToDebugFile(const std::string& content) {
        std::ofstream fileStream;
        fileStream.open("betterinfo/v2/debug.txt", std::fstream::out | std::fstream::app);
        fileStream << content << "\n";
        fileStream.close();
}

int BetterInfo::levelDifficultyAsInt(GJGameLevel* level) {
        if(level->demon != 0) return 6;
        if(level->autoLevel) return -1;
        return (level->ratings == 0) ? 0 : (level->ratingsSum / level->ratings);
}

int BetterInfo::levelDemonDifficultyAsInt(GJGameLevel* level) {
        int demonDifficulty = 2;
        if(level->demonDifficulty >= 5) demonDifficulty = level->demonDifficulty - 2;
        else if(level->demonDifficulty >= 3) demonDifficulty = level->demonDifficulty - 3;
        return demonDifficulty;
}

std::deque<gd::GJGameLevel*> BetterInfo::completedDeque() {
        std::deque<GJGameLevel*> levelsDeque;
        auto levels = GameLevelManager::sharedState()->m_onlineLevels;
        CCDictElement* obj;
        CCDICT_FOREACH(levels, obj){
                auto currentLvl = static_cast<GJGameLevel*>(obj->getObject());
                levelsDeque.push_back(currentLvl);
        }
        return levelsDeque;
}

void BetterInfo::strToLower(std::string& str) {
        for(auto& c : str) c = tolower(c);
}