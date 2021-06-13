#include "geos.h"
#include "complexity.hpp"
#include <iostream>
#include <cmath>
#include <limits>

using namespace geom_complex;

double Complexity::calc() const {
    double distEntropy = this->calcGlobalDistEntropy();
    double angleEntropy = this->calcLocalAngleEntropy();
    double smoothness = this->calcPerceptualSmoothness();

    double large = (distEntropy > angleEntropy ? distEntropy : angleEntropy);
    double small = (distEntropy > angleEntropy ? angleEntropy : distEntropy);

    return large * largeEntropyWeight + small * smallEntropyWeight + smoothness * smoothnessWeight;
}

double Complexity::calcGlobalDistEntropy() const {
    auto dists = this->calcDists();
    if (dists.empty()) {
        return 0.0;
    }

    double maxDist = *std::max_element(dists.begin(), dists.end());

    std::vector<double> distScores(dists.size());

    for (auto dist: dists) {
        distScores.push_back(dist / maxDist);
    }
    return Cost::distFactor(distScores, this->maxJ);
}

std::vector<double> Complexity::calcDists() const {
    geos::geom::Point::Ptr pPoint = pGeom->getCentroid();

    std::vector<double> dists;

    for (const auto &coord : toCoords(pGeom->getCoordinates())) {
        auto dist = coord.distance(*pPoint->getCoordinate());
        dists.push_back(dist);
    }

    return dists;
}

std::vector<double> Complexity::calcAngles() const {
    pGeom->normalize();
    auto coords = toCoords(pGeom->getCoordinates());

    std::vector<double> angles;

    if (coords.size() < 3) {
        return angles;
    }

    if (coords.front().equals2D(coords.back())) {
        coords.pop_back();
    }

    for (size_t i = 0; i < coords.size(); ++i) {
        auto pre = coords[((i - 1) + coords.size()) % coords.size()];
        auto cur = coords[i];
        auto post = coords[(i + 1) % coords.size()];

        auto post_degree = atan2(post.y - cur.y, post.x - cur.x);
        auto pre_degree = atan2(cur.y - pre.y, cur.x - pre.x);

        // in [0, pi)
        angles.push_back(std::fmod(post_degree - pre_degree + 2 * M_PI, M_PI));
    }

    return angles;
}

std::vector<Coordinate> Complexity::toCoords(CoordinateSequence::Ptr pSeq) {
    std::vector<Coordinate> coords;

    for (size_t i = 0; i < pSeq->size(); ++i) {
        coords.push_back(pSeq->getAt(i));
    }

    return coords;
}

double Complexity::calcLocalAngleEntropy() const {
    auto angles = this->calcAngles();
    std::vector<double> angleScores(angles.size());

    for (auto angle : angles) {
        angleScores.push_back(angle / M_PI);
    }

    return Cost::distFactor(angleScores, this->maxJ);
}

double Complexity::calcPerceptualSmoothness() const {
    auto angles = this->calcAngles();
    if (angles.empty()) {
        return 0.0;
    }

    double smoothess = 0.0;

    for (auto angle : this->calcAngles()) {
        smoothess += (exp(-angle / M_PI) - exp(-1)) / (1 - exp(-1));
    }

    return smoothess / static_cast<double>(angles.size());
}

double Cost::calcEntropy() const {
    auto levels = this->calcLevels();

    std::vector<size_t> counts(this->resolution, 0);

    for (const auto &lv : levels) {
        ++counts[lv];
    }

    double entropy = 0.0;
    for (const auto &c : counts) {
        if (c == 0) continue;
        auto p = static_cast<double>(c) / static_cast<double>(levels.size());
        entropy += -p * log2(p);
    }

    return entropy;
}

std::vector<size_t> Cost::calcLevels() const {
    std::vector<size_t> levels;
    for (const auto &s : this->scores) {
        auto level = std::min(this->resolution - 1, static_cast<size_t>(s / this->gap()));
        levels.push_back(level);
    }

    return levels;
}

double Cost::calcError() const {
    if (this->scores.empty()) {
        return 0.0;
    }

    auto averages = this->calcLevelAverages();
    auto gapCache = this->gap();

    double error_sum = 0.0;

    for (size_t i = 0; i < averages.size(); ++i) {
        if (averages[i] == 0.0) continue;
        double levelExpect = static_cast<double>(i + 1) * gapCache;
        error_sum += pow(levelExpect - averages[i], 2);
    }

    return error_sum / static_cast<double>(this->scores.size());
}

std::vector<double> Cost::calcLevelAverages() const {
    if (this->scores.empty()) {
        return std::vector<double>();
    }

    std::vector<double> sum(this->resolution, 0.0);
    std::vector<size_t> lvCounts(this->resolution, 0);

    for (const auto &s : this->scores) {
        auto level = std::min(this->resolution - 1, static_cast<size_t>(s / this->gap()));
        ++lvCounts[level];
        sum[level] += s;
    }

    std::vector<double> averages(this->resolution, 0.0);
    for (size_t i = 0; i < this->resolution; ++i) {
        if (lvCounts[i] == 0) continue;
        averages[i] = sum[i] / static_cast<double>(lvCounts[i]);
    }

    return averages;
}

double Cost::gap() const {
    return 1.0 / static_cast<double>(std::max(1ul, this->resolution));
}

double Cost::distFactor(const std::vector<double> &scores, size_t maxJ) {
    if (scores.empty()) {
        return 0;
    }

    std::vector<double> entropies;
    std::vector<double> errors;

    for (size_t j = 1; j <= maxJ; ++j) {
        auto cost = Cost{scores, 1ul << j};
        entropies.push_back(cost.calcEntropy());
        errors.push_back(cost.calcError());
    }

    double maxError = *std::max_element(errors.begin(), errors.end());
    double maxEntropy = log2(static_cast<double>(scores.size()));

    double minDistFactor = std::numeric_limits<double>::max();
    for (size_t i = 0; i < maxJ; ++i) {
        double distFactor = entropies[i] / maxEntropy + errors[i] / maxError;
        minDistFactor = std::min(minDistFactor, distFactor);
    }

    return minDistFactor;
}
