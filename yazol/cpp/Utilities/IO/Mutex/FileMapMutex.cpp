#include <IO/FileMap/FileMapMutex.hpp>

#include <Windows.h>
#include <Utility/String/StringHelper.hpp>

namespace Yazo
{
    namespace Utilities
    {
        namespace IO
        {
            // TODORT Define better than 1000?
            const uint32_t DEFAULT_TIMEOUT = 1000;

            FileMapMutex::FileMapMutex() : m_handle(nullptr) {}

            FileMapMutex::~FileMapMutex()
            {
                ReleaseMutex(m_handle);
                CloseHandle(m_handle);
            }

            bool FileMapMutex::Initialize(const std::string& p_name)
            {
                // Set variables
                m_name = p_name;

                // Set success variable
                bool success;

                // Open/CreateMutex
                success = InitializeExternalMutex();

                if(m_handle)
                {
                    success = true;
                }
                return success;
            }

            bool FileMapMutex::try_lock() { return try_lock(DEFAULT_TIMEOUT); }

            bool FileMapMutex::try_lock(const uint32_t& p_timeout)
            {
                DWORD check;
                check = WaitForSingleObject(m_handle, p_timeout);

                if(check == WAIT_OBJECT_0)
                {
                    return true;
                }
                else if(check == WAIT_TIMEOUT)
                {
                    return false; // Timeouted
                }

                else if(check == WAIT_ABANDONED)
                {
                    return true;
                }

                return false; // Did not get mutex
            }

            void FileMapMutex::lock()
            {
                DWORD check;
                while(true)
                {
                    check = WaitForSingleObject(m_handle, DEFAULT_TIMEOUT);
                    if(check == WAIT_TIMEOUT)
                    {
                        continue; // Timeouted
                    }
                    else if(check == WAIT_OBJECT_0)
                    {
                        break; //
                    }
                    else if(check == WAIT_ABANDONED)
                    {
                        // TODORT Check data, but ownership is mine.
                        break;
                    }
                    else if(check == WAIT_FAILED)
                    {
                        throw std::runtime_error("Failed to lock filemapmutex. Errorcode: " + std::to_string(GetLastError()));
                    }
                }
            }

            void FileMapMutex::unlock() { ReleaseMutex(m_handle); }

            bool FileMapMutex::InitializeExternalMutex()
            {
                m_handle = CreateMutex(nullptr, FALSE, String::s2ws(m_name).c_str());
                if(m_handle != NULL)
                {
                    return true;
                }
                return false;
            }
        }
    }
}