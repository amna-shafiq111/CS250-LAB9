// CS250 Lab 9 - Task 3: Weighted M-ary Tree - Galaxy Representation
// Name: Amna Shafiq | CMS-ID: 552522 | BSDS 3A
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class StarSystem;

// Edge (travel route) with a cost
class TravelRoute {
public:
    float travel_cost;
    StarSystem* destination;
    TravelRoute(float cost = 0.0f, StarSystem* dest = nullptr)
        : travel_cost(cost), destination(dest) {}
};

// Node: a star system
class StarSystem {
public:
    int system_id;
    string name;
    int num_planets;
    double x, y, z;
    bool is_habitable;
    vector<TravelRoute> routes; // outgoing edges to child systems

    StarSystem(int id, string n, int planets, double cx, double cy, double cz, bool hab)
        : system_id(id), name(n), num_planets(planets), x(cx), y(cy), z(cz), is_habitable(hab) {}
};

// Manages the M-ary tree
class Galaxy {
    int M; // max children per node
    StarSystem* root;
    vector<StarSystem*> all; // for cleanup

    // number of nodes in subtree rooted at x
    int subtree_size(StarSystem* x) const {
        if (!x) return 0;
        int s = 1;
        for (const auto& r : x->routes) s += subtree_size(r.destination);
        return s;
    }
public:
    // Overloaded constructors
    Galaxy(int m) : M(m), root(nullptr) {}
    Galaxy(int m, StarSystem* r) : M(m), root(r) { if (r) all.push_back(r); }
    ~Galaxy() { for (auto s : all) delete s; }

    StarSystem* getRoot() const { return root; }
    void setRoot(StarSystem* r) { root = r; all.push_back(r); }

    // Add child under parent with a travel cost; fails if parent already has M children
    bool add_child(StarSystem* parent, StarSystem* child, float cost) {
        if (!parent || (int)parent->routes.size() >= M) return false;
        parent->routes.push_back(TravelRoute(cost, child));
        all.push_back(child);
        return true;
    }

    // Balanced: for every node, sizes of all child subtrees differ by at most 1

    bool is_balanced(StarSystem* x) const {
        if (!x || x->routes.empty()) return true;
        int mn = 1 << 30, mx = -1;
        for (const auto& r : x->routes) {
            if (!is_balanced(r.destination)) return false;
            int sz = subtree_size(r.destination);
            mn = min(mn, sz); mx = max(mx, sz);
        }
        return (mx - mn) <= 1;
    }

    // Count leaf nodes in subtree rooted at x
    int count_leaves(StarSystem* x) const {
        if (!x) return 0;
        if (x->routes.empty()) return 1;
        int c = 0;
        for (const auto& r : x->routes) c += count_leaves(r.destination);
        return c;
    }

    void print(StarSystem* x, int depth = 0) const {
        if (!x) return;
        cout << string(depth * 4, ' ') << "- " << x->name << " (id=" << x->system_id
              << ", planets=" << x->num_planets << ", habitable=" << (x->is_habitable ? "yes" : "no") << ")\n";
        for (const auto& r : x->routes) {
            cout << string(depth * 4 +2, ' ') << "[cost " << r.travel_cost << "]\n";
            print(r.destination, depth + 1);
        }
    }
};

int main() {
    cout << "===== TASK 3: Weighted M-ary Tree - Galaxy =====\n";

    // ---- Galaxy 1: balanced ternary tree (M = 3) ----
    Galaxy g1(3, new StarSystem(1, "Sol", 8, 0, 0, 0, true));
    StarSystem* r = g1.getRoot();
    StarSystem* a = new StarSystem(2, "Alpha Centauri", 3, 4.3, 0, 0, true);
    StarSystem* b = new StarSystem(3, "Sirius", 2, 8.6, 1, 2, false);
    StarSystem* c = new StarSystem(4, "Vega", 5, 25, 3, 1, true);
    g1.add_child(r, a, 4.3f); g1.add_child(r, b, 8.6f); g1.add_child(r, c, 25.0f);
    g1.add_child(a, new StarSystem(5, "Proxima", 1, 4.2, 0, 0, false), 0.1f);
    g1.add_child(b, new StarSystem(6, "Sirius B", 0, 8.6, 1, 2, false), 0.2f);
    g1.add_child(c, new StarSystem(7, "Vega B", 2, 25, 3, 1, true), 1.5f);

    cout << "\nGalaxy 1 (M=3):\n"; g1.print(r);
    cout << "Leaf count : " << g1.count_leaves(r) << "\n";
    cout << "Balanced   : " << (g1.is_balanced(r) ? "YES" : "NO") << "\n";

    // ---- Galaxy 2: unbalanced (one child subtree much bigger) ----
    Galaxy g2(3);
    StarSystem* r2 = new StarSystem(10, "Orion", 4, 0, 0, 0, true);
    g2.setRoot(r2);
    StarSystem* p = new StarSystem(11, "Rigel", 2, 1, 1, 1, false);
    StarSystem* q = new StarSystem(12, "Betelgeuse", 1, 2, 2, 2, false);
    g2.add_child(r2, p, 3.0f); g2.add_child(r2, q, 6.0f);
    StarSystem* p1 = new StarSystem(13, "Rigel A", 1, 1, 1, 2, false);
    g2.add_child(p, p1, 1.0f);
    g2.add_child(p1, new StarSystem(14, "Rigel A1", 0, 1, 2, 2, false), 0.5f);
    g2.add_child(p1, new StarSystem(15, "Rigel A2", 3, 2, 2, 2, true), 0.7f);

    cout << "\nGalaxy 2 (M=3):\n"; g2.print(r2);
    cout << "Leaf count : " << g2.count_leaves(r2) << "\n";
    cout << "Balanced   : " << (g2.is_balanced(r2) ? "YES" : "NO") << "\n";
    return 0;
}
