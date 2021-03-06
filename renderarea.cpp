#include "renderarea.h"

#include <QPainter>

RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{
    antialiased = true;
    pixmap.load(":/images/qt-logo.png");

    orderBrush = QBrush(Qt::green);
    driverBrush = QBrush(Qt::red);
    cafeBrush = QBrush(Qt::yellow);

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize RenderArea::sizeHint() const
{
    return QSize(800, 450);
}

void RenderArea::setOrders(const QVector<pair<double, double> > &value)
{
    orders = value;
    update();
}

void RenderArea::setDrivers(const QVector<pair<double, double> > &value)
{
    drivers = value;
    update();
}

void RenderArea::setCafes(const QVector<pair<double, double> > &value)
{
    cafes = value;
    update();
}

void RenderArea::setDriverPaths(const QVector<QVector<pair<double, double> > > &value)
{
    driverPaths = value;
    update();
}

void RenderArea::setAntialiased(bool antialiased)
{
    this->antialiased = antialiased;
    update();
}

void RenderArea::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);

    if (antialiased)
        painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(objectPen);
    drawDriverPaths(painter);
    drawOrders(painter);
    drawDrivers(painter);
    drawCafes(painter);

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}

void RenderArea::drawOrders(QPainter &painter)
{
    painter.setBrush(orderBrush);
    for (pair<double,double> order : orders)
    {
        double x = order.first, y = order.second;
        painter.save();
        painter.translate(x, y);

        painter.drawEllipse(QRect(-15,-15,30,30));
        painter.restore();
    }
}

void RenderArea::drawDrivers(QPainter &painter)
{
    painter.setBrush(driverBrush);
    for (pair<double,double> driver : drivers)
    {
        double x = driver.first, y = driver.second;
        painter.save();
        painter.translate(x, y);

        painter.drawEllipse(QRect(-15,-15,30,30));
        painter.restore();
    }
}

void RenderArea::drawCafes(QPainter &painter)
{
    painter.setBrush(cafeBrush);
    for (pair<double,double> cafe : cafes)
    {
        double x = cafe.first, y = cafe.second;
        painter.save();
        painter.translate(x, y);

        painter.drawEllipse(QRect(-15,-15,30,30));
        painter.restore();
    }
}

int rand_int(int min, int max) {
    return min + rand() % (max - min + 1);
}

void RenderArea::drawDriverPaths(QPainter &painter)
{
    for (int i = 0; i < driverPaths.size(); i++) {
        QColor cur_color(rand_int(0,255),rand_int(0,255),rand_int(0,255));
        painter.save();
        painter.setPen(QPen(QBrush(cur_color),4));
        for (int j = 0; j + 1 < driverPaths[i].size(); j++) {
            painter.drawLine(static_cast<int>(driverPaths[i][j].first),
                             static_cast<int>(driverPaths[i][j].second),
                             static_cast<int>(driverPaths[i][j+1].first),
                             static_cast<int>(driverPaths[i][j+1].second));
        }
        painter.restore();
    }
}
