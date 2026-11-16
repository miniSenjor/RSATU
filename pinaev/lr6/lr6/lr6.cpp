#include <iostream>
#include <vector>
#include <map>
#include <windows.h>
#include <queue>
#include <set>
/*Топологическая сортировка Алгоритм Вирта*/
using namespace std;

vector<int> wirthTopoSort(const map<int, vector<int>>& adj) {
    set<int> nodes;
    for (const auto& pair : adj) {
        nodes.insert(pair.first);
        for (int to : pair.second)
            nodes.insert(to);
    }
    map<int, int> indeg;
    for (int v : nodes)
        indeg[v] = 0;

    for (const auto& pair : adj)
        for (int to : pair.second)
            ++indeg[to];

    queue<int> q;
    for (int v : nodes)
        if (indeg[v] == 0)
            q.push(v);

    vector<int> order;
    order.reserve(nodes.size());

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        order.push_back(v);

        auto it = adj.find(v);
        if (it != adj.end()) {
            for (int to : it->second) {
                if (--indeg[to] == 0)
                    q.push(to);
            }
        }
    }

    if (order.size() != nodes.size())
    {
        cout << "Есть цикл" << endl;
    }

    return order;
}


int main()
{
    SetConsoleOutputCP(1251);
    cout << "Введите кол-во вершин ";
    int n;
    cin >> n;
    /*cout << "Введите кол-во ребер ";
    cin >> m;*/

    map<int, vector<int>> adj;
    for (int i = 1; i <= n; i++)
    {
        adj[i] = vector<int>();
    }
    int u, v;
    cout << "Введите ребро ";
    cin >> u;
    if (u != 0)
        cin >> v;
    while (u != 0 && v != 0)
    {
        cout << "Введите ребро ";
        adj[u].push_back(v);
        cin >> u;
        if (u == 0)
            break;
        cin >> v;
    }
    /*for (int i = 0; i < m; ++i) {
        int u, v;
        cout << "Введите ребро ";
        cin >> u >> v;
        adj[u].push_back(v);
    }*/

    vector<int> res = wirthTopoSort(adj);
    if (res.empty())
        cout << "Цикл\n";
    else
        for (int v : res) cout << v << ' ';
}
