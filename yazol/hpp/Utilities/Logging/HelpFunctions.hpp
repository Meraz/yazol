#pragma once
#include <cstdint>
#include <yazol/hpp/Utilities/Constants/LoggerConstants.hpp> // TODORT can move to cpp
#include <yazol/hpp/Utilities/String/StringHelper.hpp> // TODORT can move to cpp

#include <string> // TODORT can move to cpp
#include <time.h>

namespace Yazol
{
    namespace Utilities
    {
        namespace Logging
        {
            /**
            TODORT, can move to cpp
            TODORT, docs
            */
            static std::string BuildFileMapName(const uint32_t& p_uniqueId)
            {
                using namespace std;
                return string(Constants::IPC_DEFAULT_FILEMAP_NAME + to_string(p_uniqueId));
            }

            /**
            TODORT, can move to cpp
            TODORT, docs
            */
            static std::string BuildFileMapMutexName(const uint32_t& p_uniqueId)
            {
                using namespace std;
                return string(Constants::IPC_FILEMAP_MUTEX_NAME + to_string(p_uniqueId));
            }

            /**
            TODORT, can move to cpp
            TODORT, docs
            */
            static std::string BuildFolderNameBasedOnTime()
            {
                using namespace std;
                using namespace Yazol::Utilities::String;
                time_t rawTime;
                time(&rawTime);
                struct tm& timeInfo = *localtime(&rawTime);

                string fileName = "logs/";
                fileName.append(intToStringAtleastTwoDigits(timeInfo.tm_year + 1900) + "-");
                fileName.append(intToStringAtleastTwoDigits(timeInfo.tm_mon + 1) + "-");
                fileName.append(intToStringAtleastTwoDigits(timeInfo.tm_mday) + "_");
                fileName.append(intToStringAtleastTwoDigits(timeInfo.tm_hour) + ".");
                fileName.append(intToStringAtleastTwoDigits(timeInfo.tm_min));
                return fileName;
            }
        }
    }
}
