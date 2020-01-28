#ifndef ATINA_PIPEFD_H
#define ATINA_PIPEFD_H

#include <array>

#include "atinafd.h"

#define ATINA_PIPE_FD_SIZE 2

namespace Atina {

class PipeFd {
public:
  explicit PipeFd(const std::array<int, ATINA_PIPE_FD_SIZE> &aValue = invalid());

  explicit PipeFd(std::string aName, const std::array<int, ATINA_PIPE_FD_SIZE> &aValue = invalid());

  PipeFd(const PipeFd &aOther) = default;

  PipeFd &operator=(const PipeFd &aOther);

  ~PipeFd() = default;

  explicit operator std::array<int, ATINA_PIPE_FD_SIZE>() const;

  PipeFd &operator=(const std::array<int, ATINA_PIPE_FD_SIZE> &aValue);

  bool operator==(const PipeFd &aOther) const;

  bool operator!=(const PipeFd &aOther) const;

  bool operator==(const std::array<int, ATINA_PIPE_FD_SIZE> &aValue) const;

  bool operator!=(const std::array<int, ATINA_PIPE_FD_SIZE> &aValue) const;

  static constexpr const char *defaultName() { return "Pipe file descriptor"; }

  static constexpr int readChannelIndex() { return 0; }

  static constexpr int writeChannelIndex() { return 1; }

  static constexpr std::array<int, ATINA_PIPE_FD_SIZE> invalid() {
    return {Fd::invalid(), Fd::invalid()};
  }

  static bool validate(const std::array<int, ATINA_PIPE_FD_SIZE> &aValue,
                       const char *aName, bool aIsDisplayError = true);

  static bool validate(const std::array<int, ATINA_PIPE_FD_SIZE> &aValue,
                       const std::string &aName, bool aIsDisplayError = true);

  static bool validate(const PipeFd &aPipeFd, bool aIsDisplayError = true);

  static int isValid(const std::array<int, ATINA_PIPE_FD_SIZE> &aValue);

  static void close(std::array<int, ATINA_PIPE_FD_SIZE> &aValue,
                    const char *aName = defaultName());

  static void close(std::array<int, ATINA_PIPE_FD_SIZE> &aValue,
                    const std::string &aName = defaultName());

  static void close(PipeFd &aPipeFd);

  const std::string &name() const;

  const int *data() const;

  int *data();

  bool isValid() const;

private:
  std::string mName;

  std::array<int, ATINA_PIPE_FD_SIZE> mValue;
};
} // namespace Atina

#endif // ATINA_PIPEFD_H
