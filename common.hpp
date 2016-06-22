#ifndef BELL_COMMON_HXX
#define BELL_COMMON_HXX

#include <TelepathyQt/ProtocolInterface>

namespace Bell
{

class Common {
public:
    static Tp::SimpleStatusSpecMap getSimpleStatusSpecMap();
    static Tp::AvatarSpec getAvatarSpec();
};

}

#endif
