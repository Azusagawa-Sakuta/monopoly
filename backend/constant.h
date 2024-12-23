// constant.h
#ifndef CONSTANT_H
#define CONSTANT_H

#include <string>
#include <array>

namespace game {
    typedef long long cashType;
    namespace constant {
        extern double defaultTaxRate;
        extern cashType defaultPlotCost;
        extern cashType defaultHouseCost;
        extern cashType defaultBasicRent;
        extern std::array<cashType, 6> defaultHouseRent;
        extern cashType initialCash;
        extern cashType homeReward;
        extern cashType auctionReservePrice;
        extern cashType auctionBidIncrement;
        extern std::array<cashType, 3> prisonReleasePrice;
        extern int diceMinimum;
        extern int diceMaximum;
    }
}
#endif // CONSTANT_H
