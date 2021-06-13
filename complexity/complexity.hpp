#pragma once

#include "geos.h"

namespace geom_complex {
    class Complexity {
    private:
        geos::geom::Geometry::Ptr pGeom;
        size_t maxJ;


        static std::vector<Coordinate> toCoords(CoordinateSequence::Ptr pSeq);

        std::vector<double> calcDists() const;

        std::vector<double> calcAngles() const;

    public:
        constexpr static double largeEntropyWeight = 0.6;
        constexpr static const double smallEntropyWeight = 0.07;
        constexpr static const double smoothnessWeight = 0.33;

        Complexity() = default;

        Complexity(geos::geom::Geometry::Ptr ptr, size_t maxJ = 1 << 2)
                : pGeom{std::move(ptr)},
                  maxJ{maxJ} {};

        Complexity(const Complexity &complexity) = delete;

        Complexity &operator=(const Complexity &complexity) = delete;

        Complexity(Complexity &&complexity) = delete;

        Complexity &operator=(Complexity &&complexity) = delete;

        double calcGlobalDistEntropy() const;

        double calcLocalAngleEntropy() const;

        double calcPerceptualSmoothness() const;

        double calc() const;
    };

    class Cost {
    private:
        size_t resolution;
        std::vector<double> scores;

        double gap() const;

    public:
        Cost(std::vector<double> scores, size_t resolution = 1 << 2)
                : scores{std::move(scores)},
                  resolution{resolution} {};

        Cost() = delete;

        Cost(const Cost &c) = delete;

        Cost &operator=(const Cost &c) = delete;

        Cost(Cost &&c) = default;

        Cost &operator=(Cost &&c) = delete;

        double calcEntropy() const;

        std::vector<size_t> calcLevels() const;

        std::vector<double> calcLevelAverages() const;

        double calcError() const;

        static double distFactor(const std::vector<double> &scores, size_t maxJ);
    };

}