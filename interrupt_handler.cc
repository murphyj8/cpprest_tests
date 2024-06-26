#include <iostream>
#include <interrupt_handler.h>

std::unique_ptr<InterruptHandler> InterruptHandler::m_Instance;
std::once_flag InterruptHandler::m_onceFlag;

std::unique_ptr<InterruptHandler>& InterruptHandler::Instance(){
    std::call_once(m_onceFlag, []{m_Instance.reset(new InterruptHandler);}); 
    return m_Instance; 
 }

InterruptHandler::InterruptHandler(): m_condition(), m_mutex(){
    return; 
}

void InterruptHandler::hookSIGINT() {
    signal(SIGINT, handleUserInterrupt);
}

void InterruptHandler::waitForUserInterrupt() {
    std::unique_lock<std::mutex> lock{m_mutex};
    m_condition.wait(lock);
    std::cout << "user has signaled to interrupt program..." << '\n';
}

void InterruptHandler::handleUserInterrupt(int signal){
    if(signal == SIGINT){
        std::cout << "SIGINT trapped ..  "  <<  std::endl;
        std::unique_lock<std::mutex> lock{m_Instance->m_mutex}; 
        m_Instance->m_condition.notify_one(); 
    }
    return; 
}