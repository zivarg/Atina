#include "inotify.h"

#include <unistd.h>

#include <stdio.h>

#include <sys/inotify.h>

#include "valueinitializer.h"

Inotify::Inotify() {}

Inotify::~Inotify() { deinit(); }

bool Inotify::init() {

  if (mFd == -1) {
    deinit();
  }

  ValueInitializer<int> fdInitializer{{false, -1},
      [](int &aValue) { closeFd(aValue); },
      [](int &aValue) { return initFd(aValue); }};

  if (!fdInitializer.isOk()) {
    return false;
  }

  auto fd{fdInitializer.value()};

  ValueInitializer<int> epollFdInitializer{{false, -1},
      [](int &aValue) { closeEpollFd(aValue); },
      [](int &aValue) { return initEpollFd(aValue); }};

  if (!epollFdInitializer.isOk()) {
    return false;
  }

  auto epollFd{epollFdInitializer.value()};

  ValueInitializer<epoll_event> epollEventInitializer{
      [fd, epollFd](epoll_event &aValue) { delEpollCtl(epollFd, fd); },
      [fd, epollFd](epoll_event &aValue) {
        aValue = collectEpollEvent(epollFd, fd);

        return addEpollCtl(epollFd, fd, aValue);
      }};

  if (!epollEventInitializer.isOk()) {
    return false;
  }

  mFd = fdInitializer.takeValue();

  mEpollFd = epollFdInitializer.takeValue();

  mEpollEvent = epollEventInitializer.takeValue();

  //
  //  mFd = inotify_init1(IN_NONBLOCK);
  //
  //  if (mFd == -1) {
  //    fprintf(stderr, "Inotify file descriptor cannot be initialized.");
  //    return false;
  //  }
  //
  //  mEpollFd = epoll_create1(0);
  //
  //  if (mEpollFd == -1) {
  //    fprintf(stderr, "Epoll file descriptor cannot be created.");
  //
  //    if (close(mFd) != 0) {
  //      fprintf(stderr, "Inotify file descriptor cannot be normally closed.");
  //    }
  //
  //    mFd = -1;
  //
  //    return false;
}

////  if (!initFd(&mFd)) {
////    return false;
////  }
//
//  if (!initEpollFd(&mEpollFd)) {
//    closeFd(&mFd);
//
//    return false;
//  }
//
//  return true;
//}
//
void Inotify::deinit() {
  if (mFd == -1) {
    return;
  }

  //  removeAllWatches();
  //
  //  closeFd(&mFd);
}

// bool Inotify::addWatch(const std::string &aPathName, std::uint32_t
// aEventMask) {
//  //  if (!validateFd()) {
//  //    return false;
//  //  }
//
//  int wd = inotify_add_watch(mFd, aPathName.c_str(), aEventMask);
//
//  if (wd == -1) {
//    fprintf(stderr, "Inotify watch cannot be added.");
//  }
//
//  mWds.insert({wd, aPathName});
//
//  return true;
//}
//
// void Inotify::removeWatch(const std::string &aPathName) {
//  //  if (!validateFd()) {
//  //    return;
//  //  }
//
//  int wd{pathNameToWd(aPathName)};
//
//  if (wd == -1) {
//    fprintf(stderr, "Inotify watch descriptor with this path name not
//    found.");
//  }
//
//  removeWatch(wd);
//}
//
// void Inotify::removeAllWatches() {
//  //  if (!validateFd()) {
//  //    return;
//  //  }
//
//  auto itWd{mWds.begin()};
//
//  while (itWd != mWds.end()) {
//    removeWatch(itWd->first, false);
//
//    ++itWd;
//  }
//
//  mWds.clear();
//}
//
bool Inotify::initFd(int &aFd) {
  if (aFd != -1) {
    closeFd(aFd);
  }

  aFd = inotify_init1(IN_NONBLOCK);

  if (aFd == -1) {
    fprintf(stderr, "Inotify file descriptor cannot be initialized.");
    return false;
  }

  return true;
}

bool Inotify::validateFd(const int &aFd, bool aIsDisplayError) {
  if (aFd == -1) {
    if (aIsDisplayError) {
      fprintf(stderr, "Inotify file descriptor is not initialized.");
    }
    return false;
  }

  return true;
}

void Inotify::closeFd(int &aFd) {
  if (aFd == -1) {
    return;
  }

  if (close(aFd) != 0) {
    fprintf(stderr, "Inotify file descriptor cannot be normally closed.");
  }

  aFd = -1;
}

bool Inotify::initEpollFd(int &aEpollFd) {
  if (aEpollFd != -1) {
    closeEpollFd(aEpollFd);
  }

  aEpollFd = epoll_create1(0);

  if (aEpollFd == -1) {
    fprintf(stderr, "Epoll file descriptor cannot be created.");
    return false;
  }

  return true;
}

bool Inotify::validateEpollFd(const int &aEpollFd, bool aIsDisplayError) {
  if (aEpollFd == -1) {
    if (aIsDisplayError) {
      fprintf(stderr, "Epoll file descriptor is not initialized.");
    }
    return false;
  }

  return true;
}

void Inotify::closeEpollFd(int &aEpollFd) {
  if (aEpollFd == -1) {
    return;
  }

  if (close(aEpollFd) != 0) {
    fprintf(stderr, "Epoll file descriptor cannot be normally closed.");
  }

  aEpollFd = -1;
}

bool Inotify::addEpollCtl(const int &aEpollFd, const int &aFd,
                          epoll_event &epollEvent) {
  if (!validateEpollFd(aEpollFd)) {
    return false;
  }

  if (epoll_ctl(aEpollFd, EPOLL_CTL_ADD, aFd, &epollEvent) == -1) {
    fprintf(stderr, "File descriptor cannot be added to epoll.");

    return false;
  }

  return true;
}

void Inotify::delEpollCtl(const int &aEpollFd, const int &aFd) {
  if (aEpollFd == -1) {
    return;
  }

  if (epoll_ctl(aEpollFd, EPOLL_CTL_DEL, aFd, nullptr) == -1) {
    fprintf(stderr, "File descriptor cannot be normally deleted from epoll.");
  }
}

epoll_event Inotify::collectEpollEvent(const int &aEpollFd, const int &aFd) {
  epoll_event epollEvent;

  epollEvent.events = EPOLLIN | EPOLLET;

  epollEvent.data.fd = aFd;

  return epollEvent;
}
//
// void Inotify::removeWatch(int wd, bool isRemoveFromCollection) {
//  //  if (!validateFd()) {
//  //    return;
//  //  }
//
//  if (wd == -1) {
//    fprintf(stderr, "Inotify watch descriptor not valid.");
//    return;
//  }
//
//  if (inotify_rm_watch(mFd, wd) != 0) {
//    fprintf(stderr, "Inotify watch cannot be normally removed.");
//  }
//
//  if (isRemoveFromCollection) {
//    mWds.erase(wd);
//  }
//}
//
// std::string Inotify::wdToPathName(int wd) {
//  const auto itWd(mWds.find(wd));
//
//  if (itWd == mWds.end()) {
//    return {};
//  }
//
//  return itWd->second;
//}
//
// int Inotify::pathNameToWd(const std::string &pathName) {
//  auto itWd(mWds.begin());
//
//  while (itWd != mWds.end()) {
//    if (pathName.compare(itWd->second) == 0) {
//      return itWd->first;
//    }
//
//    ++itWd;
//  }
//
//  return -1;
//}
