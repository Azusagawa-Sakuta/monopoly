// constant.cpp
#include "constant.h"

namespace game {
    namespace constant {
        double defaultTaxRate = 0.10f;
        cashType defaultPlotCost = 1000;
        cashType defaultHouseCost = 2000;
        cashType defaultBasicRent = 1000;
        std::array<cashType, 6> defaultHouseRent = {1000, 2000, 4000, 7000, 10000, 15000};
        double sameColorBonusMultiplier = 1.2;
        cashType initialCash = 100000;
        cashType homeReward = 2500;
        cashType auctionReservePrice = 1000;
        cashType auctionBidIncrement = 500;
        std::array<cashType, 3> prisonReleasePrice = {5000, 3500, 1000};
    }
}
