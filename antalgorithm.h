#ifndef ANTALGORITHM_H
#define ANTALGORITHM_H

#include <vector>
#include <cmath>
#include <algorithm>
#include <sstream>

using std::vector;

namespace AntAlgorithm {
    struct Way
    {
        double length;
        vector<bool> tabu;
        vector<int> way;
        int last;
    };
    struct Driver
    {
        double x, y, s, cx, cy;
        int id, num;
    };

    struct Cafe
    {
        double x, y;
    };

    extern double best_length;
    extern double Q, V;
    extern vector<char> used;
    extern size_t n_vertex, n_drivers, n_cafes;
    extern vector<vector<int> > ordersForCafe;
    extern vector<int> t, maxTime, dr;
    extern vector<Driver> driver;
    extern vector<Cafe> cafe;
    extern vector<double> x, y, v, ansTime;
    extern vector<vector<vector<double> > > pheromones;
    extern vector<vector<double> > d, distances;
    extern vector<Way> ant;
    extern std::stringstream cin;
    extern vector<vector<int> > driverPaths;

    extern int LIVE;

    int solve();
}

#endif // ANTALGORITHM_H
