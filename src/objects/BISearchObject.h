#pragma once

struct BISearchObject {
    public:
        std::string difficulty; //TODO
        std::string length; //TODO
        int demon; //TODO
        bool star = false;
        bool noStar = false;
        bool uncompleted = false;
        bool completed = false;
        bool completedOrbs = false;
        bool completedLeaderboard = false;
        bool featured = false;
        bool original = false;
        bool twoPlayer = false;
        bool coins = false;
        bool epic = false;
        int folder = 0;
        bool song = false;
        bool songCustom = false;
        int songID = 0;
        bool copied = false;
        bool downloaded = false;
        bool ldm = false;
        int idRangeMin = 0;
        int idRangeMax = 0;
        bool copyable = false;
        bool freeCopy = false;
        bool unfeatured = false;
        bool unepic = false;
        int starRangeMin = 0;
        int starRangeMax = 0;
};