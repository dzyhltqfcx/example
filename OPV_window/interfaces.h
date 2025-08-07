#ifndef INTERFACES_H
#define INTERFACES_H

#include <QObject>

class Interfaces : public QObject {
    Q_OBJECT
public:
    static Interfaces* instance();

signals:
    void pclChangeColor();
    void pclRestoreColor();

private:
    Interfaces() = default;
    static Interfaces* m_instance;
};

#endif // INTERFACES_H
