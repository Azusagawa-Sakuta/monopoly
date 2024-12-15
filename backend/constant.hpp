#ifndef CONSTANT_H
#define CONSTANT_H

#include <string>
#include <array>

namespace game {
    typedef long long cashType;
    namespace constant {
        double defaultTaxRate = 0.0f;
        cashType defaultPlotCost = 10000;
        cashType defaultHouseCost = 2000;
        cashType defaultBasicRent = 1000;
        std::array<cashType, 6> defaultHouseRent = {1000, 2000, 4000, 7000, 10000, 15000};
        cashType initialCash = 0;
        cashType homeReward = 0;
        cashType auctionReservePrice = 1000;
        cashType auctionBidIncrement = 500;
        std::array<cashType, 3> prisonReleasePrice = {5000, 3500, 1000};
        int diceMinimum = 1;
        int diceMaximum = 6;
    }
}
#endif