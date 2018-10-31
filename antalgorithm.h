#ifndef ANTALGORITHM_H
#define ANTALGORITHM_H

#include <vector>
#include <cmath>
#include <algorithm>
#include <sstream>

using std::vector;

class AntAlgorithm {
public:
    AntAlgorithm();
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

    double best_length;
    double Q, V;
    vector<char> used;
    size_t n_vertex, n_drivers, n_cafes;
    vector<vector<int> > ordersForCafe;
    vector<int> t, maxTime, dr;
    vector<Driver> driver;
    vector<Cafe> cafe;
    vector<double> x, y, v, ansTime;
    vector<vector<vector<double> > > pheromones;
    vector<vector<double> > d, distances;
    vector<Way> ant;
    std::stringstream cin;
    vector<vector<int> > driverPaths;

    int LIVE;

    int solve();

    double dist(Driver a, Cafe b);
    static double distForDriver(Driver a);
    static bool cmp(Driver a, Driver b);
    double dist(int i, int j);
    void findNearest();
    void set_d();
    double probability(int to, int pos);
    vector<vector<int> > getSolution(int n_drivers);
};

#endif // ANTALGORITHM_H
