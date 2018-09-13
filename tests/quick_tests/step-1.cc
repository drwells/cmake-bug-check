#include <chrono>
#include <iostream>
#include <thread>

#include <checkbug/quick/quick.h>

int main()
{
  std::this_thread::sleep_for(std::chrono::seconds(2));
  quick(3);
}
