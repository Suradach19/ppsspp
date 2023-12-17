#pragma once

#include "Core/HLE/HLE.h"
#include "Core/Net/SceSocket.h"

#if PPSSPP_PLATFORM(WINDOWS)
#include <winsock.h>
#endif

#include <memory>
#include <shared_mutex>
#include <unordered_map>


enum {
    // pspnet_inet
    ERROR_NET_INET_ALREADY_INITIALIZED		= 0x80410201,
    ERROR_NET_INET_SOCKET_BUSY				= 0x80410202,
    ERROR_NET_INET_CONFIG_INVALID_ARG		= 0x80410203,
    ERROR_NET_INET_GET_IFADDR				= 0x80410204,
    ERROR_NET_INET_SET_IFADDR				= 0x80410205,
    ERROR_NET_INET_DEL_IFADDR				= 0x80410206,
    ERROR_NET_INET_NO_DEFAULT_ROUTE			= 0x80410207,
    ERROR_NET_INET_GET_ROUTE				= 0x80410208,
    ERROR_NET_INET_SET_ROUTE				= 0x80410209,
    ERROR_NET_INET_FLUSH_ROUTE				= 0x8041020a,
    ERROR_NET_INET_INVALID_ARG				= 0x8041020b,
};

class SceNetInet {
public:
    static bool Init();
    static bool Shutdown();
    static std::shared_ptr<SceNetInet> Get() {
        return gInstance;
    }

    // TODO: document

    int GetLastError();
    void SetLastError(int error);
    int SetLastErrorToMatchPlatform();
    std::shared_ptr<SceSocket> CreateAndAssociateSceSocket(int nativeSocketId);
    std::shared_ptr<SceSocket> GetSceSocket(int sceSocketId);
    bool GetNativeSocketIdForSceSocketId(int &nativeSocketId, int sceSocketId);
    bool EraseNativeSocket(int sceSocketId);
    bool TranslateSceFdSetToNativeFdSet(int& maxFd, fd_set &destFdSet, u32 fdsPtr) const;

private:
    void CloseAllRemainingSockets() const;

    static std::shared_ptr<SceNetInet> gInstance;
    static std::shared_mutex gLock;

    int mLastError = 0;
    std::unordered_map<int, std::shared_ptr<SceSocket>> mSceSocketIdToNativeSocket;
    int mCurrentSceSocketId = 0;
    std::shared_mutex mLock;
};

void Register_sceNetInet();
