#include <bits/stdc++.h>
#include "json.hpp"                         // nlohmann/json 單一標頭
using json = nlohmann::json;

struct Edge { std::string to; double w; };
struct State {
    std::string v;
    double g, f;                            // g = 累積距離, f = g + h
    bool operator<(const State& other) const { return f > other.f; } // min‑heap
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    /* ---------- 讀取輸入 ---------- */
    int N, M;
    if(!(std::cin >> N >> M)) return 0;
    std::unordered_map<std::string, std::vector<Edge>> graph;

    for (int i = 0; i < M; ++i) {
        std::string u, v; double w;
        std::cin >> u >> v >> w;
        graph[u].push_back({v, w});         // 有向圖
    }

    std::unordered_map<std::string, double> h; // heuristic
    for (int i = 0; i < N; ++i) {
        std::string v; double hv;
        std::cin >> v >> hv;
        h[v] = hv;
    }

    std::string start, goal;
    std::cin >> start >> goal;

    /* ---------- A* 搜尋 ---------- */
    std::priority_queue<State> pq;
    std::unordered_map<std::string, double> g;       // 最佳 g
    std::unordered_map<std::string, std::string> parent;
    const double INF = 1e100;

    for (auto& [v,_] : h) g[v] = INF;
    g[start] = 0;
    pq.push({start, 0, h[start]});

    int expanded = 0;
    while (!pq.empty()) {
        auto [u, gu, fu] = pq.top(); pq.pop();
        ++expanded;
        if (u == goal) break;
        if (gu > g[u]) continue;                     // lazy deletion

        for (auto& [v, w] : graph[u]) {
            double tentative = gu + w;
            if (tentative < g[v]) {
                g[v] = tentative;
                parent[v] = u;
                pq.push({v, tentative, tentative + h[v]});
            }
        }
    }

    /* ---------- 回溯路徑 ---------- */
    std::vector<std::string> path;
    double cost = g[goal];
    for (std::string v = goal; !v.empty(); v = parent.count(v) ? parent[v] : "") {
        path.push_back(v);
    }
    std::reverse(path.begin(), path.end());

    /* ---------- 輸出 JSON ---------- */
    json j;
    j["nodes"] = json::array();
    for (auto& [v, hv] : h) j["nodes"].push_back({{"id", v},{"heuristic", hv}});
    j["edges"] = json::array();
    for (auto& [u, vec] : graph)
        for (auto& e : vec)
            j["edges"].push_back({{"source", u},{"target", e.to},{"weight", e.w}});
    j["path"]    = path;
    j["cost"]    = cost;
    j["expanded_nodes"] = expanded;

    std::ofstream fout("output.json");
    fout << j.dump(2);                        // 美化縮排
    fout.close();

    std::cout << "cost = " << cost << "\n";
    std::cout << "path = ";
    for (size_t i = 0; i < path.size(); ++i)
        std::cout << (i? " -> ":"") << path[i];
    std::cout << '\n';
}
