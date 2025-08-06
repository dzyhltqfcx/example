#ifndef INTERFACES_H
#define INTERFACES_H

#include <QObject>

// 全局信号接口类（单例模式）
class Interfaces : public QObject {
    Q_OBJECT
public:
    // 获取单例实例
    static Interfaces* instance();

signals:
    // 改变点云颜色信号（红色）
    void pclChangeColor();
    // 恢复点云初始颜色信号
    void pclRestoreColor();

private:
    // 私有构造函数（单例模式）
    Interfaces() = default;
    // 单例实例指针
    static Interfaces* m_instance;
};

#endif // INTERFACES_H
