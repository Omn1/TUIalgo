#include "renderarea.h"
#include "mainwindow.h"
#include "antalgorithm.h"

#include <QtWidgets>
#include <random>
#include <QFuture>
#include <QtConcurrent>
#include <QFutureWatcher>

MainWindow::MainWindow()
{
    renderArea = new RenderArea;

    generateOrdersButton = new QPushButton("Сгенерировать заказы");
    connect(generateOrdersButton, &QPushButton::clicked, this, &MainWindow::generateOrders);

    generateDriversButton = new QPushButton("Сгенерировать водителей");
    connect(generateDriversButton, &QPushButton::clicked, this, &MainWindow::generateDrivers);

    generateCafesButton = new QPushButton("Сгенерировать рестораны");
    connect(generateCafesButton, &QPushButton::clicked, this, &MainWindow::generateCafes);

    ordersSpinBox = new QSpinBox;
    ordersSpinBox->setMinimum(1);
    ordersSpinBox->setMaximum(1000);

    driversSpinBox = new QSpinBox;
    driversSpinBox->setMinimum(1);
    driversSpinBox->setMaximum(200);

    cafesSpinBox = new QSpinBox;
    cafesSpinBox->setMinimum(1);
    cafesSpinBox->setMaximum(50);

    antSpinBox = new QSpinBox;
    antSpinBox->setMinimum(1);
    antSpinBox->setMaximum(10000000);

    speedSpinBox = new QSpinBox;
    speedSpinBox->setMinimum(1);
    speedSpinBox->setMaximum(10000);

    cookTimeBox = new QSpinBox;
    cookTimeBox->setMinimum(0);
    cookTimeBox->setMaximum(10000);

    connect(ordersSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onAlgoParametersChanged);
    connect(driversSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onAlgoParametersChanged);
    connect(cafesSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onAlgoParametersChanged);
    connect(antSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onAlgoParametersChanged);

    antAlgoButton = new QPushButton("Муравьиный алгоритм");
    connect(antAlgoButton, &QPushButton::clicked, this, &MainWindow::launchAntAlgorithm);

    bruteforceAlgoButton = new QPushButton("Динамика по подмножествам (расстояние)");
    connect(bruteforceAlgoButton, &QPushButton::clicked, this, &MainWindow::launchBruteforceAlgorithm);

    DTOAlgoButton = new QPushButton("Динамика по подмножествам (время)");
    connect(DTOAlgoButton, &QPushButton::clicked, this, &MainWindow::launchDTOAlgorithm);

    distanceLabel = new QLabel("Расстояние (пкс): 0");
    avgTimeLabel = new QLabel("Среднее время ожидания: 0");

    timeButton = new QPushButton("Получить статистику времени ожидания");
    connect(timeButton, &QPushButton::clicked, [this]{
        QChartView *cv = new QChartView(makeWaitingTimeChart());
        cv->setRenderHint(QPainter::Antialiasing);
        cv->setMinimumSize(800,600);
        cv->show();
    });

    distanceButton = new QPushButton("Получить статистику расстояний");
    connect(distanceButton, &QPushButton::clicked, [this]{
        QChartView *cv = new QChartView(makeDriverDistsChart());
        cv->setRenderHint(QPainter::Antialiasing);
        cv->setMinimumSize(800,600);
        cv->show();
    });

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setColumnStretch(3, 1);
    mainLayout->addWidget(renderArea, 0, 0, 1, 4);
    mainLayout->addWidget(generateOrdersButton, 1, 0);
    mainLayout->addWidget(generateDriversButton, 1, 1);
    mainLayout->addWidget(generateCafesButton, 1, 2);
    mainLayout->addWidget(ordersSpinBox, 2, 0);
    mainLayout->addWidget(driversSpinBox, 2, 1);
    mainLayout->addWidget(cafesSpinBox, 2, 2);
    mainLayout->addWidget(antAlgoButton, 3, 0);
    mainLayout->addWidget(bruteforceAlgoButton, 4, 0);
    mainLayout->addWidget(DTOAlgoButton, 5, 0);
    mainLayout->addWidget(distanceLabel, 3, 2);
    mainLayout->addWidget(avgTimeLabel, 4, 2);
    mainLayout->addWidget(distanceButton, 3, 3);
    mainLayout->addWidget(timeButton, 4, 3);
    QLabel *paramLabel = new QLabel("Параметры алгоритмов:");
    paramLabel->setAlignment(Qt::AlignCenter);
    paramLabel->setMaximumHeight(30);
    mainLayout->addWidget(paramLabel, 6, 0, 1, 4);
    mainLayout->addWidget(new QLabel("Жизнь муравьиной колонии:"), 7, 0);
    mainLayout->addWidget(antSpinBox, 7, 1);
    mainLayout->addWidget(new QLabel("Средняя скорость водителя (пкс/сек):"), 8, 0);
    mainLayout->addWidget(speedSpinBox, 8, 1);
    mainLayout->addWidget(new QLabel("Среднее время приготовления заказа (сек):"), 9, 0);
    mainLayout->addWidget(cookTimeBox, 9, 1);
    setLayout(mainLayout);

    generateOrders();
    generateDrivers();
    generateCafes();

    setWindowTitle(QString("Моделирование работы алгоритмов"));
}

double randomDouble(double min, double max)
{
    return min + (max - min) * (rand() * 1.0 / RAND_MAX);
}

int randomInt(int min, int max)
{
    return min + abs(rand()) % (max - min + 1);
}

void MainWindow::generateOrders()
{
    driverPaths.clear();
    renderArea->setDriverPaths(driverPaths);
    orders.clear();
    double maxX = renderArea->width() - 20;
    double maxY = renderArea->height() - 20;
    for (int i = 0; i < ordersSpinBox->value(); i++) {
        orders.append({randomDouble(20,maxX), randomDouble(20,maxY)});
    }
    renderArea->setOrders(orders);
    onAlgoParametersChanged();
}

void MainWindow::generateDrivers()
{
    driverPaths.clear();
    renderArea->setDriverPaths(driverPaths);
    drivers.clear();
    double maxX = renderArea->width() - 20;
    double maxY = renderArea->height() - 20;
    for (int i = 0; i < driversSpinBox->value(); i++) {
        drivers.append({randomDouble(20,maxX), randomDouble(20,maxY)});
    }
    renderArea->setDrivers(drivers);
    onAlgoParametersChanged();
}

void MainWindow::generateCafes()
{
    driverPaths.clear();
    renderArea->setDriverPaths(driverPaths);
    cafes.clear();
    double maxX = renderArea->width() - 20;
    double maxY = renderArea->height() - 20;
    for (int i = 0; i < cafesSpinBox->value(); i++) {
        cafes.append({randomDouble(20,maxX), randomDouble(20,maxY)});
    }
    renderArea->setCafes(cafes);
    onAlgoParametersChanged();
}

void MainWindow::launchAntAlgorithm()
{
    driverPaths.clear();
    renderArea->setDriverPaths(driverPaths);
    lockInterface();
    cout.clear();
    cout << orders.size() << "\n";
    for (int i = 0; i < orders.size(); i++) {
        cout << orders[i].first << " " << orders[i].second << " " << cookTimeBox->value() << "\n";
    }
    cout << drivers.size() << "\n";
    for (int i = 0; i < drivers.size(); i++) {
        cout << drivers[i].first << " " << drivers[i].second << " 0\n";
    }
    cout << cafes.size() << "\n";
    for (int i = 0; i < cafes.size(); i++) {
        cout << cafes[i].first << " " << cafes[i].second << "\n";
    }
    cout << speedSpinBox->value() << " \n";

    antSolver = new AntAlgorithm;
    antSolver->LIVE = antSpinBox->value();
    antSolver->cin.swap(cout);
    disconnect(this, SLOT(antAlgoFinished()));
    connect(&algoWatcher, &QFutureWatcher<int>::finished, this, &MainWindow::antAlgoFinished);
    algoResult = QtConcurrent::run(antSolver, &AntAlgorithm::solve);
    algoWatcher.setFuture(algoResult);
}

void MainWindow::antAlgoFinished()
{
    driverPaths.clear();
    for (int i = 0; i < drivers.size(); i++) {
        driverPaths.append(QVector< pair<double,double> >());
        driverPaths[i].append(drivers[i]);
        if(antSolver->driverPaths[i].size() == 0) continue;
        driverPaths[i].append(cafes[antSolver->driver[i].id]);
        for(size_t j = 0; j < antSolver->driverPaths[i].size(); j++) {
            int order_num = antSolver->driverPaths[i][j];
            pair<double,double> order_coords = orders[order_num];
            driverPaths[i].append(order_coords);
        }
    }
    renderArea->setDriverPaths(driverPaths);
    unlockInterface();
    updateStats();
}

void MainWindow::launchBruteforceAlgorithm()
{
    driverPaths.clear();
    renderArea->setDriverPaths(driverPaths);
    lockInterface();

    bruteforceSolver = new BruteforceAlgorithm;

    bruteforceSolver->orders = orders;
    bruteforceSolver->drivers = drivers;
    bruteforceSolver->cafes = cafes;

    disconnect(this, SLOT(bruteforceAlgoFinished()));
    connect(&algoWatcher, &QFutureWatcher<int>::finished, this, &MainWindow::bruteforceAlgoFinished);
    algoResult = QtConcurrent::run(bruteforceSolver, &BruteforceAlgorithm::solve);
    algoWatcher.setFuture(algoResult);
}

void MainWindow::bruteforceAlgoFinished()
{
    driverPaths.clear();
    for (int i = 0; i < drivers.size(); i++) {
        driverPaths.append(QVector< pair<double,double> >());
        driverPaths[i].append(drivers[i]);
        if( bruteforceSolver->drivers_cafe[i] == -1) continue;
        driverPaths[i].append(cafes[bruteforceSolver->drivers_cafe[i]]);
        for(int j = 0; j < bruteforceSolver->driversPath[i].size(); j++) {
            int order_num = bruteforceSolver->driversPath[i][j];
            pair<double,double> order_coords = orders[order_num];
            driverPaths[i].append(order_coords);
        }
    }
    renderArea->setDriverPaths(driverPaths);
    unlockInterface();
    updateStats();
}

void MainWindow::launchDTOAlgorithm()
{
    driverPaths.clear();
    renderArea->setDriverPaths(driverPaths);
    lockInterface();

    DTOSolver = new DynamicsTimeOptimizer;

    DTOSolver->orders = orders;
    DTOSolver->drivers = drivers;
    DTOSolver->cafes = cafes;

    DTOSolver->avg_cook_time = cookTimeBox->value();
    DTOSolver->driver_speed = speedSpinBox->value();

    disconnect(this, SLOT(DTOAlgoFinished()));
    connect(&algoWatcher, &QFutureWatcher<int>::finished, this, &MainWindow::DTOAlgoFinished);
    algoResult = QtConcurrent::run(DTOSolver, &DynamicsTimeOptimizer::solve);
    algoWatcher.setFuture(algoResult);
}

void MainWindow::DTOAlgoFinished()
{
    driverPaths.clear();
    for (int i = 0; i < drivers.size(); i++) {
        driverPaths.append(QVector< pair<double,double> >());
        driverPaths[i].append(drivers[i]);
        if( DTOSolver->drivers_cafe[i] == -1) continue;
        driverPaths[i].append(cafes[DTOSolver->drivers_cafe[i]]);
        for(int j = 0; j < DTOSolver->driversPath[i].size(); j++) {
            int order_num = DTOSolver->driversPath[i][j];
            pair<double,double> order_coords = orders[order_num];
            driverPaths[i].append(order_coords);
        }
    }
    renderArea->setDriverPaths(driverPaths);
    unlockInterface();
    updateStats();
}

void MainWindow::updateStats()
{
    double distance = 0;
    double sum_order_time = 0;
    order_waiting_times.clear();
    driver_distances.clear();
    for (int i = 0; i < drivers.size(); i++){
        double cur_dist = 0;
        double time_for_cooking = cookTimeBox->value() * (driverPaths[i].size() - 2);
        for(int j = 0; j + 1 < driverPaths[i].size(); j++){
            distance += DynamicsTimeOptimizer::real_dist(driverPaths[i][j], driverPaths[i][j+1]);
            cur_dist += DynamicsTimeOptimizer::real_dist(driverPaths[i][j], driverPaths[i][j+1]);
            if (j > 0) {
                sum_order_time += cur_dist / speedSpinBox->value() + time_for_cooking;
                order_waiting_times.push_back(cur_dist / speedSpinBox->value() + time_for_cooking);
            }
        }
        driver_distances.push_back(cur_dist);
    }
    if (orders.size() > 0) sum_order_time /= orders.size();
    distanceLabel->setText("Растояние (пкс): " + QString::number(distance));
    avgTimeLabel->setText("Среднее время ожидания: " + QString::number(sum_order_time));
}

void MainWindow::onAlgoParametersChanged()
{
    int n_orders = orders.size();
    int n_drivers = drivers.size();
    int ant_live = antSpinBox->value();
    bruteforceAlgoButton->setEnabled(n_drivers * pow(3,n_orders) <= 1e11);
    DTOAlgoButton->setEnabled(n_drivers * pow(3,n_orders) <= 1e11);
}

void MainWindow::lockInterface()
{
    antAlgoButton->setEnabled(false);
    bruteforceAlgoButton->setEnabled(false);
    DTOAlgoButton->setEnabled(false);
    generateCafesButton->setEnabled(false);
    generateDriversButton->setEnabled(false);
    generateOrdersButton->setEnabled(false);
    antSpinBox->setEnabled(false);
    speedSpinBox->setEnabled(false);
    cookTimeBox->setEnabled(false);
}

void MainWindow::unlockInterface()
{
    antAlgoButton->setEnabled(true);
    bruteforceAlgoButton->setEnabled(true);
    DTOAlgoButton->setEnabled(true);
    generateCafesButton->setEnabled(true);
    generateDriversButton->setEnabled(true);
    generateOrdersButton->setEnabled(true);
    antSpinBox->setEnabled(true);
    speedSpinBox->setEnabled(true);
    cookTimeBox->setEnabled(true);
    onAlgoParametersChanged();
}

QChart *MainWindow::makeWaitingTimeChart()
{
    QBarSet *barSet = new QBarSet("Время ожидания заказа");
    //QStringList categories;
    for (int i = 0; i < order_waiting_times.size(); i++) {
        //categories << cur_date.toString("dd.MM.yy");
        *barSet << order_waiting_times[i];
    }
    QBarSeries *series = new QBarSeries;
    series->append(barSet);
    QChart *chart = new QChart;
    chart->addSeries(series);
    chart->setTitle("Времена ожидания заказов");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    //axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    return chart;
}

QChart *MainWindow::makeDriverDistsChart()
{
    QBarSet *barSet = new QBarSet("Расстояние, пройденное водителем");
    //QStringList categories;
    for (int i = 0; i < driver_distances.size(); i++) {
        //categories << cur_date.toString("dd.MM.yy");
        *barSet << driver_distances[i];
    }
    QBarSeries *series = new QBarSeries;
    series->append(barSet);
    QChart *chart = new QChart;
    chart->addSeries(series);
    chart->setTitle("Расстояния, пройденные водителями");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    //axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    return chart;
}
