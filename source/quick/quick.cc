#include <iostream>

#include <checkbug/quick/quick.h>

void quick(const std::size_t a)
{
  for (std::size_t i = 0; i < a; ++i)
    std::cout << "quick";
  if (0 < a)
    std::cout << '\n';
}
