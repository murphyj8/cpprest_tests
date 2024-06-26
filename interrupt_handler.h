#ifndef __INTERRUPT_HANDLER_H__
#define __INTERRUPT_HANDLER_H__

#include <condition_variable>
#include <mutex>
#include <memory>
#include <signal.h>

class InterruptHandler{
    public:
        static std::unique_ptr<InterruptHandler>& Instance(); 
        void hookSIGINT();
        void waitForUserInterrupt(); 
    private:
        InterruptHandler(); 
        InterruptHandler(const InterruptHandler&) = delete; 
        InterruptHandler(InterruptHandler&&j) = delete;
        InterruptHandler& operator=(InterruptHandler&&) = delete;
        InterruptHandler& operator=(InterruptHandler&) = delete;

        static void handleUserInterrupt(int);
        static std::unique_ptr<InterruptHandler> m_Instance; 
        static std::once_flag m_onceFlag;

        std::condition_variable m_condition;
        std::mutex m_mutex;
}; 

#endif //#ifndef __INTERRUPT_HANDLER_H__
