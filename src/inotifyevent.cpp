#include "inotifyevent.h"

InotifyEvent::InotifyEvent(int aWd, std::uint32_t aMask, std::uint32_t aCookie, const std::string &aName) :
        mWd{aWd}, mMask{aMask}, mCookie{aCookie}, mName{aName} {}

InotifyEvent::~InotifyEvent() {}

int InotifyEvent::wd() const {
    return mWd;
}

std::uint32_t InotifyEvent::mask() const {
    return mMask;
}

std::uint32_t InotifyEvent::cookie() const {
    return mCookie;
}

const std::string &InotifyEvent::name() const {
    return mName;
}

