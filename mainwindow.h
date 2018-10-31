#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <vector>
#include <sstream>
#include <QFuture>
#include <QFutureWatcher>
#include "antalgorithm.h"

using std::vector;
using std::pair;

class QCheckBox;
class QComboBox;
class QLabel;
class QSpinBox;
class QPushButton;
class RenderArea;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void generateOrders();
    void generateDrivers();
    void generateCafes();
    void launchAntAlgorithm();

    void antAlgoFinished();

private:
    RenderArea *renderArea;
    QPushButton *generateOrdersButton;
    QPushButton *generateDriversButton;
    QPushButton *generateCafesButton;
    QPushButton *antAlgoButton;

    QSpinBox *ordersSpinBox;
    QSpinBox *driversSpinBox;
    QSpinBox *cafesSpinBox;

    QVector< pair<double,double> > orders;
    QVector< pair<double,double> > drivers;
    QVector< pair<double,double> > cafes;
    QVector< QVector< pair<double,double> > > driverPaths;
    std::stringstream cout;
    QFuture<int> algoResult;
    QFutureWatcher<int> algoWatcher;

    AntAlgorithm *antSolver;
};

#endif // WINDOW_H
