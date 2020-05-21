// Copyright Lebedev Alexander 2020
#pragma once
#include <cstdint>
#include <set>
#include <utility>
#include <functional>

class IMethod
{
public:
    using value    = double;
    using boundary = std::pair<double, double>;
    using segment  = std::pair<value, boundary>;

    explicit IMethod(uint32_t count,
                     double eps,
                     std::function<double(double)> function);

    void execute(uint32_t *count, double *min, double *point, double lowerBoundary, double upperBoundary);

    double inline f(double x) const;
protected:
    [[nodiscard]] virtual double getValue(double lowerBoundary, double upperBoundary) = 0;

    [[nodiscard]] virtual double getPoint(double lowerBoundary, double upperBoundary) = 0;

    virtual void preprocess() = 0;

    void addSegment(double lowerBoundary, double upperBoundary);

    uint32_t                      m_maxCount;
    double                        m_eps;
    std::set<segment>             m_segments;
    std::function<double(double)> m_function;
};

class SeqScanMethod final : public IMethod
{
public:
    explicit SeqScanMethod(uint32_t count,
                           double eps,
                           std::function<double(double)> function);

    ~SeqScanMethod() = default;
private:
    [[nodiscard]] double getValue(double lowerBoundary, double upperBoundary) override;

    [[nodiscard]] double getPoint(double lowerBoundary, double upperBoundary) override;

    void preprocess() override;
};

class PiyavskiyMethod final : public IMethod
{
public:
    explicit PiyavskiyMethod(uint32_t count,
                           double eps,
                           double parameter,
                           std::function<double(double)> function);

    ~PiyavskiyMethod() = default;
private:
    [[nodiscard]] double getValue(double lowerBoundary, double upperBoundary) override;

    [[nodiscard]] double getPoint(double lowerBoundary, double upperBoundary) override;

    void preprocess() override;

    double m_parameter;
};

class StronginMethod final : public IMethod
{
public:
    explicit StronginMethod(uint32_t count,
                           double eps,
                           double parameter,
                           std::function<double(double)> function);

    ~StronginMethod() = default;
private:
    [[nodiscard]] double getValue(double lowerBoundary, double upperBoundary) override;

    [[nodiscard]] double getPoint(double lowerBoundary, double upperBoundary) override;

    void preprocess() override;

    double m_parameter;
    double m;
};