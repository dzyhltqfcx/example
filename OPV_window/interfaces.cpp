#include "interfaces.h"

// 正确初始化静态成员
Interfaces* Interfaces::m_instance = nullptr;

Interfaces* Interfaces::instance() {
    if (!m_instance) {
        m_instance = new Interfaces();
    }
    return m_instance;
}
