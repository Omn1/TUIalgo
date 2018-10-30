#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include <vector>
#include <QVector>
#include <QPainter>

using std::pair;
using std::vector;

class RenderArea : public QWidget
{
    Q_OBJECT

public:
    enum Shape { Line, Points, Polyline, Polygon, Rect, RoundedRect, Ellipse, Arc,
                 Chord, Pie, Path, Text, Pixmap };

    RenderArea(QWidget *parent = nullptr);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void setOrders(const QVector<pair<double,double> > &value);

    void setDrivers(const QVector<pair<double, double> > &value);

    void setCafes(const QVector<pair<double, double> > &value);

    void setDriverPaths(const QVector<QVector<pair<double, double> > > &value);

public slots:
    void setShape(Shape shape);
    void setAntialiased(bool antialiased);
    void setTransformed(bool transformed);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawOrders(QPainter &painter);
    void drawDrivers(QPainter &painter);
    void drawCafes(QPainter &painter);
    void drawDriverPaths(QPainter &painter);

    Shape shape;
    QPen objectPen;
    QBrush orderBrush;
    QBrush driverBrush;
    QBrush cafeBrush;
    bool antialiased;
    bool transformed;
    QPixmap pixmap;
    QVector< pair<double,double> > orders;
    QVector< pair<double,double> > drivers;
    QVector< pair<double,double> > cafes;
    QVector< QVector< pair<double,double> > > driverPaths;
};

#endif // RENDERAREA_H
