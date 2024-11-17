#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <cmath>
#include <random>

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
    for (int i = bins.size() - 1; i < bins.size(); i++) {
        int currWeight = bins[i].currWeight;
        if (currWeight + item <= c) {
            bins[i].currWeight += item;
            bins[i].items.push_back(item);
            fit = true;
            break;
        }
    }
    if (!fit) bins.push_back({item, {item}});
}

vector<Bin> initial_solution(vector<int>& w, int c) {
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

vector<int> generateNeighboors(int n, int max) {
    vector<int> randomNumbers;

    while (randomNumbers.size() < n) {
        int randomNumber = rand() % max;
        if (find(randomNumbers.begin(), randomNumbers.end(), randomNumber) == randomNumbers.end()) {
            randomNumbers.push_back(randomNumber);
        }
    }

    return randomNumbers;
}

vector<Bin> eliminate_bin(vector<Bin> newSolution, int i) {
    Bin b = newSolution[i];
    newSolution.erase(newSolution.begin() + i);
    for (int item : b.items) {
        int indexInsert = rand() % newSolution.size();
        newSolution[indexInsert].currWeight += item;
        newSolution[indexInsert].items.push_back(item);
    }
    return newSolution;
}

void local_search(vector<Bin>& solution, int c) {
    bool improvement = true;
    while (!is_feasible(solution, c) && improvement) {
        improvement = false;
        for (int i = solution.size() - 1; i > 0; i--) {
            for (int j = 1; j < i; j++) {
                if (solution[i].currWeight > c || solution[j].currWeight > c) {
                    vector<int> items;
                    for (auto item : solution[i].items) items.push_back(item);
                    for (auto item : solution[j].items) items.push_back(item);
                    pair<Bin, Bin> pair = largestDifferencingMethod(items);
                    bool validPair = 
                        (solution[i] == pair.first && solution[j] == pair.second) ||
                        (solution[j] == pair.first && solution[i] == pair.second);
                    if (!validPair) {
                        solution[i] = pair.first;
                        solution[j] = pair.second;
                        improvement = true;
                    }
                }
            }
        }
    }
}

vector<Bin> solution_improvement(vector<Bin>& solution, int c) {
    vector<Bin> s = solution;
    bool improvement = true;
    while (improvement) {
        improvement = false;
        sort(s.begin(), s.end());
        int i = s.size()-1;
        vector<Bin> newSolution = eliminate_bin(s, i);
        if (is_feasible(newSolution, c)) {
            cout << "improved after eliminate_bin:" << endl;
            printSolution(s, false, false, true);
            printSolution(newSolution, false, false, true);
            improvement = true;
            s = newSolution;
            //break;
        } else {
            local_search(newSolution, c);
            if(is_feasible(newSolution, c)) {
                cout << "improved by eliminate+local_search:" << endl;
                printSolution(s, false, false, true);
                printSolution(newSolution, false, false, true);
                s = newSolution;
                improvement = true;
                //break;
            }
        }
    }
    return s;
}


vector<Bin> pertubation(vector<Bin>& solution, int c) {
    vector<Bin> newSolution = solution;
    int bin1 = 0, bin2 = 0, index1, index2, item1, item2, n = newSolution.size();
    while (bin1 == bin2) {
        bin1 = rand() % n + 1;
        bin2 = rand() % n + 1;
    }
    if (bin1 == n) {
        index2 = rand() % newSolution[bin2].items.size();
        item2 = newSolution[bin2].items[index2];
        newSolution.push_back({item2, {item2}});
        newSolution[bin2].currWeight -= item2;
        newSolution[bin2].items.erase(newSolution[bin2].items.begin() + index2);
    } else if (bin2 == n) {
            index1 = rand() % newSolution[bin1].items.size();
        item1 = newSolution[bin1].items[index1];
        newSolution.push_back({item1, {item1}});
        newSolution[bin1].currWeight -= item1;
        newSolution[bin1].items.erase(newSolution[bin1].items.begin() + index1);
    } else {
        index1 = rand() % newSolution[bin1].items.size();
        index2 = rand() % newSolution[bin2].items.size();
        item1 = newSolution[bin1].items[index1];
        item2 = newSolution[bin2].items[index2];
        newSolution[bin1].currWeight += item2 - item1;
        newSolution[bin1].items[index1] = item2;
        newSolution[bin2].currWeight += item1 - item2;
        newSolution[bin2].items[index2] = item1;
    }

    newSolution.erase(
        remove_if(newSolution.begin(), newSolution.end(), [](Bin b) { 
            return b.currWeight == 0;
        }), 
        newSolution.end()
    );
    return newSolution;
}

void ils(vector<int>& w, int c, int iter) {
    int i = 0;
    vector<Bin> solution = initial_solution(w, c);
    vector<Bin> bestGlobal = solution;
    printSolution(bestGlobal, false, true, false);
    while(i < iter && bestGlobal.size() > 1) {
        vector<Bin> bestLocal = solution_improvement(solution, c);
        if (is_feasible(bestLocal, c) && bestLocal.size() < bestGlobal.size())
            bestGlobal = bestLocal;
        solution = pertubation(bestLocal, c);
        i++;
    }
    printSolution(bestGlobal, false, true, false);
}

int main(int argc, char* argv[]) {
    int n, c, v, iter, seed;
    vector<int> w;
    vector<Bin> bins;
    const char* filename;
    
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <filename> <iter> <seed>" << endl;
        return 1;
    }

    filename = argv[1];
    iter = atoi(argv[2]);
    seed = atoi(argv[3]);

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

    srand(seed);

    ils(w, c, iter);

    return 0;
}
