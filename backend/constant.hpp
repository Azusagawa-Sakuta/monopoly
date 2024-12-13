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
        cashType defaultHotelCost = 5000;
        cashType defaultBasicRent = 1000;
        std::array<cashType, 5> defaultHouseRent = {1000, 2000, 3000, 4000, 5000};
        cashType defaultHotelRent = 4000;
        cashType initialCash = 0;
        cashType homeReward = 0;
    }
}
#endif