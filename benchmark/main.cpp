//
// Created by noname on 20.12.22.
//

#include <benchmark/benchmark.h>
#include <monogon/Array.h>
#include <monogon/Variable.h>
#include <monogon/initializer/Initializer.h>
#include <iostream>

auto dot(const Array<double>& A, const Array<double>& B)
{
    return A.dot(B);
}

auto sum(const Array<double>& A, const Array<double>& B)
{
    return A + B;
}

auto dot(const Variable<Array<double>>& A, const Variable<Array<double>>& B)
{
    return A.dot(B).back_propagation();
}

auto sum(const Variable<Array<double>>& A, const Variable<Array<double>>& B)
{
    return (A + B).back_propagation();
}

static void bm_mat_mul(benchmark::State& state)
{
    GlorotUniform glorotUniform;
    for(auto _ : state)
    {
        state.PauseTiming();
        Array A = glorotUniform(static_cast<size_t>(state.range(0)), static_cast<size_t>(state.range(0))).get_value();
        state.ResumeTiming();
        dot(A, A);
    }
    state.SetComplexityN(state.range(0));
}

static void bm_mat_sum(benchmark::State& state)
{
    GlorotUniform glorotUniform;
    for(auto _ : state)
    {
        state.PauseTiming();
        Array A = glorotUniform(static_cast<size_t>(state.range(0)), static_cast<size_t>(state.range(0))).get_value();
        state.ResumeTiming();
        sum(A, A);
    }
    state.SetComplexityN(state.range(0));
}

static void bm_var_mul(benchmark::State& state)
{
    GlorotUniform glorotUniform;
    for(auto _ : state)
    {
        state.PauseTiming();
        Variable A = glorotUniform(static_cast<size_t>(state.range(0)), static_cast<size_t>(state.range(0)));
        state.ResumeTiming();
        dot(A, A);
    }
    state.SetComplexityN(state.range(0));
}

static void bm_var_sum(benchmark::State& state)
{
    GlorotUniform glorotUniform;
    for(auto _ : state)
    {
        state.PauseTiming();
        Variable A = glorotUniform(static_cast<size_t>(state.range(0)), static_cast<size_t>(state.range(0)));
        state.ResumeTiming();
        sum(A, A);
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK(bm_mat_sum)->RangeMultiplier(2)->Range(1 << 1, 1 << 12)->Complexity();
BENCHMARK(bm_mat_mul)->RangeMultiplier(2)->Range(1 << 1, 1 << 12)->Complexity();
BENCHMARK(bm_var_sum)->RangeMultiplier(2)->Range(1 << 1, 1 << 12)->Complexity();
BENCHMARK(bm_var_mul)->RangeMultiplier(2)->Range(1 << 1, 1 << 12)->Complexity(benchmark::oNCubed);

BENCHMARK_MAIN();
