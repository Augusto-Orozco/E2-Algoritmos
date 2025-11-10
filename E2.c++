#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

struct Edge {
    int u, v;
    double w;
    bool operator<(Edge const& other) const {
        return w < other.w;
    }
};

struct DSU {
    int n;
    vector<int> p, r;
    DSU(int n): n(n), p(n), r(n,0) { iota(p.begin(), p.end(), 0); }
    int find(int x){ return p[x]==x? x : p[x]=find(p[x]); }
    bool unite(int a, int b){
        a = find(a); b = find(b);
        if (a==b) return false;
        if (r[a] < r[b]) swap(a,b);
        p[b]=a;
        if (r[a]==r[b]) r[a]++;
        return true;
    }
};

int main(int argc, char* argv[]){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <archivo_entrada>\n";
        return 1;
    }

    ifstream fin(argv[1]);
    if (!fin) {
        cerr << "No se pudo abrir el archivo: " << argv[1] << "\n";
        return 1;
    }

    int n;
    if (!(fin >> n)) {
        cerr << "Formato inválido: no se leyó el número de nodos.\n";
        return 1;
    }
    if (n <= 0) {
        cerr << "Número de nodos debe ser positivo.\n";
        return 1;
    }

    vector<vector<double>> mat(n, vector<double>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (!(fin >> mat[i][j])) {
                cerr << "Formato inválido: faltan entradas en la matriz.\n";
                return 1;
            }
        }
    }

    // Construir lista de aristas: considerar solo i<j y pesos > 0
    vector<Edge> edges;
    for (int i = 0; i < n; ++i) {
        for (int j = i+1; j < n; ++j) {
            double w = mat[i][j];
            // ignorar si w <= 0 (interpretado como no arista)
            if (w > 0 && isfinite(w)) {
                edges.push_back({i, j, w});
            }
        }
    }

    // Si no hay aristas suficientes, no es conexo
    if ((int)edges.size() < n-1) {
        cout << "No hay suficientes aristas para conectar todas las colonias.\n";
        return 0;
    }

    sort(edges.begin(), edges.end());

    DSU dsu(n);
    vector<Edge> mst;
    double total = 0.0;

    for (auto &e : edges) {
        if (dsu.unite(e.u, e.v)) {
            mst.push_back(e);
            total += e.w;
            if ((int)mst.size() == n-1) break;
        }
    }

    if ((int)mst.size() != n-1) {
        cout << "El grafo no es conexo: no es posible conectar todas las colonias.\n";
        return 0;
    }

    cout.setf(std::ios::fixed); cout << setprecision(2);
    cout << "Cableado óptimo (Árbol de expansión mínima) encontrado:\n";
    cout << "Aristas (colonia_a -- colonia_b) : distancia (km)\n";
    for (auto &e : mst) {
        cout << "Colonia " << e.u << " -- Colonia " << e.v << " : " << e.w << " km\n";
    }
    cout << "Distancia total mínima requerida: " << total << " km\n";

    return 0;
}
