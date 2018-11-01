#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <vector>
#include <sstream>
#include <QFuture>
#include <QFutureWatcher>
#include "antalgorithm.h"
#include "bruteforcealgorithm.h"
#include "dynamicstimeoptimizer.h"
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>

QT_CHARTS_USE_NAMESPACE

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

    void launchBruteforceAlgorithm();
    void bruteforceAlgoFinished();

    void launchDTOAlgorithm();
    void DTOAlgoFinished();

    void updateStats();

    void onAlgoParametersChanged();

private:

    QChart *makeWaitingTimeChart();
    QChart *makeDriverDistsChart();

    RenderArea *renderArea;
    QPushButton *generateOrdersButton;
    QPushButton *generateDriversButton;
    QPushButton *generateCafesButton;
    QPushButton *antAlgoButton;
    QPushButton *bruteforceAlgoButton;
    QPushButton *DTOAlgoButton;

    QLabel *distanceLabel;
    QLabel *avgTimeLabel;
    QPushButton *timeButton;
    QPushButton *distanceButton;

    QSpinBox *ordersSpinBox;
    QSpinBox *driversSpinBox;
    QSpinBox *cafesSpinBox;

    QSpinBox *antSpinBox;

    QVector< pair<double,double> > orders;
    QVector< pair<double,double> > drivers;
    QVector< pair<double,double> > cafes;
    QVector< QVector< pair<double,double> > > driverPaths;
    std::stringstream cout;
    QFuture<int> algoResult;
    QFutureWatcher<int> algoWatcher;

    AntAlgorithm *antSolver;
    BruteforceAlgorithm *bruteforceSolver;
    DynamicsTimeOptimizer *DTOSolver;

    QVector<double> order_waiting_times;
    QVector<double> driver_distances;
};

#endif // WINDOW_H
