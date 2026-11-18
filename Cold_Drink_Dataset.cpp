/*
 * PRN  : ____________________   (fill in your full PRN)
 * Name : ____________________
 *
 * Personalization key
 *   Seed S            = 159 (last 4 digits of PRN)
 *   Domain scenario   = Cold-drink flavours   (PRN mod 3 = 0)
 *   Dataset           = v[i] = (S + 13*i) % 100, i = 0..19
 *   Hash function     = Mid-square            (PRN is odd)
 *   Collision method  = Separate chaining     (PRN mod 2 = 1)
 *   Table size m      = 37                    (smallest prime >= (159 % 23) + 11)
 *
 * Mid-square used: square the key, zero-pad to 4 digits, take the
 * middle 2 digits, then take (middle) mod m.
 */
#include <iostream>
#include <list>
#include <string>
#include <vector>
using namespace std;

struct Flavour {
    int code;
    string name;
};

class HashTable {
    int m;
    vector<list<Flavour>> table;
    int collisions = 0;

public:
    explicit HashTable(int size) : m(size), table(size) {}

    int hash(int key) const {
        int sq = key * key;           // e.g. 59 -> 3481
        int mid = (sq / 10) % 100;    // middle two digits of a 4-digit square
        return mid % m;
    }

    bool insert(const Flavour& f) {
        int idx = hash(f.code);
        for (const auto& x : table[idx])
            if (x.code == f.code) return false;   // duplicate key rejected
        if (!table[idx].empty()) collisions++;
        table[idx].push_back(f);
        return true;
    }

    const Flavour* search(int code) const {
        for (const auto& x : table[hash(code)])
            if (x.code == code) return &x;
        return nullptr;
    }

    void printChain(int idx) const {
        cout << idx << " -> ";
        bool first = true;
        for (const auto& x : table[idx]) {
            if (!first) cout << " -> ";
            cout << x.code;
            first = false;
        }
        cout << "\n";
    }

    void display() const {
        cout << "\nFinal hash table (non-empty buckets):\n";
        for (int i = 0; i < m; i++)
            if (!table[i].empty()) printChain(i);
    }

    int getCollisions() const { return collisions; }
};

int main() {
    const int S = 159, M = 37;
    const string names[20] = {
        "Cola", "Orange", "Lemon-Lime", "Root Beer", "Mango",
        "Grape", "Pineapple", "Strawberry", "Cherry", "Peach",
        "Watermelon", "Lychee", "Green Apple", "Blueberry", "Guava",
        "Raspberry", "Coconut", "Ginger Ale", "Kiwi", "Vanilla"};

    HashTable ht(M);
    cout << "Insertion trace (mid-square, m = " << M << ")\n";
    for (int i = 0; i < 20; i++) {
        int key = (S + 13 * i) % 100;
        Flavour f{key, names[i]};
        int idx = ht.hash(key);
        ht.insert(f);
        cout << "insert " << key << " (" << f.name << ") -> index " << idx
             << " | chain: ";
        ht.printChain(idx);
    }
    ht.display();
    cout << "\nCollisions: " << ht.getCollisions() << "\n";
    cout << "Load factor: " << 20.0 / M << "\n";

    int probe[] = {80, 6, 99};
    for (int k : probe) {
        const Flavour* r = ht.search(k);
        cout << "search " << k << ": "
             << (r ? r->name : string("not found")) << "\n";
    }
    return 0;
}