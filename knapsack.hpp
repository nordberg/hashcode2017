/**
* @Author: Andre Nyström & Axel Riese
* @Email:  andreny@kth.se axelri@kth.se
*/

#include <iterator>
#include <vector>

typedef unsigned int uint;

struct Item {
    uint value;
    uint weight;

    Item(uint value, uint weight) {
        this->value = value;
        this->weight = weight;
    }
};

/**
 * knapsack solves the bounded knapsack problem, i.e the maximum value of all
 * items that could be added to a set such that the sum of the weigths
 * does not exceed the set capacity. Assume that every item could be used
 * just once. It outputs the Items that are in the optimal set
 *
 * @param capacity  [The capacity of the knapsack]
 * @param begin           [Iterator pointing to the first Item in the sequence]
 * @param end             [Iterator pointing to the end of the sequence]
 * @param out             [Begin iterator to container where result is to be stored]
 * @return                [Returns true if a solution was added to out]
*/
template <typename InputIterator, typename OutputIterator>
bool knapsack(const double& capacity, InputIterator begin, InputIterator end, OutputIterator out) {
    uint max_weight = (uint) capacity;
    int n = std::distance(begin, end);

    if (n < 1) {
        return true;
    }

    // C x N+1 matrix,
    // build optimal solution for subproblem: maximize value
    // given weight w and used items (in order) <= i
    std::vector<std::vector<uint>> mat(max_weight+1, std::vector<uint>(n+1));

    // build matrix by either adding the current Items
    // or continue with the current set of Items, such
    // that the current accumulated value is maximized
    for (int w = 1; w < max_weight+1; ++w) {
        for (int i = 1; i < n+1; ++i) {
            uint not_added = mat[w][i-1];
            if ((begin+i-1)->weight > w) {
                mat[w][i] = not_added;
            } else {
                uint added = mat[w-(begin+i-1)->weight][i-1] + (begin+i-1)->value;
                mat[w][i] = std::max(added, not_added);
            }
        }
    }

    // follow from mat[C][n] to determine item set
    std::vector<int> res;
    int w = max_weight;
    for (int i = n; i > 0 && w > 0; --i) {
        if (mat[w][i] > mat[w][i-1]) {
            *out = i-1;
            out++;
            w -= (begin+i-1)->weight;
        }
    }

    return true;
}
