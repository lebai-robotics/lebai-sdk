#include <iostream>

#include <lebai/discovery.hh>

int main() {
  lebai::zeroconf::Discovery discovery;
  const auto controllers = discovery.resolve();

  std::cout << "Found " << controllers.size() << " controller(s)" << std::endl;
  for (const auto &controller : controllers) {
    std::cout << controller.ip_address << " " << controller.model << " "
              << controller.hostname << std::endl;
  }
  return 0;
}
