#include <iostream>

#include "src/atinainotify.h"

int main() {
  Atina::Inotify inotify;

  inotify.open();

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
