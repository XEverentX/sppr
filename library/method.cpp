// Copyright Lebedev Alexander 2020
#include "method.hpp"

#include <limits>
#include <iostream>
#include <cmath>

IMethod::IMethod(uint32_t count,
                 double eps,
                 std::function<double(double)> function)
        : m_maxCount(count),
          m_eps(eps),
          m_function(function)
{
    
}

void IMethod::addSegment(double lowerBoundary, double upperBoundary)
{
    m_segments.insert({getValue(lowerBoundary, upperBoundary), {lowerBoundary, upperBoundary}});
}

double inline IMethod::f(double x) const
{
    return m_function(x);
}

void IMethod::execute(uint32_t *count, double *min, double *point, double lowerBoundary, double upperBoundary)
{
    addSegment(lowerBoundary, upperBoundary);

    double   currPoint;
    double   currEps = std::numeric_limits<double>::max();
    double   globalMin = std::numeric_limits<double>::max();
    uint32_t currCount = 0;

    do
    {
        preprocess();

        auto maxValueSegmentIter = --m_segments.end();
        auto maxValueSegment = *maxValueSegmentIter; 
        m_segments.erase(maxValueSegmentIter);

        auto left = maxValueSegment.second.first;
        auto right = maxValueSegment.second.second;
        auto middle = getPoint(left, right);

        addSegment(left, middle);
        addSegment(middle, right);

        auto currMin = m_function(middle);
        currEps = std::fabs(right - left);

        if (currMin < globalMin)
        {
            globalMin = currMin;
            currPoint = middle;
        }

        currCount++;
    } while (currEps >= m_eps && currCount < m_maxCount);
    
    *count = currCount;
    *min = globalMin;
    *point = currPoint;
}

SeqScanMethod::SeqScanMethod(uint32_t count,
                             double eps,
                             std::function<double(double)> function)
        : IMethod(count, eps, function)
{
    // Empty constructor
}                                    

double SeqScanMethod::getValue(double lowerBoundary, double upperBoundary)
{
    return upperBoundary - lowerBoundary;
}

double SeqScanMethod::getPoint(double lowerBoundary, double upperBoundary)
{
    return (lowerBoundary + upperBoundary) / 2;
}

void SeqScanMethod::preprocess() { }

PiyavskiyMethod::PiyavskiyMethod(uint32_t count,
                             double eps,
                             double parameter,
                             std::function<double(double)> function)
        : IMethod(count, eps, function),
          m_parameter(parameter)
{
    // Empty constructor
}                                    

double PiyavskiyMethod::getValue(double lowerBoundary, double upperBoundary)
{
    return 0.5 * m_parameter * (upperBoundary - lowerBoundary) - (f(upperBoundary) + f(lowerBoundary)) / 2;
}

double PiyavskiyMethod::getPoint(double lowerBoundary, double upperBoundary)
{
    return 0.5 * (upperBoundary + lowerBoundary) - (f(upperBoundary) - f(lowerBoundary)) / (2 * m_parameter);
}

void PiyavskiyMethod::preprocess() { }

StronginMethod::StronginMethod(uint32_t count,
                             double eps,
                             double parameter,
                             std::function<double(double)> function)
        : IMethod(count, eps, function),
          m_parameter(parameter)
{
    // Empty constructor
}                                    

double StronginMethod::getValue(double lowerBoundary, double upperBoundary)
{
    double l = lowerBoundary;
    double r = upperBoundary;
    double sqr = (f(r) - f(l));
    sqr = sqr * sqr;

    return m * (r - l) + sqr / (m * (r - l)) - 2 * (f(r) + f(l));
}

double StronginMethod::getPoint(double lowerBoundary, double upperBoundary)
{
    return 0.5 * (upperBoundary + lowerBoundary) - (f(upperBoundary) - f(lowerBoundary)) / (2 * m);
}

void StronginMethod::preprocess()
{
    double M = 0.;
    for (auto x : m_segments)
    {
        double currX = x.second.first;
        double prevX = x.second.second;
        M = std::max(M, std::fabs(f(currX) - f(prevX)) / (currX - prevX));
    }
    m = (std::fabs(M) < 1e-10 ? 1. : m_parameter * M);
}
