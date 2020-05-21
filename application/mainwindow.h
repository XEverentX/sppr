// Copyright Lebedev Alexander 2020
#pragma once
 
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
 
class Window : public QWidget {
    
public:
    Window(QWidget *parent = 0);

    ~Window() = default;

    void setStrongin() noexcept;
    
    void setPiyavskiy() noexcept;

    void setScan() noexcept;

    void run();
private:
    QLineEdit *evalA;
    QLineEdit *evalB;
    QLineEdit *evalC;
    QLineEdit *evalD;

    QLineEdit *lowerBoundary;
    QLineEdit *upperBoundary;

    QLineEdit *parameter;
    QLineEdit *eps;
    QLineEdit *maxCount;

    QLineEdit *min;
    QLineEdit *x;
    QLineEdit *count;

    QLabel *stronginLabel;
    QLabel *PiyavskiyLabel;
    QLabel *scanLabel;

    QLabel *methodLabel;

    enum {
        PIYAVSKIY,
        STRONGIN,
        SCAN
    } methodType;
};
