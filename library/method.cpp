// Copyright Lebedev Alexander 2020
#include "method.hpp"

#include <limits>
#include <cmath>
#include <algorithm>

IMethod::IMethod(uint32_t count,
                 double eps,
                 std::function<double(double)> function)
        : m_maxCount(count),
          m_eps(eps),
          m_function(function)
{
    
}

double inline IMethod::f(double x) const
{
    return m_function(x);
}

double inline IMethod::getOptimalInex()
{
    double maxValue = std::numeric_limits<double>::min();
    int index = 1;

    for (int i = 1; i < m_x.size(); i++)
    {
        auto value = getValue(m_x[i - 1], m_x[i]);
        if (value > maxValue)
        {
            maxValue = value;
            index = i;
        }
    }

    return index;
}

void IMethod::execute(uint32_t *count, double *min, double *point, double x1, double x2)
{
    double   currEps = std::numeric_limits<double>::max();
    double   currPoint;
    double   globalMin;

    if(f(x1) > f(x2))
    {
        globalMin = f(x2);
        currPoint = x2;
    } else
    {
        globalMin = f(x1);
        currPoint = x1;
    }
    
    uint32_t currCount = 0;

    m_x.push_back(x1);
    m_x.push_back(x2);

    do
    {
        preprocess();

        auto index = getOptimalInex();

        auto left = m_x[index - 1];
        auto right = m_x[index];
        auto middle = getPoint(left, right);

        currEps = std::fabs(right - left);

        m_x.push_back(middle);

        std::sort(m_x.begin(), m_x.end());

        auto currMin = m_function(middle);

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

std::vector<double> IMethod::getXVector() const
{
    return m_x;
}

SeqScanMethod::SeqScanMethod(uint32_t count,
                             double eps,
                             std::function<double(double)> function)
        : IMethod(count, eps, function)
{
    // Empty constructor
}                                    

double SeqScanMethod::getValue(double x1, double x2)
{
    return x2 - x1;
}

double SeqScanMethod::getPoint(double x1, double x2)
{
    return (x1 + x2) / 2;
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

double PiyavskiyMethod::getValue(double x1, double x2)
{
    return 0.5 * m * (x2 - x1) - (f(x2) + f(x1)) / 2.;
}

double PiyavskiyMethod::getPoint(double x1, double x2)
{
    return (0.5 * (x2 + x1)) - (f(x2) - f(x1)) / (2. * m);
}

void PiyavskiyMethod::preprocess()
{
    double M = 0.;
    for (int i = 1; i < m_x.size(); i++)
    {
        double prevX = m_x[i - 1];
        double currX = m_x[i];
        M = std::max(M, std::fabs(f(currX) - f(prevX)) / (currX - prevX));
    }
    m = (M <= 0. ? 1. : m_parameter * M);
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

double StronginMethod::getValue(double x1, double x2)
{
    double z1 = f(x1);
    double z2 = f(x2);

    return m * (x2 - x1) + (z2 - z1) * (z2 - z1) / (m * (x2 - x1)) - 2 * (z1 + z2);
}

double StronginMethod::getPoint(double x1, double x2)
{
    return 0.5 * (x2 + x1) - (f(x2) - f(x1)) / (2 * m);
}

void StronginMethod::preprocess()
{
    double M = 0.;
    for (int i = 1; i < m_x.size(); i++)
    {
        double prevX = m_x[i - 1];
        double currX = m_x[i];
        M = std::max(M, std::fabs(f(currX) - f(prevX)) / (currX - prevX));
    }
    m = (M <= 0. ? 1. : m_parameter * M);
}
