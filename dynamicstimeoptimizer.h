#ifndef DYNAMICSTIMEOPTIMIZER_H
#define DYNAMICSTIMEOPTIMIZER_H

#include <QVector>
#include <cmath>

using std::pair;

class DynamicsTimeOptimizer
{
public:
    DynamicsTimeOptimizer();

    int solve();

    QVector<QVector<double> > dp;

    QVector<QVector<int> > prev_dp;

    QVector<QVector<double > > order_dp;

    QVector<QVector<int> > prev_order_dp;

    QVector<int> drivers_cafe;

    QVector<pair<double,double> > drivers;

    QVector<pair<double,double> > cafes;

    QVector<pair<double,double> > orders;

    QVector<int> drivers_mask;

    QVector<QVector<int> > driversPath;

    static double real_dist(pair<double,double> a, pair<double,double> b);

    double dist(pair<double,double> a, pair<double,double> b);
    static const double inf;

    double driver_speed;

    double avg_cook_time;

private:
    void init_vars();
    double get_cafe_orders_dist(int cafe, int msk);
    int get_first_order(int cafe, int msk);
    void calc_dp();
    void calc_order_dp();
    void calc_order_dist();
    void calc_driver_orders_dists();
    void restore_answers();
    void restore_driver(int driver);
    int mxmask;
    int n_drivers, n_orders, n_cafes;
    QVector<QVector<double> > order_dist;
    QVector<QVector<double> > driver_orders_dist;
    QVector<QVector<int> > driver_orders_cafe;
    QVector<int> mask_bits;
};

#endif // DYNAMICSTIMEOPTIMIZER_H
