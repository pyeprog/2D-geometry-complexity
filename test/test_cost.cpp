#include <vector>
#include "gtest/gtest.h"
#include "complexity.hpp"

TEST(CostTest, calcEmptyLevelTest) {
    geom_complex::Cost cost{{}, 1 << 2};
    auto lvs = cost.calcLevels();

    ASSERT_EQ(lvs.size(), 0);
}

TEST(CostTest, calcLevelTest) {
    geom_complex::Cost cost{{0.1, 0.3, 0.8, 1.0}, 1 << 2};
    auto lvs = cost.calcLevels();

    ASSERT_EQ(lvs.size(), 4);

    ASSERT_EQ(lvs[0], 0);
    ASSERT_EQ(lvs[1], 1);
    ASSERT_EQ(lvs[2], 3);
    ASSERT_EQ(lvs[3], 3);
}

TEST(CostTest, calcEmptyEntropyTest) {
    geom_complex::Cost cost{{}, 1 << 3};

    ASSERT_EQ(cost.calcEntropy(), 0.0);
}

TEST(CostTest, calcEntropyTest) {
    geom_complex::Cost cost{{0.1, 0.3, 0.8, 1.0}, 1 << 3};

    ASSERT_TRUE(cost.calcEntropy() > 0.0);
    ASSERT_EQ(cost.calcEntropy(), 2.0);
}

TEST(CostTest, calcEmptyLevelAveragesTest) {
    geom_complex::Cost cost{{}, 1 << 3};
    auto averages = cost.calcLevelAverages();

    ASSERT_EQ(averages.size(), 0);
}


TEST(CostTest, calcLevelAveragesTest) {
    size_t resolution = 1 << 2;
    geom_complex::Cost cost{{0.1, 0.3, 0.8, 1.0}, resolution};
    auto averages = cost.calcLevelAverages();

    ASSERT_EQ(averages.size(), resolution);
    ASSERT_EQ(averages[0], 0.1);
    ASSERT_EQ(averages[1], 0.3);
    ASSERT_EQ(averages[2], 0.0);
    ASSERT_EQ(averages[3], 0.9);
}

TEST(CostTest, calcEmptyErrorTest) {
    geom_complex::Cost cost{{}, 1 << 3};
    auto err = cost.calcError();

    ASSERT_EQ(err, 0.0);
}

TEST(CostTest, calcErrorTest) {
    geom_complex::Cost cost{{0.1, 0.3, 0.8, 1.0}, 1 << 3};
    auto err = cost.calcError();

    ASSERT_TRUE(err > 0.0);
}

TEST(CostTest, emptyDistFactorTest) {
    auto distFactor = geom_complex::Cost::distFactor({}, 5);

    ASSERT_EQ(distFactor, 0.0);
}

TEST(CostTest, distFactorTest) {
    auto distFactor = geom_complex::Cost::distFactor({0.1, 0.3, 0.8, 1.0}, 5);

    ASSERT_TRUE(distFactor > 0.0);

    auto lessDistFactor = geom_complex::Cost::distFactor({0.1, 0.1, 0.1, 0.2}, 5);
    ASSERT_TRUE(distFactor > lessDistFactor);

    auto minDistFactor = geom_complex::Cost::distFactor({0.1, 0.1, 0.1, 0.1}, 5);
    ASSERT_TRUE(lessDistFactor > minDistFactor);
}