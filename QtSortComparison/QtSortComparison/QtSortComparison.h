#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtSortComparison.h"
#include <algorithm>
#include <QDebug>
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>
#include <thread>
#include <cmath>

class QtSortComparison : public QMainWindow
{
    Q_OBJECT

    typedef void (QtSortComparison::*Pt2Func)(std::vector<int, std::allocator<int>>&, int, int);

public:
    QtSortComparison(QWidget *parent = Q_NULLPTR);
    ~QtSortComparison();
signals:
    void bProg(int x);
    void sProg(int x);
    void qProg(int x);



private:
    Ui::QtSortComparisonClass ui;
    std::vector<int> Vector;
    std::vector<int> bVector;
    std::vector<int> sVector;
    std::vector<int> qVector;
    int VectorSize;
    int qSwaps = 0;
    double bubbleTime;
    double shellTime;
    double quickTime;
    void generateIntVector(std::vector<int>& vec, int size);
    void copyIntVector(std::vector<int>& vec1, std::vector<int>& vec2, int size);
    void timeOf(Pt2Func pt2Func, QtSortComparison* member, std::vector<int>& vec, int start, int end, double* outTime);
    int partition(std::vector<int>& vec, int low, int high);
    void bubbleSort(std::vector<int>& vec, int _null, int n);
    void shellSort(std::vector<int>& vec, int _null, int n);
    void shellPrattSort(std::vector<int>& vec, int _null, int n);
    void quickSort(std::vector<int>& vec, int low, int high);

private slots:
    void bubbleProgress(int x);
    void shellProgress(int x);
    void quickProgress(int x);
    void generateClicked();
    void showArrayClicked();
    void showBArrayClicked();
    void showSArrayClicked();
    void showQArrayClicked();
    void startAllSort();
    void startBubbleSort();
    void startShellSort();
    void startQuickSort();
    void StartBubbleSort();
    void StartShellSort();
    void StartQuickSort();
    void ArraySizeChanged(QString _item);

};
