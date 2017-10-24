#pragma once

#include <chrono>
#include "oqpi/platform.hpp"
#include "oqpi/error_handling.hpp"

#include <thread>
#include <pthread.h>

namespace oqpi {

    using thread_impl = class linux_thread;
    class linux_thread
    {
    protected:
        //------------------------------------------------------------------------------------------
        static uint32_t hardware_concurrency()
        {
            return std::thread::hardware_concurrency();;
        }

    protected:
        using native_handle_type    = std::thread::native_handle_type;
        using id                    = std::thread::id;

    public:
        static void set_core_affinity_mask(native_handle_type handle, core_affinity affinityMask);
        static core_affinity get_core_affinity_mask(native_handle_type handle);
        static void set_priority(native_handle_type handle, thread_priority priority);
        static thread_priority get_priority(native_handle_type handle);
        static void set_name(id threadId, const char *name);
    };

    namespace this_thread {

        //------------------------------------------------------------------------------------------
        inline void set_name(const char *name)
        {
            linux_thread::set_name(std::this_thread::get_id(), name);
        }
        //------------------------------------------------------------------------------------------
        template<typename _Rep, typename _Period>
        inline void sleep_for(const std::chrono::duration<_Rep, _Period>& relTime)
        {
            std::this_thread::sleep_for(relTime);
        }
        //------------------------------------------------------------------------------------------
        /*inline uint32_t get_current_core()
        {
            // Retrieves the number of the processor the current thread was running on during
            // the call to this function.
            return GetCurrentProcessorNumber();
        }*/
        //------------------------------------------------------------------------------------------
        inline void yield() noexcept
        {
            // Causes the calling thread to yield execution to another thread that is ready to run
            // on the current processor. The operating system selects the next thread to be executed.
            std::this_thread::yield();
        }
        //------------------------------------------------------------------------------------------
        inline void set_priority(thread_priority threadPriority)
        {
            auto id = std::this_thread::get_id();
            pthread_t h = pthread_self();
            linux_thread::set_priority(h, threadPriority);
        }
        //------------------------------------------------------------------------------------------
        inline void set_affinity_mask(core_affinity coreAffinityMask)
        {
            pthread_t h = pthread_self();
            linux_thread::set_core_affinity_mask(h, coreAffinityMask);
        }
        //------------------------------------------------------------------------------------------
        inline auto get_id()
        {
            return std::this_thread::get_id();
        }
    }
}
