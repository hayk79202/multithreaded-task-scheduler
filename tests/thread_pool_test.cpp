#include "thread_pool.hpp"
#include <stdexcept>
#include <vector>

void require(bool condition) {
    if (!condition) throw std::runtime_error("thread pool test failed");
}

int main() {
    ThreadPool pool(3);
    require(pool.submit([](int x) { return x * 2; }, 21).get() == 42);
    std::vector<std::future<int>> results;
    for (int i = 0; i < 100; ++i) {
        results.push_back(pool.submit([i] { return i * i; }));
    }
    int sum = 0;
    for (auto& result : results) sum += result.get();
    require(sum == 328350);
    auto failure = pool.submit([]() -> int { throw std::runtime_error("expected"); });
    try {
        failure.get();
        return 1;
    } catch (const std::runtime_error&) {
    }
    auto queued = pool.submit([] { return 7; });
    pool.shutdown();
    require(queued.get() == 7);
    try {
        pool.submit([] {});
        return 1;
    } catch (const std::runtime_error&) {
    }
    ThreadPool fallback(0);
    require(fallback.submit([] { return 9; }).get() == 9);
}
