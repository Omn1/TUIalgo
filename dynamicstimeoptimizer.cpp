#include "dynamicstimeoptimizer.h"

using std::min;

const double DynamicsTimeOptimizer::inf = 1e9;

DynamicsTimeOptimizer::DynamicsTimeOptimizer()
{

}

int DynamicsTimeOptimizer::solve()
{
    init_vars();
    calc_order_dist();
    calc_order_dp();
    calc_driver_orders_dists();
    calc_dp();
    restore_answers();
}

void DynamicsTimeOptimizer::calc_dp()
{
    for(int i = 0; i < n_drivers; i++) {
        if (i==0) {
            for (int msk = 0; msk < mxmask; msk++) {
                dp[i][msk] = driver_orders_dist[i][msk];
                prev_dp[i][msk] = 0;
            }
        }
        else{
            for (int msk = 0; msk < mxmask; msk++) {
                dp[i][msk] = dp[i-1][msk];
                prev_dp[i][msk] = msk;
                for (int submsk = msk; submsk > 0; submsk = (submsk-1)&msk) {
                    if ( dp[i-1][submsk] + driver_orders_dist[i][msk^submsk] < dp[i][msk]) {
                        dp[i][msk] = dp[i-1][submsk] + driver_orders_dist[i][msk^submsk];
                        prev_dp[i][msk] = submsk;
                    }
                }
            }
        }
    }
}

void DynamicsTimeOptimizer::calc_order_dp()
{
    mxmask = 1 << n_orders;
    for(int msk = 0; msk < mxmask; msk++) {
        int bits = mask_bits[msk];
        for(int i = 0; i < n_orders; i++) {
            if ( (msk>>i)&1 ) {
                if(bits == 1) {
                    order_dp[i][msk] = 0;
                    break;
                }
                msk ^= 1<<i;
                double best = inf;
                int best_j = 0;
                for (int j = 0; j < n_orders; j++) {
                    if ( (msk>>j)&1 ) {
                        if ( order_dp[j][msk] + mask_bits[msk] * order_dist[i][j] < best) {
                            best = order_dp[j][msk] + mask_bits[msk] * order_dist[i][j];
                            best_j = j;
                        }
                    }
                }
                msk ^= 1<<i;
                order_dp[i][msk] = best;
                prev_order_dp[i][msk] = best_j;
            }
        }
    }
}

void DynamicsTimeOptimizer::calc_order_dist()
{
    for(int i = 0; i < n_orders; i++) {
        for(int j = 0; j < n_orders; j++) {
            order_dist[i][j] = dist(orders[i], orders[j]);
        }
    }
}

void DynamicsTimeOptimizer::calc_driver_orders_dists()
{
    for(int msk = 0; msk < mxmask; msk++) {
        for(int i = 0; i < n_cafes; i++) {
            for(int j = 0; j < n_drivers; j++) {
                double new_dist = get_cafe_orders_dist(i,msk) + mask_bits[msk] * dist(drivers[j], cafes[i]);
                if (new_dist < driver_orders_dist[j][msk]) {
                    driver_orders_dist[j][msk] = new_dist;
                    driver_orders_cafe[j][msk] = i;
                }
            }
        }
    }
}

void DynamicsTimeOptimizer::restore_answers()
{
    int cur_driver = n_drivers - 1;
    int cur_msk = mxmask - 1;
    while(cur_driver > 0) {
        drivers_mask[cur_driver] = cur_msk ^ prev_dp[cur_driver][cur_msk];
        cur_msk = prev_dp[cur_driver][cur_msk];
        cur_driver--;
    }
    drivers_mask[0] = cur_msk;
    for(int i = 0; i < n_drivers; i++) restore_driver(i);
}

void DynamicsTimeOptimizer::restore_driver(int driver)
{
    int msk = drivers_mask[driver];
    drivers_cafe[driver] = driver_orders_cafe[driver][msk];
    if (!msk) return;
    int cur_order = get_first_order(drivers_cafe[driver], msk);
    while (cur_order != -1) {
        driversPath[driver].append(cur_order);
        int new_order = prev_order_dp[cur_order][msk];
        msk ^= 1<<cur_order;
        cur_order = new_order;
    }
}

double DynamicsTimeOptimizer::dist(pair<double, double> a, pair<double, double> b)
{
    return sqrt((a.first - b.first)*(a.first - b.first) + (a.second - b.second) * (a.second - b.second));
}

void DynamicsTimeOptimizer::init_vars()
{
    n_drivers = drivers.size();
    n_orders = orders.size();
    n_cafes = cafes.size();
    mxmask = 1 << n_orders;
    order_dp = QVector<QVector<double> > (n_orders, QVector<double>(mxmask, inf));
    prev_order_dp = QVector<QVector<int> > (n_orders, QVector<int>(mxmask, -1));
    dp = QVector<QVector<double> > (n_drivers, QVector<double> (mxmask, inf));
    prev_dp = QVector<QVector<int> > (n_drivers, QVector<int> (mxmask, 0));
    order_dist = QVector<QVector<double> > (n_orders, QVector<double>(n_orders));
    driver_orders_dist = QVector<QVector<double> > (n_drivers, QVector<double>(mxmask, inf));
    driver_orders_cafe = QVector<QVector<int> > (n_drivers, QVector<int> (mxmask, -1));
    drivers_mask = QVector<int> (n_drivers, 0);
    drivers_cafe = QVector<int> (n_drivers, 0);
    driversPath.resize(n_drivers);
    mask_bits.resize(mxmask);
    for (int i = 0; i < mxmask; i++) {
        for(int j = 0; j < n_orders; j++) mask_bits[i] += (i>>j)&1;
    }
}

double DynamicsTimeOptimizer::get_cafe_orders_dist(int cafe, int msk)
{
    double best = inf;
    for(int i = 0; i < n_orders; i++) {
        if ( (msk>>i)&1 ) {
            best = min(best, order_dp[i][msk] + mask_bits[msk] * dist(cafes[cafe], orders[i]));
        }
    }
    return best;
}

int DynamicsTimeOptimizer::get_first_order(int cafe, int msk)
{
    double best = inf;
    double best_i = -1;
    for(int i = 0; i < n_orders; i++) {
        if( (msk>>i)&1 ){
            if(order_dp[i][msk] + mask_bits[msk] * dist(cafes[cafe], orders[i]) < best){
                best = order_dp[i][msk] + mask_bits[msk] * dist(cafes[cafe], orders[i]);
                best_i = i;
            }
        }
    }
    return best_i;
}
