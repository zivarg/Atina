#ifndef ATINA_FD_H
#define ATINA_FD_H

#include <assert.h>

#include <memory>

#include <unistd.h>

namespace Atina {

class Fd {
public:
  Fd() = default;

  explicit Fd(int aValue);

  Fd(const Fd &aOther);

  Fd &operator=(const Fd &aOther);

  ~Fd();

  static int invalid();

  static bool isValid(int aValue);

  operator int() const;

  Fd &operator=(int aValue);

  bool isValid() const;

private:
  int mValue{invalid()};
};

} // namespace Atina

#endif // ATINA_FD_H
