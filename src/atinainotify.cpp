#include "atinainotify.h"

#include <fcntl.h>

#include <unistd.h>

#include <stdio.h>

#include <sys/inotify.h>

#include "atinavalueinitializer.h"

namespace Atina {

Inotify::Inotify() {}

Inotify::~Inotify() { close(); }

bool Inotify::open() {

  if (mFd == -1) {
    close();
  }

  ValueInitializer<Fd> fdInitializer{{false, mFd.name()},
                                     [](Fd &aValue) { Fd::close(aValue); },
                                     [](Fd &aValue) { return initFd(aValue); }};

  if (!fdInitializer.isOk()) {
    return false;
  }

  auto fd{fdInitializer.value()};

  ValueInitializer<Fd> epollFdInitializer{
      {false, mEpollFd.name()},
      [](Fd &aValue) { Fd::close(aValue); },
      [](Fd &aValue) { return initEpollFd(aValue); }};

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
void Inotify::close() {
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
bool Inotify::initFd(Fd &aFd) {
  if (aFd != -1) {
    Fd::close(aFd);
  }

  if ((aFd = inotify_init1(IN_NONBLOCK)) == -1) {
    fprintf(stderr, "%s cannot be initialized.", aFd.name().c_str());
    return false;
  }

  return true;
}

bool Inotify::initEpollFd(Fd &aEpollFd) {
  if (aEpollFd != -1) {
    Fd::close(aEpollFd);
  }

  if ((aEpollFd = epoll_create1(0)) == -1) {
    fprintf(stderr, "$s cannot be created.", aEpollFd.name().c_str());
    return false;
  }

  return true;
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

bool Inotify::initPipeFd(PipeFd &aPipeFd) {
  if (pipe2(aPipeFd.data(), O_NONBLOCK) == -1) {
    fprintf(stderr, "%s cannot be created.", aPipeFd.name().c_str());
  }
}

bool Inotify::closePipeFd(int *aPipeFd, int aPipeFdSize) {}

// bool Inotify::initStopPipeFd(int aStopPipeFd[2])
//{}
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

} // namespace Atina