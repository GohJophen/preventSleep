#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>

#include <QSettings>

#include <QTimer>

#include <QShortcut>

#include <QSystemTrayIcon>

#include <QCloseEvent>


#include "simulate_key_press.h"


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    //标签
    QLabel* lblIntervalPrefix;//文本标签“每隔”
    QLabel* lblIntervalSuffix;//文本标签"秒，唤醒一次操作系统；"
    QLabel* lblNumOfTimesPrefix;//文本标签"唤醒"
    QLabel* lblNumOfTimesSuffix;//文本标签"次；"
    QLabel* lblTakeTimePrefix;//文本标签"防休眠总时长:"
    QLabel* lblTakeTime;//文本标签;“防休眠总时长数值”
    QLabel* lblTakeTimeSuffix;//文本标签"秒"
    QLabel* lblState;

    //数字输入框
    QSpinBox* spnInterval;//间隔时间数值选择器
    QSpinBox* spnNumOfTimes;//发送次数数值选择器

    //复选框
    QCheckBox* chkIfPerpetual;//"永久循环"复选框

    //按钮
    QPushButton* btnOnOrOff;//开始/结束按钮
    QPushButton* btnToSystemTray;

    //布局
    QHBoxLayout* hlayout1;//第一行布局
    QHBoxLayout* hlayout2;//第二行布局
    QHBoxLayout* hlayout3;//第三行布局
    QVBoxLayout* vlayoutMain;//主布局(第一行、第二行和第三行垂直)

    //数据
    int interval;
    int numOfTimes;
    bool ifPerpetual;

    //定时器
    QTimer* timer;

    //普通函数
    void timerStart();
    void timeOut();

    //用户配置数据
    QSettings settings;
    int defaultInterval;
    int defaultNumOfTimes;

    //快捷键
    QShortcut* shortcutOnOrOff;

    //图标地址
    QString trayIconPath;

    //系统托盘图标
    QSystemTrayIcon *trayIcon;

    //重写事件函数
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onSpinBoxValueChanged();
    void OnOrOff();
    void OnPreventSleep();
    void OffPreventSleep();
    void toSystemTray();
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
};
#endif // WIDGET_H
