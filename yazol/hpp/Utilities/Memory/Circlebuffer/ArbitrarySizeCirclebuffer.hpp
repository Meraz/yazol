#pragma once
#include <cstdint>
#include <yazol/hpp//Utilities/PointerArithmetic/PointerArithmetic.hpp>
#include <yazol/hpp//Utilities/Memory/Circlebuffer/CircleBufferHeader.hpp>
#include <yazol/hpp//Utilities/IO/Mutex/Mutex.hpp>

namespace Yazol
{
    namespace Utilities
    {
        namespace Memory
        {
            struct ArbitraryStaticData;

            class ArbitrarySizeCirclebuffer
            {
            public:
                /**
                TODO docs
                */
                ArbitrarySizeCirclebuffer();

                /**
                TODO docs
                */
                virtual ~ArbitrarySizeCirclebuffer();

                /**
                The useable size of the buffer will be p_bufferSize - sizeof(StaticData).
                Each object will allocate sizeof(T) + sizeof(CircleBufferHeader).
                */
                void Initialize(const uint32_t& p_bufferSize, IO::Mutex* p_metaDataMutex = nullptr);

                /**
                The useable size of the buffer will be p_bufferSize - sizeof(StaticData).
                Each object will allocate sizeof(T) + sizeof(CircleBufferHeader).
                */
                void Initialize(void* const p_preAllocatedBuffer, const uint32_t& p_bufferSize, IO::Mutex* p_metaDataMutex = nullptr);

                /**
                Threadsafe, internal loackage. Throws exception if not possible.
                */
                bool Produce(const CircleBufferHeader& p_Header, const void* const p_data);

                /**
                Threadsafe, internal loackage. Returns false if there was nothing to read.
                */
                bool Consume(CircleBufferHeader*& o_header, void* o_dataBuffer, const uint32_t& p_outbufferSize);

            protected:
                void AssertInitialize(const uint32_t& p_bufferSize);

                void LockMetaData();

                void UnLockMetaData();

                void ComputeAdjustments();

                void SetupVariables();
                void ResetMetaData();

                ArbitraryStaticData* m_data;
                void* m_rawBufferPointerStart;
                void* m_rawBufferPointerEnd;
                void* m_adjustedBufferPointerStart;
                void* m_adjustedBufferPointerEnd;
                uint32_t m_rawBufferSize;
                uint32_t m_adjustedBufferSize;
                bool m_internalMemoryManagement;
                std::mutex m_produceLock;
                std::mutex m_consumeLock;
                bool m_alreadyInitialized;
                IO::Mutex* m_metaDataMutex;
            };
        }
    }
}