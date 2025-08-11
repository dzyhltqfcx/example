#include "interfaces.h"

Interfaces* Interfaces::m_instance = nullptr;

Interfaces* Interfaces::instance() {
    if (!m_instance) {
        m_instance = new Interfaces();
    }
    return m_instance;
}
