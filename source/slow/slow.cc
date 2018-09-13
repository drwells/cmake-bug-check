#include <chrono>
#include <iostream>
#include <thread>

#include <checkbug/slow/slow.h>

void quick(const std::size_t a)
{
  std::this_thread::sleep_for(std::chrono::seconds{1});
  for (std::size_t i = 0; i < a; ++i)
    std::cout << "slow";
  if (0 < a)
    std::cout << '\n';
}
