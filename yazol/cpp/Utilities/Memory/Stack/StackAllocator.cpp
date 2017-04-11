#include <Utilities/Memory/Stack/StackAllocator.hpp>
#include <Utilities/Memory/AllocationHeaderBuilder.hpp>
#include <memory>

namespace Yazol
{
    namespace Utilities
    {
        namespace Memory
        {
            StackAllocator::StackAllocator() : m_top(nullptr) {}

            void StackAllocator::Initialize(const size_t& p_memorySize)
            {
                // Allocate the memory to use, the stackallocator does not use a global alignment.
                MemoryAllocator::Initialize(p_memorySize, 0);
                Clear();
            }

            void* StackAllocator::AllocateAligned(const size_t& p_memorySize, const uint8_t& p_alignment)
            {
                if(p_alignment % 2 != 0)
                {
                    // TODO logging
                    throw std::runtime_error("Alignment must be a tuple of 2.");
                }
                if(p_alignment == 0)
                {
                    // TODO logging
                    throw std::runtime_error("Alignment must be larger than 1.");
                }

                // Compute the desired memorysize
                const size_t expandedSize = p_memorySize + p_alignment;

                // Fetch the unaligned data.
                void* rawAdress = AllocateUnaligned(expandedSize);

                // Compute adjustment
                const uint8_t adjustment = ComputeAdjustment(rawAdress, p_alignment);

                // Final adress
                void* alignedAdress = reinterpret_cast<void*>(reinterpret_cast<size_t>(rawAdress) + adjustment);

                // Set adjustment metadata
                AllocationHeaderBuilder::SetAdjustment(alignedAdress, adjustment);

                return alignedAdress;
            }

            void* StackAllocator::AllocateUnaligned(const size_t& p_newMemorySize)
            {
                if(m_occupiedMemory + p_newMemorySize < m_totalMemory)
                {
                    void* returnAdress = m_top;
                    m_top = reinterpret_cast<void*>(reinterpret_cast<size_t>(m_top) + p_newMemorySize);

                    // Update the currently occupied data information
                    m_occupiedMemory += p_newMemorySize;

                    return returnAdress;
                }
                else
                {
                    throw std::runtime_error("Error allocating - Not enough memory left in this stack.");
                    // TODORT maybe allocate dynamically instead?
                    //
                    // TODORT log
                }
            }

            void StackAllocator::Clear()
            {
                m_top = GetAdressStartAligned();
                m_occupiedMemory = 0;
            }

            void StackAllocator::FreeToMarker(const MemoryMarker& p_marker)
            {
                if(p_marker.VerifyManager(this))
                {
                    void* adress = p_marker.GetMarkerAdress();
                    if(reinterpret_cast<size_t>(adress) > reinterpret_cast<size_t>(m_top))
                    {
                        // Compute adjustment
                        const uint8_t adjustment = AllocationHeaderBuilder::GetAdjustment(adress);

                        // Compute the new top of the stack
                        m_top = reinterpret_cast<void*>(reinterpret_cast<size_t>(adress) - adjustment);

                        // Compute the new total available data
                        m_occupiedMemory = reinterpret_cast<size_t>(m_top) - reinterpret_cast<size_t>(GetAdressStartAligned());
                    }
                    else
                    { /* The marker is no longer valid, it points to memory that has already been released. */
                    }
                }
                else
                {
                    // TODO logging
                    throw std::runtime_error("Wrong marker used.");
                }
            }

            MemoryMarker StackAllocator::GetMarker() { return MemoryMarker(m_top, this); }
        }
    }
}