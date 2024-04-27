#include <iostream>
#include <chrono>
#include <string>
#include <format>

constexpr bool is_prime(uint64_t n) {
  if (n % 2 == 0) {  // If even ...
    return n == 2;
  }
  for (int i = 3; i <= n/i; i += 2) { // Go up by 2
    if (n%i == 0) {
      return false;
    }
  }
  return n > 1;
}

constexpr uint64_t nearest_prime(uint64_t n) {
  
  for (; n > 0; --n) {
    if (is_prime(n)) {
      return n;
    }
  }
  return 0;
}
constexpr uint64_t floorlog2(uint64_t x)
{
  return x == 1 ? 0 : 1+floorlog2(x >> 1);
}
constexpr uint64_t ceillog2(uint64_t x)
{
  return x == 1 ? 0 : floorlog2(x - 1) + 1;
}

int main(int argc, char** argv) {
  constexpr std::size_t block_size = 64 * 1024;
  constexpr std::size_t entry_size = 24;
  constexpr std::size_t extra_data = 8;
  constexpr std::size_t max_probe_count = floorlog2((block_size-extra_data)/entry_size);
  constexpr std::size_t number_of_entries = nearest_prime((block_size-extra_data)/entry_size-max_probe_count);
  constexpr std::size_t max_num_of_entries = (block_size-extra_data)/entry_size-max_probe_count;
  constexpr std::size_t total_size = number_of_entries * entry_size
                       + max_probe_count * entry_size + extra_data;
  std::cout << "nearest prime: " << number_of_entries << std::endl;
  std::cout << "max slot count: " << max_num_of_entries << std::endl;
  std::cout << "total size: " << total_size << std::endl;
  std::cout << "wasted space: " << block_size-total_size << std::endl;
  std::cout << "max probe count: " << max_probe_count << std::endl;
  
  auto t0 = std::chrono::high_resolution_clock::now();
  constexpr auto m_prime = number_of_entries;
  uint64_t a = 0;
  for (auto i = 0; i < 100000000; ++i) {
    a = i % m_prime;
  }
  auto t1 = std::chrono::high_resolution_clock::now();
  constexpr auto m_mask = 64 * 1024 - 1;
  for (auto i = 0; i < 100000000; ++i) {
    a = i & m_mask;
  }
  auto t2 = std::chrono::high_resolution_clock::now();
  auto m_input = argc > 1 ? std::stoul(argv[1]) : m_prime;
  for (auto i = 0; i < 100000000; ++i) {
    a = i & m_input;
  }
  auto t3 = std::chrono::high_resolution_clock::now();
  auto d1 = t1 - t0;
  auto d2 = t2 - t1;
  auto d3 = t3 - t2;
  std::cout << std::format("Prime: {}", std::chrono::duration_cast<std::chrono::microseconds>(d1).count()) << std::endl;
  std::cout << std::format("Mask: {}", std::chrono::duration_cast<std::chrono::microseconds>(d2).count()) << std::endl;
  std::cout << std::format("User: {}", std::chrono::duration_cast<std::chrono::microseconds>(d3).count()) << std::endl;
  return 0;
}
