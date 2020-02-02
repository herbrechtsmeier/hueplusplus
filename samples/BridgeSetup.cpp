#include <iostream>
#include <algorithm>

#include <Hue.h>

#ifdef _MSC_VER
#include <WinHttpHandler.h>

using SystemHttpHandler = WinHttpHandler;

#else
#include <LinHttpHandler.h>

using SystemHttpHandler = LinHttpHandler;

#endif

// Configure existing connections here, or leave empty for new connection
const std::string macAddress = "";
const std::string username = "";

Hue connectToBridge() {

  HueFinder finder(std::make_shared<SystemHttpHandler>());

  std::vector<HueFinder::HueIdentification> bridges = finder.FindBridges();

  for (const auto& bridge : bridges) {
    std::cout << "Bridge: " << bridge.mac << " at " << bridge.ip << '\n';
  }
  if (bridges.empty()) {
    std::cout << "Found no bridges\n";
    throw std::runtime_error("no bridges found");
  }

  if (macAddress.empty()) {
    std::cout << "No bridge given, connecting to first one.\n";
    return finder.GetBridge(bridges.front());
  }
  if (!username.empty()) {
    finder.AddUsername(macAddress, username);
  }
  auto it = std::find_if(bridges.begin(), bridges.end(),
    [&](const auto& identification) {return identification.mac == macAddress; });
  if (it == bridges.end()) {
    std::cout << "Given bridge not found\n";
    throw std::runtime_error("bridge not found");
  }
  return finder.GetBridge(*it);
}


int main(int argc, char **argv) {

  try {
    Hue hue = connectToBridge();

    std::cout << "Connected to bridge. IP: " << hue.getBridgeIP() << ", username: " << hue.getUsername() << '\n';

  }
  catch (...) {
  }

  std::cout << "Press enter to exit\n";
  std::cin.get();

  return 0;
}