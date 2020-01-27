#ifndef INOTIFY_H
#define INOTIFY_H

#include <sys/epoll.h>
//
//#include <string>
//
//#include <bits/unordered_set.h>
//#include <unordered_map>

class Inotify {
public:
  Inotify();

  ~Inotify();

  bool init();

  void deinit();
  //
  //  bool addWatch(const std::string &aPathName, std::uint32_t aEventMask);
  //
  //  void removeWatch(const std::string &aPathName);
  //
  //  void removeAllWatches();

protected:
  static bool initFd(int &aFd);

  static bool validateFd(const int &aFd, bool aIsDisplayError = true);

  static void closeFd(int &aFd);

  static bool initEpollFd(int &aEpollFd);

  static bool validateEpollFd(const int &aEpollFd, bool aIsDisplayError = true);

  static void closeEpollFd(int &aEpollFd);

  static bool addEpollCtl(const int &aEpollFd, const int &aFd,
                          epoll_event &epollEvent);

  static void delEpollCtl(const int &aEpollFd, const int &aFd);

  static epoll_event collectEpollEvent(const int &aEpollFd, const int &aFd);

  //  void removeWatch(int aWd, bool aIsRemoveFromCollection = true);
  //
  //  std::string wdToPathName(int aWd);
  //
  //  int pathNameToWd(const std::string &aPathName);

protected:
  int mFd{-1};

  int mEpollFd{-1};

  epoll_event mEpollEvent;
  //
  //  std::unordered_map<int, std::string> mWds;
};

#endif // INOTIFY_H
