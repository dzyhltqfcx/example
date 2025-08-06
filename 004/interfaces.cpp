#include "interfaces.h"

// 初始化单例指针
Interfaces* Interfaces::m_instance = nullptr;

// 获取单例实例（线程不安全，适用于GUI单线程）
Interfaces* Interfaces::instance() {
    if (!m_instance) {
        m_instance = new Interfaces();
    }
    return m_instance;
}
