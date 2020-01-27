#ifndef INOTIFYEVENT_H
#define INOTIFYEVENT_H

#include <string>

#include <sys/inotify.h>

class InotifyEvent {
public:
    enum Mask {
        Mask_Access = IN_ACCESS,
        Mask_Modify = IN_MODIFY,
        Mask_Attrib = IN_ATTRIB,
        Mask_CloseWrite = IN_CLOSE_WRITE,
        Mask_CloseNoWrite = IN_CLOSE_NOWRITE,
        Mask_Close = Mask_CloseWrite | Mask_CloseNoWrite,
        Mask_Open = IN_OPEN,
        Mask_MovedFrom = IN_MOVED_FROM,
        Mask_MovedTo = IN_MOVED_TO,
        Mask_Move = Mask_MovedFrom | Mask_MovedTo,
        Mask_Create = IN_CREATE,
        Mask_Delete = IN_DELETE,
        Mask_DeleteSelf = IN_DELETE_SELF,
        Mask_MoveSelf = IN_MOVE_SELF,
        Mask_Unmount = IN_UNMOUNT,
        Mask_QOverflow = IN_Q_OVERFLOW,
        Mask_Ignored = IN_IGNORED,
        Mask_OnlyDir = IN_ONLYDIR,
        Mask_DontFollow = IN_DONT_FOLLOW,
        Mask_ExclUnlink = IN_EXCL_UNLINK,
        Mask_MaskAdd = IN_MASK_ADD,
        Mask_IsDir = IN_ISDIR,
        Mask_OneShot = IN_ONESHOT,
        Mask_AllEvents = Mask_Access | Mask_Modify | Mask_Attrib | Mask_CloseWrite | Mask_CloseNoWrite | Mask_Open
                         | Mask_MovedFrom | Mask_MovedTo | Mask_Create | Mask_Delete | Mask_DeleteSelf | Mask_MoveSelf
    };
public:
    InotifyEvent(int aWd, std::uint32_t aMask, std::uint32_t aCookie, const std::string &aName);

    ~InotifyEvent();

    int wd() const;

    std::uint32_t mask() const;

    std::uint32_t cookie() const;

    const std::string &name() const;

protected:
    int mWd;

    std::uint32_t mMask;

    std::uint32_t mCookie;

    std::string mName;
};

#endif //INOTIFYEVENT_H
