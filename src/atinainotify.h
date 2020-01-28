#ifndef ATINA_INOTIFY_H
#define ATINA_INOTIFY_H

#include "atinafd.h"

#include <sys/epoll.h>

namespace Atina {

class Inotify {
public:
  Inotify();

  ~Inotify();

  bool open();

  void close();
  //
  //  bool addWatch(const std::string &aPathName, std::uint32_t aEventMask);
  //
  //  void removeWatch(const std::string &aPathName);
  //
  //  void removeAllWatches();

protected:
  static bool initFd(Fd &aFd);

  static bool initEpollFd(Fd &aEpollFd);

  static bool addEpollCtl(const int &aEpollFd, const int &aFd,
                          epoll_event &epollEvent);

  static void delEpollCtl(const int &aEpollFd, const int &aFd);

  static epoll_event collectEpollEvent(const int &aEpollFd, const int &aFd);

  static bool initPipeFd(int *aPipeFd, int aPipeFdSize);

  static bool closePipeFd(int *aPipeFd, int aPipeFdSize);

  //  void removeWatch(int aWd, bool aIsRemoveFromCollection = true);
  //
  //  std::string wdToPathName(int aWd);
  //
  //  int pathNameToWd(const std::string &aPathName);

protected:
  Fd mFd{"Inotify file descriptor"};

  Fd mEpollFd{"Epoll file descriptor"};

  epoll_event mEpollEvent;

  int mStopPipeFd[2]{-1, -1};

  epoll_event mStopPipeEpollEvent;

  const int mPipeReadIdx{0};

  const int mPipeWriteIdx{1};
  //
  //  std::unordered_map<int, std::string> mWds;
};

} // namespace Atina

#endif // ATINA_INOTIFY_H
