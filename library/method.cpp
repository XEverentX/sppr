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
        auto maxValueSegmentIter = --m_segments.end();
        auto maxValueSegment = *maxValueSegmentIter; 
        m_segments.erase(maxValueSegmentIter);

        auto left = maxValueSegment.second.first;
        auto right = maxValueSegment.second.second;
        auto middle = getPoint(left, right);

        addSegment(left, middle);
        addSegment(middle, right);

        auto currMin = m_function(middle);
        // currEps = std::fabs(globalMin - currMin);

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
    return m_parameter * (upperBoundary - lowerBoundary) / 2 - (f(upperBoundary) + f(lowerBoundary)) / 2;
}

double PiyavskiyMethod::getPoint(double lowerBoundary, double upperBoundary)
{
    return (upperBoundary + lowerBoundary) / 2 - (f(upperBoundary) - f(lowerBoundary)) / (2 * m_parameter);
}

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
    return upperBoundary - lowerBoundary;
}

double StronginMethod::getPoint(double lowerBoundary, double upperBoundary)
{
    return (lowerBoundary + upperBoundary) / 2;
}
