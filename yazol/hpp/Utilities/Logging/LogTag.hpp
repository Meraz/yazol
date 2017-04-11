#pragma once
#include <array>
#include <cstdint>

namespace Yazol
{
    namespace Utilities
    {
        namespace Logging
        {
            enum class LogTag : uint8_t
            {
                GRAPHIC,
                NETWORK,
                CLIENT,
                SERVER,
                GENERAL,
                NOTAG,
                PHYSICS,
                AUDIO,
                GAME,
                COMPONENT,
                GUI,
                INPUT,
                TIMER,
                ANIMATION,
                PARTICLE,
                MEMORY,
                ENGINE_CORE
            };

            const std::array<LogTag, 17> ALL_LOG_TAGS = {LogTag::GRAPHIC, LogTag::NETWORK,    LogTag::CLIENT, LogTag::SERVER,    LogTag::GENERAL,
                                                         LogTag::NOTAG,   LogTag::PHYSICS,    LogTag::AUDIO,  LogTag::GAME,      LogTag::COMPONENT,
                                                         LogTag::GUI,     LogTag::INPUT,      LogTag::TIMER,  LogTag::ANIMATION, LogTag::PARTICLE,
                                                         LogTag::MEMORY,  LogTag::ENGINE_CORE};
        }
    }
}