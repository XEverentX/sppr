// Copyright Lebedev Alexander 2020
#include "mainwindow.h"

#include <method.hpp>

#include <iostream>
#include <functional>
#include <cmath>

#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
 
Window::Window(QWidget *parent)
        : QWidget(parent),
          methodType(STRONGIN)
{

    // LineEdits
    evalA = new QLineEdit("2", this);
    evalB = new QLineEdit("3", this);
    evalC = new QLineEdit("3", this);
    evalD = new QLineEdit("5", this);

    x1 = new QLineEdit("0", this);
    x2 = new QLineEdit("8", this);

    parameter = new QLineEdit("1.1", this);
    eps       = new QLineEdit("0.01", this);
    maxCount  = new QLineEdit("800", this);

    min   = new QLineEdit(this);
    count = new QLineEdit(this);
    x     = new QLineEdit(this);

    min->setReadOnly(true);
    x->setReadOnly(true);
    count->setReadOnly(true);

    // Lables
    QLabel *header      = new QLabel("Search for the global minimum of a function:", this);
    QLabel *evaluation1 = new QLabel("F(x) = ", this);
    QLabel *evaluation2 = new QLabel("sin", this);
    QLabel *evaluation3 = new QLabel("x + ", this);
    QLabel *evaluation4 = new QLabel("cos", this);
    QLabel *evaluation5 = new QLabel("x", this);

    QLabel *boundary = new QLabel(" <= x <= ", this);

    QLabel *parameterLabel = new QLabel("Parameter: ", this);
    QLabel *epsLabel       = new QLabel("Accuracy: ", this);
    QLabel *maxCountLabel  = new QLabel("Maximal Count: ", this);

    methodLabel = new QLabel("Strongin");

    QLabel *method  = new QLabel("Method: ", this);

    QLabel *minLabel   = new QLabel("Minimum: ", this);
    QLabel *xLabel     = new QLabel("Coordinate: ", this);
    QLabel *countLabel = new QLabel("Count: ", this);
    
    // Buttons
    QPushButton *runButton = new QPushButton("Run", this);

    QPushButton *stronginButton    = new QPushButton("Strongin", this);
    QPushButton *PiyavskiyButton = new QPushButton("Piyavskiy", this);
    QPushButton *scanButton        = new QPushButton("SeqScanning", this);

    // Layouts
    auto mainLayout = new QGridLayout(this);
    auto layout  = new QVBoxLayout();
    auto evalLayout = new QHBoxLayout();
    auto boundaryLayout = new QHBoxLayout();
    auto methodButtonsLayout = new QHBoxLayout();
    auto methodLayout = new QHBoxLayout();
    auto parametersLayout = new QGridLayout();
    auto resultLayout = new QGridLayout();

    // Building
    layout->addWidget(header);

    evalLayout->addWidget(evaluation1);
    evalLayout->addWidget(evalA);
    evalLayout->addWidget(evaluation2);
    evalLayout->addWidget(evalB);
    evalLayout->addWidget(evaluation3);
    evalLayout->addWidget(evalC);
    evalLayout->addWidget(evaluation4);
    evalLayout->addWidget(evalD);
    evalLayout->addWidget(evaluation5);
    layout->addLayout(evalLayout);

    boundaryLayout->addWidget(x1);
    boundaryLayout->addWidget(boundary);
    boundaryLayout->addWidget(x2);
    layout->addLayout(boundaryLayout);


    methodButtonsLayout->addWidget(stronginButton);
    methodButtonsLayout->addWidget(PiyavskiyButton);
    methodButtonsLayout->addWidget(scanButton);
    layout->addLayout(methodButtonsLayout);

    methodLayout->addWidget(method);
    methodLayout->addWidget(methodLabel);
    layout->addLayout(methodLayout);

    parametersLayout->addWidget(parameterLabel, 0, 0);
    parametersLayout->addWidget(parameter, 0, 1);
    parametersLayout->addWidget(epsLabel, 1, 0);
    parametersLayout->addWidget(eps, 1, 1);
    parametersLayout->addWidget(maxCountLabel, 2, 0);
    parametersLayout->addWidget(maxCount, 2, 1);
    layout->addLayout(parametersLayout);

    layout->addWidget(runButton);

    layout->addWidget(new QLabel("Results:", this));
    resultLayout->addWidget(minLabel, 0, 0);
    resultLayout->addWidget(min, 0, 1);
    resultLayout->addWidget(xLabel, 1, 0);
    resultLayout->addWidget(x, 1, 1);
    resultLayout->addWidget(countLabel, 2, 0);
    resultLayout->addWidget(count, 2, 1);
    layout->addLayout(resultLayout);

    mainLayout->addLayout(layout, 0, 0);

    // Plot
    customPlot = new QCustomPlot(this);

    customPlot->xAxis2->setVisible(true);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->yAxis2->setVisible(true);
    customPlot->yAxis2->setTickLabels(false);

    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    QSizePolicy sp = customPlot->sizePolicy();
    sp.setHorizontalStretch(1);
    customPlot->setSizePolicy(sp);
    mainLayout->addWidget(customPlot, 0, 1);

    setLayout(mainLayout);

    // Connect
    connect(runButton, &QPushButton::clicked, this, &Window::run);
    
    connect(stronginButton, &QPushButton::clicked, this, &Window::setStrongin);
    connect(PiyavskiyButton, &QPushButton::clicked, this, &Window::setPiyavskiy);
    connect(scanButton, &QPushButton::clicked, this, &Window::setScan);
}

void Window::setStrongin() noexcept
{
    methodType = STRONGIN;
    methodLabel->setText("Strongin");
}
    
void Window::setPiyavskiy() noexcept
{
    methodType = PIYAVSKIY;
    methodLabel->setText("Piyavskiy");
}

void Window::setScan() noexcept
{
    methodType = SCAN;
    methodLabel->setText("Sequential scanning");
}

void Window::run()
{
    auto evalAVal = evalA->text().toDouble();
    auto evalBVal = evalB->text().toDouble();
    auto evalCVal = evalC->text().toDouble();
    auto evalDVal = evalD->text().toDouble();

    auto x1Val = x1->text().toDouble();
    auto x2Val = x2->text().toDouble();

    auto parameterVal = parameter->text().toDouble(); 
    auto epsVal       = eps->text().toDouble();
    auto maxCountVal  = maxCount->text().toUInt();

    double globalMin     = 0.;
    double globalPoint   = 0.;
    uint32_t globalCount = 0;

    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(Qt::blue));
    QVector<double> cx(500), y(500);
    double len = static_cast<double>(x2Val - x1Val) / 500;

    for (int i = 0; i < 500; i++)
    {
        cx[i] = i * len;
        y[i] = evalAVal * sin(cx[i] * evalBVal) + evalCVal * cos(cx[i] * evalDVal);
    }

    customPlot->graph(0)->setData(cx, y);
    customPlot->graph(0)->rescaleAxes();
    customPlot->xAxis->setRange(x1Val, x2Val);
    customPlot->yAxis->setRange(-10, 10);

    auto lambda = [&] (double x) -> double {
            return evalAVal * sin(x * evalBVal) + evalCVal * cos(x * evalDVal);
        };

    IMethod *method;

    switch (methodType)
    {
    case SCAN:
        method = new SeqScanMethod(maxCountVal, epsVal, lambda);
        break;

    case PIYAVSKIY:

        method = new PiyavskiyMethod(maxCountVal, epsVal, parameterVal, lambda);
        break;

    case STRONGIN:
        method = new StronginMethod(maxCountVal, epsVal, parameterVal, lambda);
        break;
    }
    
    method->execute(&globalCount, &globalMin, &globalPoint, x1Val, x2Val);
    
    auto xVector = method->getXVector();
    QVector<double> qXVector;
    QVector<double> qYVector;

    for (auto x : xVector)
    {
        qXVector.push_back(x);
        qYVector.push_back(-9.);
    }

    customPlot->addGraph();
    customPlot->graph(1)->setData(qXVector, qYVector);
    customPlot->graph(1)->setPen(QColor(50, 50, 50, 255));
    customPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
    customPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));

    customPlot->replot();

    min->setPlaceholderText(QString::number(globalMin));
    count->setPlaceholderText(QString::number(globalCount));
    x->setPlaceholderText(QString::number(globalPoint));
}
