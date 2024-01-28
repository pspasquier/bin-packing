#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <numeric>

using namespace std;

const int MAX_ITERATIONS_WITHOUT_IMPROVEMENT = 3;
const int MAX_ITERATIONS = 500;

struct Bin
{
    int currWeight;
    vector<int> items;

    bool operator<(const Bin &other) const
    {
        return currWeight > other.currWeight;
    }
};

vector<Bin> first_fit(vector<int> &w, int c)
{
    vector<Bin> bins;
    for (int i = 0; i < w.size(); i++)
    {
        int item = w[i];
        bool fit = false;
        for (int j = 0; j < bins.size(); j++)
        {
            int currWeight = bins[j].currWeight;
            if (currWeight + item <= c)
            {
                bins[j].currWeight += item;
                bins[j].items.push_back(i);
                fit = true;
                break;
            }
        }
        if (!fit)
        {
            bins.push_back({item, {i}});
        }
    }
    return bins;
}

vector<Bin> best_fit(vector<int> &w, int c)
{
    vector<Bin> bins;
    for (int i = 0; i < w.size(); i++)
    {
        int item = w[i];
        bool fit = false;
        for (int j = 0; j < bins.size(); j++)
        {
            int currWeight = bins[j].currWeight;
            if (currWeight + item <= c)
            {
                bins[j].currWeight += item;
                bins[j].items.push_back(i);
                fit = true;
                break;
            }
        }
        if (!fit)
        {
            bins.push_back({item, {i}});
        }
        sort(bins.begin(), bins.end());
    }
    return bins;
}

vector<Bin> worst_fit(vector<int> &w, int c)
{
    vector<Bin> bins;
    priority_queue<Bin> pq;

    for (int i = 0; i < w.size(); i++)
    {
        int item = w[i];
        if (pq.size() == 0 || pq.top().currWeight + item > c)
        {
            pq.push({item, {i}});
        }
        else
        {
            Bin b = pq.top();
            pq.pop();
            b.currWeight += item;
            b.items.push_back(i);
            pq.push(b);
        }
    }

    while (!pq.empty())
    {
        bins.push_back(pq.top());
        pq.pop();
    }

    return bins;
}

vector<vector<Bin>> generate_neighborhood(vector<Bin> &current_solution, const vector<int> &w, int bin_capacity)
{
    vector<vector<Bin>> neighborhood;

    // Generate default neighborhood
    for (int i = 0; i < current_solution.size(); i++)
    {
        for (int j = i + 1; j < current_solution.size(); j++)
        {
            for (int item_i = 0; item_i < current_solution[i].items.size(); item_i++)
            {
                for (int item_j = 0; item_j < current_solution[j].items.size(); item_j++)
                {
                    // Swap items between bins
                    vector<Bin> new_solution = current_solution;
                    swap(new_solution[i].items[item_i], new_solution[j].items[item_j]);

                    // Recalculate the weights of the bins i and j
                    new_solution[i].currWeight = 0;
                    for (auto item : new_solution[i].items)
                    {
                        new_solution[i].currWeight += w[item];
                    }
                    new_solution[j].currWeight = 0;
                    for (auto item : new_solution[j].items)
                    {
                        new_solution[j].currWeight += w[item];
                    }

                    // Check if the new solution is valid
                    if (new_solution[i].currWeight <= bin_capacity && new_solution[j].currWeight <= bin_capacity)
                    {
                        neighborhood.push_back(new_solution);
                    }
                }
            }
        }
    }

    // Trying to decrease the number of bins
    for (int i = 0; i < current_solution.size(); i++)
    {
        if (current_solution[i].items.size() == 1)
        { // if bin has only one item
            for (int j = 0; j < current_solution.size(); j++)
            {
                if (i != j && current_solution[j].currWeight + w[current_solution[i].items[0]] <= bin_capacity)
                { // if item fits in bin j
                    vector<Bin> new_solution = current_solution;
                    new_solution[j].items.push_back(new_solution[i].items[0]); // move item to bin j
                    new_solution[j].currWeight += w[new_solution[i].items[0]]; // update weight of bin j
                    new_solution.erase(new_solution.begin() + i);              // remove bin i
                    neighborhood.push_back(new_solution);
                }
            }
        }
    }

    return neighborhood;
}

void print_neighborhood(const vector<vector<Bin>> &neighborhood)
{
    int solution_number = 1;
    for (const auto &solution : neighborhood)
    {
        cout << "Solution " << solution_number << ":\n";
        cout << "Total bins: " << solution.size() << "\n";
        for (const auto &bin : solution)
        {
            cout << "[ ";
            for (const auto &item : bin.items)
            {
                cout << item << " ";
            }
            cout << "]\n";
        }
        cout << "\n";
        solution_number++;
    }
}

vector<Bin> choose_best_solution(const vector<vector<Bin>> &neighborhood)
{
    vector<Bin> best_solution = neighborhood[0];
    for (const auto &solution : neighborhood)
    {
        if (solution.size() < best_solution.size())
        {
            best_solution = solution;
        }
    }
    return best_solution;
}

vector<Bin> perturb_solution(vector<Bin> &current_solution, const vector<int> &w, int bin_capacity)
{
    vector<Bin> perturbed_solution = current_solution;
    if (perturbed_solution.size() > 1)
    {
        // Select two different bins randomly
        int i = rand() % perturbed_solution.size();
        int j;
        do
        {
            j = rand() % perturbed_solution.size();
        } while (j == i);

        // Select an item from bin i randomly
        int item_i = rand() % perturbed_solution[i].items.size();

        // If the item fits in bin j, move it
        if (perturbed_solution[j].currWeight + w[perturbed_solution[i].items[item_i]] <= bin_capacity)
        {
            perturbed_solution[j].items.push_back(perturbed_solution[i].items[item_i]);
            perturbed_solution[j].currWeight += w[perturbed_solution[i].items[item_i]];
            perturbed_solution[i].items.erase(perturbed_solution[i].items.begin() + item_i);
            perturbed_solution[i].currWeight -= w[perturbed_solution[i].items[item_i]];
        }

        // If bin i is empty we can remove it
        if (perturbed_solution[i].items.empty())
        {
            perturbed_solution.erase(perturbed_solution.begin() + i);
        }
    }
    return perturbed_solution;
}

int main(int argc, char *argv[])
{
    int n, c, v;
    vector<int> w;
    vector<Bin> bins;
    const char *filename;

    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << " <filename> <algorithm for initial solution>" << endl;
        return 1;
    }

    filename = argv[1];
    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }

    file >> n >> c;
    for (int i = 0; i < n; ++i)
    {
        file >> v;
        w.push_back(v);
    }

    file.close();

    string algorithm = argv[2];
    if (algorithm == "ff")
    {
        // cout << "first-fit" << ": " << first_fit(w,c).size() << endl;
        bins = first_fit(w, c);
    }
    else if (algorithm == "bf")
    {
        // cout << "best-fit" << ": " << best_fit(w,c).size() << endl;
        bins = best_fit(w, c);
    }
    else if (algorithm == "wf")
    {
        // cout << "worst-fit" << ": " << worst_fit(w, c).size() << endl;
        bins = worst_fit(w, c);
    }
    else
    {
        cerr << "Algoritm for initial solution not found!";
        return 1;
    }

    cout << "[Initial Solution] Minimum bins: " << bins.size() << endl;
    for (auto b : bins)
    {
        cout << "[ ";
        for (auto item : b.items)
        {
            cout << item << " ";
        }
        cout << "]" << endl;
    }

    // Iterated Local Search (ILS)
    int bin_capacity = c;
    vector<Bin> current_solution = bins;
    vector<Bin> best_solution = current_solution;
    int iterations_without_improvement = 0;
    int iterations = 0;

    while (true)
    {
        vector<vector<Bin>> neighborhood = generate_neighborhood(current_solution, w, bin_capacity);
        vector<Bin> local_best_solution = choose_best_solution(neighborhood);
        if (local_best_solution.size() < best_solution.size())
        {
            best_solution = local_best_solution;
            cout << "Hurray New Solution!" << endl;
            iterations_without_improvement = 0;
        }
        else
        {
            iterations_without_improvement++;
            iterations++;
            cout << "ITERATION: " << iterations << endl;
        }

        // If no improvement after several iterations, perturb the solution
        if (iterations_without_improvement >= MAX_ITERATIONS_WITHOUT_IMPROVEMENT)
        {
            current_solution = perturb_solution(best_solution, w, bin_capacity);
            iterations_without_improvement = 0;
            // print perturbed solution
            cout << endl
                 << "[ILS] Perturbed Solution: " << current_solution.size() << endl;
            for (auto b : current_solution)
            {
                cout << "[ ";
                for (auto item : b.items)
                {
                    cout << item << " ";
                }
                cout << "]" << endl;
            }
        }
        else
        {
            current_solution = local_best_solution;
            // print current solution
            cout << endl
                 << "[LS] Current Solution: " << current_solution.size() << endl;
            for (auto b : current_solution)
            {
                cout << "[ ";
                for (auto item : b.items)
                {
                    cout << item << " ";
                }
                cout << "]" << endl;
            }
        }

        // Stopping condition
        if (iterations >= MAX_ITERATIONS)
        {
            break;
        }
    }

    // print best solution
    cout << endl
         << "[ILS] Current Solution: " << current_solution.size() << endl;
    for (auto b : current_solution)
    {
        cout << "[ ";
        for (auto item : b.items)
        {
            cout << item << " ";
        }
        cout << "]" << endl;
    }

    return 0;
}
