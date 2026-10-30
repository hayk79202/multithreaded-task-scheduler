#include "thread_pool.hpp"
#include <cassert>
#include <stdexcept>

int main() {
    ThreadPool pool(2);
    auto value = pool.submit([](int x) { return x * 2; }, 21);
    assert(value.get() == 42);
    auto failure = pool.submit([]() -> int { throw std::runtime_error("expected"); });
    try {
        failure.get();
        return 1;
    } catch (const std::runtime_error&) {
    }
    auto queued = pool.submit([] { return 7; });
    pool.shutdown();
    assert(queued.get() == 7);
    try {
        pool.submit([] {});
        return 1;
    } catch (const std::runtime_error&) {
    }
}
