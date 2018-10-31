#include "antalgorithm.h"

#define pb push_back

#define ALPHA	0.5
#define BETA	3

#define M		20
#define DELTA   0.5
#define pi      3.1415926
#define EX      7

using std::vector;
using std::sort;
using std::random_shuffle;
using std::max;
using std::min;

double AntAlgorithm::dist(Driver A, Cafe B)
{
    return sqrt((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y + B.y));
}

double AntAlgorithm::distForDriver(Driver a) {
    return a.s + sqrt((a.x - a.cx) * (a.x - a.cx) + (a.y - a.cy) * (a.y - a.cy));
}

bool AntAlgorithm::cmp(Driver a, Driver b)
{
    int xa = distForDriver(a), xb = distForDriver(b);
    return xa < xb;
}

double AntAlgorithm::dist(int i, int j)
{
    return sqrt((x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j]));
}

void AntAlgorithm::findNearest() {
    for (int i = 0; i < n_drivers; i++)
    {
        double mi = 1000000000;
        int c = -1;
        for (int j = 0; j < n_cafes; j++)
            if (c == -1 || dist(driver[i], cafe[j]) < mi)
                mi = dist(driver[i], cafe[j]), c = j;
        driver[i].cx = cafe[c].x;
        driver[i].cy = cafe[c].y;
        driver[i].id = c;
    }
}


void AntAlgorithm::set_d()
{
    d.resize(n_vertex+n_cafes, vector<double>(n_vertex+n_cafes));
    for (int i = 0; i < n_vertex+n_cafes; i++)
        for (int j = 0; j < n_vertex+n_cafes; j++)
            d[i][j] = dist(i, j);
    distances.resize(n_vertex + n_cafes, vector<double>(n_vertex + n_cafes));
    pheromones.resize(n_drivers, vector<vector<double> >(n_vertex + n_cafes, vector<double>(n_vertex + n_cafes)));
    for (int pos = 0; pos < n_drivers; pos++)
        for (int i = 0; i < n_vertex + n_cafes; i++)
            for (int j = 0; j < n_vertex + n_cafes; j++)
            {
                pheromones[pos][i][j] = 1.0 / n_vertex;
                if (i != j)
                    if (d[i][j] != 0)
                        distances[i][j] = 1.0 / d[i][j];
                    else
                        distances[i][j] = 100000000;
            }
}

double AntAlgorithm::probability(int to, int pos) {
    if (used[to] || ant[pos].tabu[to])
        return 0;
    int from = ant[pos].last;
    return pow(pheromones[pos][from][to], ALPHA) * pow(distances[from][to], BETA);
}

vector<vector<int> > AntAlgorithm::getSolution(int n_drivers)
{
    findNearest();
    sort(driver.begin(), driver.end(), cmp);
    set_d();
    ant.resize(n_drivers);
    vector<vector<int> > best;
    best_length = 100000000;
    vector<int> vert;
    for (int i = 0; i < n_vertex; i++)
        vert.pb(i);
    for (int t = 0; t < LIVE; t++)
    {
        random_shuffle(vert.begin(), vert.end());
        for (int i = 0; i < n_drivers; i++) {
            ant[i].length = distForDriver(driver[i]);
            ant[i].tabu.clear();
            ant[i].tabu.resize(n_vertex, 0);
            ant[i].last = driver[i].id + n_vertex;
            ant[i].way.clear();
        }

        for (int i = 0; i < n_vertex; i++)
            for (int j = 0; j < driver.size(); j++)
                if (x[i] == driver[j].cx && y[i] == driver[j].cy)
                {
                    used[i] = 1;
                    ant[j].way.pb(i);
                    break;
                }
        double total_length = 0;
        while(1) {
            int pos = -1;
            for (int i = 0; i < n_drivers; i++)
                if (pos == - 1 || ant[i].length < ant[pos].length)
                    pos = i;
            //cout << pos << endl;
            double sum = 0;
            int from = ant[pos].last;
            for (int i = 0; i < n_vertex; i++)
            {
                if (used[i] || ant[pos].tabu[i])
                    continue;
                //cout << '*' << i << endl;
                sum += pow(pheromones[pos][from][i], ALPHA) * pow(distances[from][i], BETA);
            }
            if (sum == 0)
                break;
            //cout << pos;
            for (int i = 0; i < n_vertex; i++)
            {
                double p = probability(i, pos) / sum * 100;
                v[i] = ((i == 0) ? 0 : v[i - 1]) + p;
            }
            //cout << ' ' << 1 << endl;
            double r = (rand() % 9999 + 1) * 1.0 / 100;
            int go = -1;
            for (int i = 0; i < n_vertex; i++)
                if (v[i] >= r && !ant[pos].tabu[i])
                {
                    go = i;
                    break;
                }
            //cout << '-' << go << endl;
            total_length += dist(ant[pos].last, go);
            if (total_length > best_length)
                break;
            //cout << pos << ' ' << go << endl;
            ant[pos].length += dist(ant[pos].last, go);
            ant[pos].tabu[go] = 1;
            ant[pos].way.pb(go);
            ant[pos].last = go;
            //cout << ant[1].way.size()<<endl;
            for (int i = 0; i < n_drivers; i++) {
                ant[i].tabu[go] = 1;
            }
        }
        //cout << "o";
        for (int i = 0; i < n_drivers; i++)
            for (int j = 0; j + 1 < ant[i].way.size(); j++) {
                int from = ant[i].way[j], to = ant[i].way[j];
                pheromones[i][from][to] += Q * 1.0 / ant[i].length;
                pheromones[i][to][from] = pheromones[i][from][to];
            }
        for (int i = 0; i < n_drivers; i++)
            for (int a = 0; a < n_vertex; a++)
                for (int b = 0; b < n_vertex; b++)
                    pheromones[i][a][b] *= (1-DELTA), pheromones[i][a][b] = max(pheromones[i][a][b], 1.0 / n_vertex);
        if (total_length < best_length) {
            best.clear();
          //  cout << ant[1].way.size();
            for (int i = 0; i < n_drivers; i++)
                best.pb(ant[i].way);
            //cout << best[1].size();
            best_length = total_length;
        }
    }
    vector<vector<int> > ans;
    ans.resize(best.size());
    for (int i = 0; i < best.size(); i++)
        ans[driver[i].num] = best[i];
    best = ans;

    return best;
}



AntAlgorithm::AntAlgorithm()
{

}

int AntAlgorithm::solve()
{
    //srand(time(NULL));
    /* Read and resize*/
    //cout << "Enter the number of orders\n";
    cin >> n_vertex;
    x.resize(n_vertex);
    y.resize(n_vertex);
    v.resize(n_vertex, 0);
    t.resize(n_vertex);
    //cout << "Enter all orders(coordinates and time for cooking)\n";
    for (int i = 0; i < n_vertex; i++)
        cin  >> x[i] >> y[i] >> t[i];
    //cout << "Enter the number of drivers\n";
    cin >> n_drivers;
    //cout << "Enter all drivers(coordinates and distance till end)\n";
    driver.resize(n_drivers);
    for (int i = 0; i < n_drivers; i++)
        cin >> driver[i].x >> driver[i].y >> driver[i].s, driver[i].num = i;
    //cout << "Enter the number of cafe\n";
    cin >> n_cafes;
    cafe.resize(n_cafes);
    for (int i = 0; i < n_cafes; i++)
        x.pb(0), y.pb(0);
    //cout << "Enter all cafes(coordinates)\n";
    for (int i = 0; i < n_cafes; i++)
        cin >> cafe[i].x >> cafe[i].y, x[i + n_vertex] = cafe[i].x, y[i + n_vertex] = cafe[i].y;
    //cout << "Enter driver's speed\n";
    cin >> V;
    used.resize(n_vertex + n_cafes);

    ///solve
    vector<vector<int> > best = getSolution(min(n_drivers, n_vertex));
    driverPaths = best;
    return 0;
}
