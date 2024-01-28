#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <cmath>

using namespace std;

struct Bin {
    int currWeight;
    vector<int> items;

    bool operator<(const Bin& other) const {
        return currWeight > other.currWeight;
    }

    bool operator==(const Bin& other) const {
        return currWeight == other.currWeight;
    }
};

void printSolution(vector<Bin> solution, bool start, bool end, bool bins) {
    if (start) cout << "Solução Inicial: " << solution.size() << endl;
    else if (end) cout << "Solução final: " << solution.size() << endl;
    else cout << "Minimum bins: " << solution.size() << endl;
    if (bins) {
        for (auto b : solution) {
            cout << "[ ";
            for (auto item : b.items) {
                cout << item << " ";
            }
            cout << "] " << b.currWeight << endl;
        }
        cout << "------------------" << endl;
    }
}

void best_fit(vector<Bin>& bins, int item, int c) {
    bool fit = false;
    sort(bins.begin(), bins.end());
    for (int i = 0; i < bins.size(); i++) {
        int currWeight = bins[i].currWeight;
        if (currWeight + item <= c) {
            bins[i].currWeight += item;
            bins[i].items.push_back(item);
            fit = true;
            break;
        }
    }
    if (!fit)
        bins.push_back({item, {item}});
}

vector<Bin> initial_solution(vector<int>& w , int c) {
    vector<Bin> bins;
    for (int item : w)
        best_fit(bins, item, c);
    return bins;
}

bool is_feasible(vector<Bin>& solution, int c) {
    for (auto b : solution)
        if (b.currWeight > c)
            return false;
    return true;
}

pair<Bin, Bin> largestDifferencingMethod(vector<int>& items) {
    Bin set1 = {0, {}}, set2 = {0, {}};
    sort(items.begin(), items.end(), greater<int>());
    for (int item : items) {
        if (set1.currWeight <= set2.currWeight) {
            set1.currWeight += item;
            set1.items.push_back(item);
        } else {
            set2.currWeight += item;
            set2.items.push_back(item);
        }
    }
    return make_pair(set1, set2);
}

void local_search(vector<Bin>& solution, int c) {
    int i = 0;
    while (!is_feasible(solution, c)) {
        sort(solution.begin(), solution.end());
        vector<int> items;
        for (auto item : solution[0].items) items.push_back(item);
        for (auto item : solution[1].items) items.push_back(item);
        pair<Bin, Bin> validPair = largestDifferencingMethod(items);
        if ((solution[0] == validPair.first && solution[1] == validPair.second) ||
            (solution[1] == validPair.first && solution[0] == validPair.second) ||
            abs(solution[0].currWeight - solution[1].currWeight) < abs(validPair.first.currWeight - validPair.second.currWeight)) {
            break;
            // sort(solution[0].items.begin(), solution[0].items.end(), greater<int>());
            // int item = solution[0].items.back();
            // solution[0].items.pop_back();
            // solution[0].currWeight -= item;
            // best_fit(solution, item, c);
        }
        else {
            solution[0] = validPair.first;
            solution[1] = validPair.second;
        }
    }
    while (!is_feasible(solution, c)) {
        sort(solution.begin(), solution.end());
        sort(solution[0].items.begin(), solution[0].items.end(), greater<int>());
        int item = solution[0].items.back();
        solution[0].items.pop_back();
        solution[0].currWeight -= item;
        best_fit(solution, item, c);
    }
}

vector<Bin> perturbation(vector<Bin>& solution) {
    vector<Bin> newSolution = solution;
    int indexRemove = rand() % newSolution.size();
    Bin b = newSolution[indexRemove];
    newSolution.erase(newSolution.begin() + indexRemove);
    for (int item : b.items) {
        int indexInsert = rand() % newSolution.size();
        newSolution[indexInsert].currWeight += item;
        newSolution[indexInsert].items.push_back(item);
    }
    return newSolution;
}

void swap(vector<Bin>& solution) {
    int bin1 = 0, bin2 = 0, index1, index2, item1, item2;
    while (bin1 != bin2) {
        bin1 = rand() % solution.size();
        bin2 = rand() % solution.size();
    }
    index1 = rand() % solution[bin1].items.size();
    index2 = rand() % solution[bin2].items.size();
    item1 = solution[bin1].items[index1];
    item2 = solution[bin2].items[index2];
    solution[bin1].currWeight += item2 - item1;
    solution[bin1].items[index1] = item2;
    solution[bin2].currWeight += item1 - item2;
    solution[bin2].items[index2] = item1;
}


void ils(vector<int>& w, int c) {
    vector<Bin> solution = initial_solution(w, c);
    vector<Bin> bestSolution = solution;
    int iterWithoutImpro = 0;
    printSolution(bestSolution, true, false, false);
    for (int i = 0; i < 10000; i++) {
        vector<Bin> currentSolution;
        if (iterWithoutImpro == 20) {
            iterWithoutImpro = 0;
            currentSolution = solution;
            swap(currentSolution);
        } else {
            currentSolution = perturbation(solution);
        }
        local_search(currentSolution, c);
        int delta = currentSolution.size() - solution.size();
        // printSolution(currentSolution, false, false, false);
        if (delta < 0 || 0.6 > ((double) rand() / RAND_MAX)) {
            solution = currentSolution;
        }
        if (solution.size() < bestSolution.size()) {
            iterWithoutImpro = 0;
            bestSolution = solution;
        } else {
            iterWithoutImpro++;
        }
    }
    printSolution(bestSolution, false, true, false);
}

int main(int argc, char* argv[]) {
    int n, c, v;
    vector<int> w;
    vector<Bin> bins;
    const char* filename;

    srand(time(NULL));
    
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
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

    ils(w, c);

    return 0;
}
