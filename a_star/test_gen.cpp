#include <bits/stdc++.h>
using namespace std;

/* ===== 幫助函式 ===== */
struct Coord { double x, y; };
double dist(const Coord& a, const Coord& b) {
    return hypot(a.x - b.x, a.y - b.y);
}

int main(int argc, char* argv[]) {
    /* -------- 參數設定 --------
       用法：
         test_gen <N> <M> [seed]  > sample.txt
       N = 節點數 (>= 2)，M = 邊數 (>= N-1)
    */
    if (argc < 3) {
        cerr << "用法: test_gen <N> <M> [seed] > sample.txt\n";
        return 1;
    }
    int N = stoi(argv[1]);
    int M = stoi(argv[2]);
    if (N < 2 || M < N - 1) {
        cerr << "需滿足 N >= 2 且 M >= N-1\n";
        return 1;
    }
    size_t seed = (argc >= 4) ? stoull(argv[3]) : random_device{}();
    mt19937 rng(seed);
    uniform_real_distribution<double> pos(0.0, 100.0);   // 範圍 [0,100] 的座標
    uniform_int_distribution<int> pick(0, N - 1);

    /* -------- 產生座標與節點名稱 -------- */
    vector<Coord> coord(N);
    for (auto& c : coord) c = {pos(rng), pos(rng)};

    auto idName = [&](int i) {
        if (i == 0) return string("S");
        if (i == N - 1) return string("T");
        return string("V") + to_string(i);
    };

    /* -------- 先建保證可達的「骨幹路徑」 --------
       S -> V1 -> V2 -> ... -> T
    */
    vector<tuple<int,int,int>> edges;
    for (int i = 0; i < N - 1; ++i) {
        int u = i, v = i + 1;
        int w = max(1, int(round(dist(coord[u], coord[v]))));
        edges.emplace_back(u, v, w);
    }

    /* -------- 再隨機加邊到 M 條 -------- */
    while (edges.size() < size_t(M)) {
        int u = pick(rng);
        int v = pick(rng);
        if (u == v) continue;
        if (u == N - 1) continue;               // 目標節點不發出邊
        if (any_of(edges.begin(), edges.end(),
                   [&](auto& e){return get<0>(e)==u && get<1>(e)==v;}))
            continue;                           // 重複邊跳過
        int w = max(1, int(round(dist(coord[u], coord[v]))));
        edges.emplace_back(u, v, w);
    }

    /* -------- 啟發式 = 直線距離到 T -------- */
    vector<int> h(N);
    for (int i = 0; i < N; ++i) {
        h[i] = int(round(dist(coord[i], coord[N - 1])));  // admissible
    }

    /* --------- 輸出 --------- */
    cout << N << ' ' << M << '\n';
    for (auto& [u, v, w] : edges)
        cout << idName(u) << ' ' << idName(v) << ' ' << w << '\n';

    for (int i = 0; i < N; ++i)
        cout << idName(i) << ' ' << h[i] << '\n';

    cout << idName(0) << ' ' << idName(N - 1) << '\n';
}
