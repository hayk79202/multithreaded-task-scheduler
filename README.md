# Multithreaded Task Scheduler

![Build and test](https://github.com/hayk79202/multithreaded-task-scheduler/actions/workflows/ci.yml/badge.svg)

A compact C++17 thread pool that runs submitted jobs on a fixed set of worker threads. It is designed as a readable systems programming project: the implementation is intentionally small enough to audit.

## What it does

- Accepts callable tasks and returns `std::future` results.
- Queues work safely with a mutex and condition variable.
- Drains queued tasks during graceful shutdown.
- Can wait for all queued and running tasks with `wait_for_idle()`.
- Rejects new submissions after shutdown begins.

## Quick start

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/scheduler_demo
```

Requires a C++17 compiler, CMake 3.16+, and a platform with threads. On Windows, run the generated executable from the build configuration directory (for example `build/Debug/scheduler_demo.exe`).

The demo prints the squares of 1 through 8: `1 4 9 16 25 36 49 64`.

## Design

```text
submit(fn) -> packaged task -> synchronized queue -> worker thread -> future
                                   |
                              shutdown drains queue
```

`ThreadPool` owns its workers. Destruction calls `shutdown()`, joins every worker, and waits for already queued tasks to finish. A task exception is stored in its future. Call `wait_for_idle()` from outside the pool when you need to wait without retaining every future. The test executable covers many submitted jobs, idle waiting, exception propagation, queue draining, rejection after shutdown, and the zero-worker fallback. GitHub Actions builds and tests on Linux and Windows.

## Scope

This is a small working prototype, not a production scheduler. It has no priorities, cancellation, dynamic worker sizing, or published benchmark results. A future benchmark can compare throughput and queue latency across worker counts and task sizes; those measurements should be run on a stated machine before any performance claims are made.

## Repository map

- `include/thread_pool.hpp` — pool implementation
- `src/main.cpp` — example program
- `tests/thread_pool_test.cpp` — basic behavior checks
- `.github/workflows/ci.yml` — Linux and Windows build checks

## License

MIT
