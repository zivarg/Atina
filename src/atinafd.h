#ifndef ATINA_FD_H
#define ATINA_FD_H

#include <string>

namespace Atina {

class Fd {
public:
  Fd(int aValue = invalid());

  Fd(std::string aName, int aValue = invalid());

  Fd(const Fd &aOther);

  Fd &operator=(const Fd &aOther);

  ~Fd() = default;

  explicit operator int() const;

  Fd &operator=(int aValue);

  static constexpr const char *defaultName() { return "File descriptor"; }

  static constexpr int invalid() { return -1; }

  static bool validate(const int &aValue, const char *aName,
                       bool aIsDisplayError = true);

  static bool validate(const int &aValue, const std::string &aName,
                       bool aIsDisplayError = true);

  static bool validate(const Fd &aFd, bool aIsDisplayError = true);

  static int isValid(int aValue);

  static void close(int &aValue, const char *aName = defaultName());

  static void close(int &aValue, const std::string &aName = defaultName());

  static void close(Fd &aFd);

  const std::string &name() const;

  bool isValid() const;

private:
  std::string mName;

  int mValue;
};

} // namespace Atina

#endif // ATINA_FD_H
