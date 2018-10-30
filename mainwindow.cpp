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

    generateCafesButton = new QPushButton("Сгренерировать рестораны");
    connect(generateCafesButton, &QPushButton::clicked, this, &MainWindow::generateCafes);

    ordersSpinBox = new QSpinBox;
    ordersSpinBox->setMinimum(1);
    ordersSpinBox->setMaximum(1000);

    driversSpinBox = new QSpinBox;
    driversSpinBox->setMinimum(1);
    driversSpinBox->setMaximum(200);

    cafesSpinBox = new QSpinBox;
    cafesSpinBox->setMinimum(1);
    cafesSpinBox->setMaximum(20);

    antAlgoButton = new QPushButton("Муравьиный алгоритм");
    connect(antAlgoButton, &QPushButton::clicked, this, &MainWindow::launchAntAlgorithm);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(2, 1);
    mainLayout->addWidget(renderArea, 0, 0, 1, 3);
    mainLayout->addWidget(generateOrdersButton, 1, 0);
    mainLayout->addWidget(generateDriversButton, 1, 1);
    mainLayout->addWidget(generateCafesButton, 1, 2);
    mainLayout->addWidget(ordersSpinBox, 2, 0);
    mainLayout->addWidget(driversSpinBox, 2, 1);
    mainLayout->addWidget(cafesSpinBox, 2, 2);
    mainLayout->addWidget(antAlgoButton, 3, 0);
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
    orders.clear();
    for (int i = 0; i < ordersSpinBox->value(); i++) {
        orders.append({randomDouble(10,790), randomDouble(10,440)});
    }
    renderArea->setOrders(orders);
}

void MainWindow::generateDrivers()
{
    drivers.clear();
    for (int i = 0; i < driversSpinBox->value(); i++) {
        drivers.append({randomDouble(10,790), randomDouble(10,440)});
    }
    renderArea->setDrivers(drivers);
}

void MainWindow::generateCafes()
{
    cafes.clear();
    for (int i = 0; i < cafesSpinBox->value(); i++) {
        cafes.append({randomDouble(10,790), randomDouble(10,440)});
    }
    renderArea->setCafes(cafes);
}

void MainWindow::launchAntAlgorithm()
{
    antAlgoButton->setEnabled(false);
    cout.clear();
    cout << orders.size() << "\n";
    for (int i = 0; i < orders.size(); i++) {
        cout << orders[i].first << " " << orders[i].second << " 0\n";
    }
    cout << drivers.size() << "\n";
    for (int i = 0; i < drivers.size(); i++) {
        cout << drivers[i].first << " " << drivers[i].second << " 0\n";
    }
    cout << cafes.size() << "\n";
    for (int i = 0; i < cafes.size(); i++) {
        cout << cafes[i].first << " " << cafes[i].second << "\n";
    }
    cout << "1\n";

    AntAlgorithm::cin.swap(cout);
    disconnect(this, SLOT(antAlgoFinished()));
    connect(&algoWatcher, &QFutureWatcher<int>::finished, this, &MainWindow::antAlgoFinished);
    algoResult = QtConcurrent::run( AntAlgorithm::solve );
    algoWatcher.setFuture(algoResult);
}

void MainWindow::antAlgoFinished()
{
    driverPaths.clear();
    for (int i = 0; i < drivers.size(); i++) {
        driverPaths.append(QVector< pair<double,double> >());
        driverPaths[i].append(drivers[i]);
        driverPaths[i].append(cafes[AntAlgorithm::driver[i].id]);
        for(size_t j = 0; j < AntAlgorithm::driverPaths[i].size(); j++) {
            int order_num = AntAlgorithm::driverPaths[i][j];
            pair<double,double> order_coords = orders[order_num];
            driverPaths[i].append(order_coords);
        }
    }
    renderArea->setDriverPaths(driverPaths);
    antAlgoButton->setEnabled(true);
}
