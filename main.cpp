#include <iostream>

#include "src/inotify.h"

int main() {
  Inotify inotify;

  inotify.init();

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
