#include "atinapipefd.h"

#include <unistd.h>

namespace Atina {

PipeFd::PipeFd(const std::array<int, ATINA_PIPE_FD_SIZE> &aValue)
    : mName{defaultName()}, mValue{aValue} {}

PipeFd::PipeFd(std::string aName,
               const std::array<int, ATINA_PIPE_FD_SIZE> &aValue)
    : mName{std::move(aName)}, mValue{aValue} {};

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

bool PipeFd::operator==(const PipeFd &aOther) const {
  return aOther.mValue == mValue;
}

bool PipeFd::operator!=(const PipeFd &aOther) const {
  return !operator==(aOther);
}

bool PipeFd::operator==(
    const std::array<int, ATINA_PIPE_FD_SIZE> &aValue) const {
  return aValue == mValue;
}

bool PipeFd::operator!=(
    const std::array<int, ATINA_PIPE_FD_SIZE> &aValue) const {
  return !operator==(aValue);
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
  if (::close(aValue[readChannelIndex()]) != 0 ||
      ::close(aValue[writeChannelIndex()]) != 0) {
    fprintf(stderr, "%s cannot be normally closed.", aName);
  }

  aValue = invalid();
}

void PipeFd::close(std::array<int, ATINA_PIPE_FD_SIZE> &aValue,
                   const std::string &aName) {
  close(aValue, aName.c_str());
}

void PipeFd::close(PipeFd &aPipeFd) { close(aPipeFd.mValue, aPipeFd.mName); }

const std::string &PipeFd::name() const { return mName; }

const int *PipeFd::data() const { return mValue.data(); }

int *PipeFd::data() { return mValue.data(); }

bool PipeFd::isValid() const { return isValid(mValue); }

} // namespace Atina