#include "atinafd.h"

namespace Atina {

Fd::Fd(int aValue) : mValue{aValue} {}

Fd::Fd(const Fd &aOther) : mValue{aOther.mValue} {}

Fd &Fd::operator=(const Fd &aOther) {
  if (&aOther == this) {
    return *this;
  }

  mValue = aOther.mValue;

  return *this;
}

Fd::~Fd() {
  if (isValid(mValue) && close(mValue) != 0) {
    fprintf(stderr, "File descriptor cannot be normally closed.");
  }
}

int Fd::invalid() { return -1; }

Fd::operator int() const {}

Fd &Fd::operator=(int aValue) {
  mValue = aValue;

  return *this;
}

bool Fd::isValid(int aValue) { return aValue >= 0; }

bool Fd::isValid() const { return isValid(mValue); }

} // namespace Atina