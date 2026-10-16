#ifndef AED_ALGHORITHMS_BACKTRACKING_H
#define AED_ALGHORITHMS_BACKTRACKING_H

#include <cstddef>
#include <utility>
#include <vector>

namespace aed {

// choices(state) returns valid options; undo must reverse choose.
template <typename State, typename IsComplete, typename Choices,
          typename Choose, typename Undo, typename Visit>
void backtrack(State& state, const IsComplete& is_complete,
               const Choices& choices, const Choose& choose,
               const Undo& undo, const Visit& visit) {
    if (is_complete(std::as_const(state))) {
        visit(std::as_const(state));
        return;
    }

    const auto options = choices(std::as_const(state));
    for (const auto& option : options) {
        choose(state, option);
        backtrack(state, is_complete, choices, choose, undo, visit);
        undo(state, option);
    }
}

namespace detail {

template <typename T>
void permutation_search(const std::vector<T>& values,
                        std::vector<T>& current,
                        std::vector<bool>& used,
                        std::vector<std::vector<T>>& result) {
    if (current.size() == values.size()) {
        result.push_back(current);
        return;
    }

    for (std::size_t i = 0; i < values.size(); ++i) {
        if (used[i]) {
            continue;
        }

        used[i] = true;
        current.push_back(values[i]);

        permutation_search(values, current, used, result);

        current.pop_back();
        used[i] = false;
    }
}

template <typename T>
void subset_search(const std::vector<T>& values, std::size_t index,
                   std::vector<T>& current,
                   std::vector<std::vector<T>>& result) {
    if (index == values.size()) {
        result.push_back(current);
        return;
    }

    current.push_back(values[index]);
    subset_search(values, index + 1, current, result);
    current.pop_back();

    subset_search(values, index + 1, current, result);
}

} // namespace detail

// O(n * n!) time and result storage. Equal input values may repeat results.
template <typename T>
std::vector<std::vector<T>> permutations(const std::vector<T>& values) {
    std::vector<std::vector<T>> result;
    std::vector<T> current;
    std::vector<bool> used(values.size(), false);
    current.reserve(values.size());

    detail::permutation_search(values, current, used, result);
    return result;
}

// O(n * 2^n) time and result storage. Equal input values may repeat results.
template <typename T>
std::vector<std::vector<T>> subsets(const std::vector<T>& values) {
    std::vector<std::vector<T>> result;
    std::vector<T> current;
    current.reserve(values.size());

    detail::subset_search(values, 0, current, result);
    return result;
}

} // namespace aed

#endif // AED_ALGHORITHMS_BACKTRACKING_H
