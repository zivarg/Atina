#include "atinafd.h"

#include <unistd.h>

#include <stdio.h>

#include <utility>

namespace Atina {

Fd::Fd(int aValue) : mName{defaultName()}, mValue{aValue} {}

Fd::Fd(std::string aName, int aValue)
    : mName{std::move(aName)}, mValue{aValue} {};

Fd::Fd(const Fd &aOther) : mName{aOther.mName}, mValue{aOther.mValue} {}

Fd &Fd::operator=(const Fd &aOther) {
  if (&aOther == this) {
    return *this;
  }

  mName = aOther.mName;

  mValue = aOther.mValue;

  return *this;
}

Fd::operator int() const { return mValue; }

Fd &Fd::operator=(int aValue) {
  if (aValue == mValue) {
    return *this;
  }

  mValue = aValue;

  return *this;
}

bool Fd::validate(const int &aValue, const char *aName, bool aIsDisplayError) {
  if (isValid(aValue)) {
    return true;
  }

  if (aIsDisplayError) {
    fprintf(stderr, "%s descriptor is not valid.",
            aName != nullptr ? aName : defaultName());
  }

  return false;
}

bool Fd::validate(const int &aValue, const std::string &aName,
                  bool aIsDisplayError) {
  return validate(aValue, aName.c_str(), aIsDisplayError);
}

bool Fd::validate(const Fd &aFd, bool aIsDisplayError) {
  return validate(aFd.mValue, aFd.mName, aIsDisplayError);
}

int Fd::isValid(int aValue) { return aValue >= 0; }

void Fd::close(int &aValue, const char *aName) {
  if (::close(aValue) != 0) {
    fprintf(stderr, "%s cannot be normally closed.", aName);
  }

  aValue = invalid();
}

void Fd::close(Fd &aValue) { close(aValue.mValue, aValue.mName.c_str()); }

const std::string &Fd::name() const { return mName; }

bool Fd::isValid() const { return isValid(mValue); }

} // namespace Atina