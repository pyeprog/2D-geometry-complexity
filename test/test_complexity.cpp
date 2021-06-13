#include <vector>
#include "gtest/gtest.h"
#include "complexity.hpp"

using namespace geom_complex;

class ComplexityTest : public ::testing::Test {
protected:
    void SetUp() override {
        geos::io::WKTReader reader;
        pGeom1 = reader.read("POLYGON ((515 426, 370 390, 410 230, 610 240, 540 290, 720 300, 710 430, 515 426))");
        pGeom2 = reader.read("POLYGON ((520 420, 311 395, 410 220, 400 250, 610 240, 540 290, 720 300, 710 410, 520 420))");
    }

    geos::geom::Geometry::Ptr pGeom1;
    geos::geom::Geometry::Ptr pGeom2;
};

TEST_F(ComplexityTest, CalcValidityTest) {
    Complexity complexity{std::move(pGeom1), 3};

    auto res = complexity.calc();

    ASSERT_TRUE(res > 0.0);
}

TEST_F(ComplexityTest, CalcCorrectnessTest) {
    Complexity complx1{std::move(pGeom1), 5};
    Complexity complx2{std::move(pGeom2), 5};

    auto res1 = complx1.calc();
    auto res2 = complx2.calc();

    ASSERT_TRUE(res1 < res2);
}

TEST_F(ComplexityTest, calcGlobalDistEntropyTest) {
    Complexity complexity{std::move(pGeom1), 3};
    auto distEntropy = complexity.calcGlobalDistEntropy();

    ASSERT_TRUE(distEntropy > 0.0);
}

TEST_F(ComplexityTest, calcGlobalDistEntropyCorrectness) {
    Complexity complexity1{std::move(pGeom1), 3};
    Complexity complexity2{std::move(pGeom2), 3};

    auto distEntr1 = complexity1.calcGlobalDistEntropy();
    auto distEntr2 = complexity2.calcGlobalDistEntropy();

    ASSERT_TRUE(distEntr1 < distEntr2);
}

TEST_F(ComplexityTest, calcLocalAngleEntropyTest) {
    Complexity complexity{std::move(pGeom1), 3};
    auto angleEntropy = complexity.calcLocalAngleEntropy();

    ASSERT_TRUE(angleEntropy > 0.0);
}

TEST_F(ComplexityTest, calcLocalAngleEntropyCorrectness) {
    Complexity complexity1{std::move(pGeom1), 3};
    Complexity complexity2{std::move(pGeom2), 3};

    auto angleEntr1 = complexity1.calcLocalAngleEntropy();
    auto angleEntr2 = complexity2.calcLocalAngleEntropy();

    ASSERT_TRUE(angleEntr1 < angleEntr2);
}

TEST_F(ComplexityTest, calcSmoothnessTest) {
    Complexity complexity{std::move(pGeom1), 3};
    auto smoothness = complexity.calcPerceptualSmoothness();

    ASSERT_TRUE(smoothness > 0.0);
}

TEST_F(ComplexityTest, calcSmoothnessCorrectness) {
    Complexity complexity1{std::move(pGeom1), 3};
    Complexity complexity2{std::move(pGeom2), 3};

    auto smooth1 = complexity1.calcPerceptualSmoothness();
    auto smooth2 = complexity2.calcPerceptualSmoothness();

    ASSERT_TRUE(smooth1 < smooth2);
}
