#include "atinapipefd.h"

#include <unistd.h>

namespace Atina {

PipeFd::PipeFd(int aValue) : mName{defaultName()}, mValue{aValue} {}

PipeFd::PipeFd(std::string aName, int aValue)
    : mName{std::move(aName)}, mValue{aValue} {};

PipeFd::PipeFd(const PipeFd &aOther)
    : mName{aOther.mName}, mValue{aOther.mValue} {}

PipeFd &PipeFd::operator=(const PipeFd &aOther) {
  if (&aOther == this) {
    return *this;
  }

  mName = aOther.mName;

  mValue = aOther.mValue;

  return *this;
}

PipeFd::operator std::array<int, ATINA_PIPE_FD_SIZE>() const { return mValue; }

PipeFd &PipeFd::operator=(const std::array<int, ATINA_PIPE_FD_SIZE> &aValue) {
  if (aValue == mValue) {
    return *this;
  }

  mValue = aValue;

  return *this;
}

bool PipeFd::validate(const std::array<int, ATINA_PIPE_FD_SIZE> &aValue,
                      const char *aName, bool aIsDisplayError) {
  if (isValid(aValue)) {
    return true;
  }

  if (aIsDisplayError) {
    fprintf(stderr, "%s is not valid.",
            aName != nullptr ? aName : defaultName());
  }

  return false;
}

bool PipeFd::validate(const std::array<int, ATINA_PIPE_FD_SIZE> &aValue,
                      const std::string &aName, bool aIsDisplayError) {
  return validate(aValue, aName.c_str(), aIsDisplayError);
}

bool PipeFd::validate(const PipeFd &aPipeFd, bool aIsDisplayError) {
  return validate(aPipeFd.mValue, aPipeFd.mName, aIsDisplayError);
}

int PipeFd::isValid(const std::array<int, ATINA_PIPE_FD_SIZE> &aValue) {
  return aValue[readChannelIndex()] >= 0 && aValue[writeChannelIndex()] >= 0;
}

void PipeFd::close(std::array<int, ATINA_PIPE_FD_SIZE> &aValue,
                   const char *aName) {
  if (::close(aValue) != 0) {
    fprintf(stderr, "%s cannot be normally closed.", aName);
  }

  aValue = invalid();
}

void PipeFd::close(std::array<int, ATINA_PIPE_FD_SIZE> &aValue,
                   const std::string &aName) {
  close(aValue, aName.c_str());
}

void PipeFd::close(PipeFd &aPipeFd) { close(aPipeFd.mValue, aPipeFd.mName); }

const std::string &Fd::name() const { return mName; }

const int *data() const;

int *data();

bool Fd::isValid() const { return isValid(mValue); }

} // namespace Atina