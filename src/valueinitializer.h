#ifndef VALUEINITIALIZER_H
#define VALUEINITIALIZER_H

#include <functional>

#include <utility>

template <typename Type> class ValueInitializer {
  typedef std::function<bool(Type &)> Handler;

  typedef std::function<void(Type &)> DeinitializerHandler;

public:
  ValueInitializer(const DeinitializerHandler &aDeinitializerHandler,
                   const Handler &aHandler = nullptr)
      : ValueInitializer({false, {}}, aDeinitializerHandler, aHandler) {}

  ValueInitializer(const std::pair<bool, Type> &aValue,
                   const DeinitializerHandler &aDeinitializerHandler,
                   const Handler &aHandler = nullptr)
      : mDeinitializerHandler{aDeinitializerHandler}, mValue{aValue},
        mIsValueTaken{false} {
    if (aHandler != nullptr) {
      mValue.first = aHandler(mValue.second);
    }
  }

  ValueInitializer(const ValueInitializer &aOther) = delete;

  ValueInitializer &operator=(const ValueInitializer &aOther) = delete;

  ~ValueInitializer() {
    if (mIsValueTaken || !mValue.first || mDeinitializerHandler == nullptr) {
      return;
    }

    mDeinitializerHandler(mValue.second);

    mValue.first = false;
  }

  const Type &value() const { return mValue.second; }

  void setValue(const std::pair<bool, Type> &aValue) {
    if (aValue == mValue) {
      return;
    }

    mValue = aValue;
  }

  bool isOk() const { return mValue.first; }

  const Type &takeValue() {
    mIsValueTaken = true;

    return mValue.second;
  }

private:
  DeinitializerHandler mDeinitializerHandler;

  std::pair<bool, Type> mValue;

  bool mIsValueTaken;
};

#endif // VALUEINITIALIZER_H
