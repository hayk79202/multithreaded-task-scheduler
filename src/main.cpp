#include "thread_pool.hpp"
#include <iostream>
#include <vector>

int main() {
    ThreadPool pool(4);
    std::vector<std::future<int>> results;
    for (int i = 1; i <= 8; ++i) {
        results.push_back(pool.submit([i] { return i * i; }));
    }
    for (auto& result : results) std::cout << result.get() << ' ';
    std::cout << '\n';
}
