// Copyright Lebedev Alexander 2020
#pragma once
#include <cstdint>
#include <set>
#include <utility>
#include <functional>
#include <vector>

class IMethod
{
public:
    using value    = double;
    using boundary = std::pair<double, double>;
    using segment  = std::pair<value, boundary>;

    explicit IMethod(uint32_t count,
                     double eps,
                     std::function<double(double)> function);

    void execute(uint32_t *count, double *min, double *point, double x1, double x2);

    double inline f(double x) const;

    std::vector<double> getXVector() const;

protected:
    [[nodiscard]] virtual double getValue(double x1, double x2) = 0;

    [[nodiscard]] virtual double getPoint(double x1, double x2) = 0;

    virtual void preprocess() = 0;

    double inline getOptimalInex();

    uint32_t                      m_maxCount;
    double                        m_eps;
    std::function<double(double)> m_function;
    std::vector<double>           m_x;
};

class SeqScanMethod final : public IMethod
{
public:
    explicit SeqScanMethod(uint32_t count,
                           double eps,
                           std::function<double(double)> function);

    ~SeqScanMethod() = default;
private:
    [[nodiscard]] double getValue(double x1, double x2) override;

    [[nodiscard]] double getPoint(double x1, double x2) override;

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
    [[nodiscard]] double getValue(double x1, double x2) override;

    [[nodiscard]] double getPoint(double x1, double x2) override;

    void preprocess() override;

    double m_parameter;
    double m;
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
    [[nodiscard]] double getValue(double x1, double x2) override;

    [[nodiscard]] double getPoint(double x1, double x2) override;

    void preprocess() override;

    double m_parameter;
    double m;
};