#include <chrono>
#include <iostream>
#include <thread>

int main()
{
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::cout << "hello, world\n";
}
