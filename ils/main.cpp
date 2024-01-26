#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Bin {
    int currWeight;
    vector<int> items;

    bool operator<(const Bin& other) const {
        return currWeight > other.currWeight;
    }
};

vector<Bin> first_fit(vector<int>& w, int c) {
    vector<Bin> bins;
    for (int i = 0; i < w.size(); i++) {
        int item = w[i];
        bool fit = false;
        for (int j = 0; j < bins.size(); j++) {
            int currWeight = bins[j].currWeight;
            if (currWeight + item <= c) {
                bins[j].currWeight += item;
                bins[j].items.push_back(i);
                fit = true;
                break;
            }
        }
        if (!fit) {
            bins.push_back({item, {i}});
        }
    }
    return bins;
}

vector<Bin> best_fit(vector<int>& w, int c) {
    vector<Bin> bins;
    for (int i = 0; i < w.size(); i++) {
        int item = w[i];
        bool fit = false;
        for (int j = 0; j < bins.size(); j++) {
            int currWeight = bins[j].currWeight;
            if (currWeight + item <= c) {
                bins[j].currWeight += item;
                bins[j].items.push_back(i);
                fit = true;
                break;
            }
        }
        if (!fit) {
            bins.push_back({item, {i}});
        }
        sort(bins.begin(), bins.end());
    }
    return bins;
}

vector<Bin> worst_fit(vector<int>& w, int c) {
    vector<Bin> bins;
    priority_queue<Bin> pq;

    for (int i = 0; i < w.size(); i++) {
        int item = w[i];
        if (pq.size() == 0 || pq.top().currWeight + item > c) {
            pq.push({item, {i}});
        } else {
            Bin b = pq.top();
            pq.pop();
            b.currWeight += item;
            b.items.push_back(i);
            pq.push(b);
        }
    }

    while (!pq.empty()) {
        bins.push_back(pq.top());
        pq.pop();
    }

    return bins;
}

int main(int argc, char* argv[]) {
    int n, c, v;
    vector<int> w;
    vector<Bin> bins;
    const char* filename;
    
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <filename> <algorithm for initial solution>" << endl;
        return 1;
    }

    filename = argv[1];
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }

    file >> n >> c;
    for (int i = 0; i < n; ++i) {
        file >> v;
        w.push_back(v);
    }

    file.close();

    string algorithm = argv[2];
    if (algorithm == "ff") {
        // cout << "first-fit" << ": " << first_fit(w,c).size() << endl;
        bins = first_fit(w, c);
    } else if (algorithm == "bf") {
        // cout << "best-fit" << ": " << best_fit(w,c).size() << endl;
        bins = best_fit(w, c);
    } else if (algorithm == "wf") {
        // cout << "worst-fit" << ": " << worst_fit(w, c).size() << endl;
        bins = worst_fit(w, c);
    } else {
        cerr << "Algoritm for initial solution not found!";
        return 1;
    }
    cout << "Minimum bins: " << bins.size() << endl;
    for (auto b : bins) {
        cout << "[ ";
        for (auto item : b.items) {
            cout << item << " ";
        }
        cout << "]" << endl;
    }

    return 0;
}
