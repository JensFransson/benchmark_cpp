#include <iostream>

#include <algorithm>
#include <array>
#include <cstring>
#include <functional>
#include <random>
#include <string>

#include <chrono>

template <typename T = std::mt19937>
auto random_generator() -> T {
    auto constexpr seed_bytes = sizeof(typename T::result_type) * T::state_size;
    auto constexpr seed_len = seed_bytes / sizeof(std::seed_seq::result_type);
    auto seed = std::array<std::seed_seq::result_type, seed_len>();
    auto dev = std::random_device();
    std::generate_n(begin(seed), seed_len, std::ref(dev));
    auto seed_seq = std::seed_seq(begin(seed), end(seed));
    return T{seed_seq};
}

auto generate_random_alphanumeric_string(std::size_t len) -> std::string {
    static constexpr auto chars =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
    thread_local auto rng = random_generator<>();
    auto dist = std::uniform_int_distribution{{}, std::strlen(chars) - 1};
    auto result = std::string(len, '\0');
    std::generate_n(begin(result), len, [&]() { return chars[dist(rng)]; });
    return result;
}


int main() {
    int size = 10000000;
    int len = 36;

    std::cout << "Generating a list of " << size << " random strings of length " << len << " each ..." << std::endl;

    auto start_generate = std::chrono::high_resolution_clock::now();

    std::vector<std::string> randomList;
    for (int i = 0; i < size; i++) {
        const std::string &random = generate_random_alphanumeric_string(len);
        randomList.push_back(random);
    }

    auto finish_generate = std::chrono::high_resolution_clock::now();

    auto microseconds_generate = std::chrono::duration_cast<std::chrono::microseconds>(finish_generate - start_generate);
    std::cout << "Generation time: " << microseconds_generate.count() << " microseconds\n";
    std::cout << "First list element: " << randomList.at(0) << "\n\n";

    std::cout << "Sorting the list of " << size << " random strings ..." << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    std::sort(randomList.begin(), randomList.end());

    auto finish = std::chrono::high_resolution_clock::now();

    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
    std::cout << "Sort time: " << microseconds.count() << " microseconds\n";

    return 0;
}


