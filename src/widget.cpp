#include "widget.h"

Widget::Widget(QWidget *parent): QWidget(parent),settings("GohJophen", "preventSleep"){

    // 读取默认值
    defaultInterval= settings.value("interval", 100).toInt();
    defaultNumOfTimes= settings.value("numOfTimes", 160).toInt();

    //创建 标签实例对象
    lblIntervalPrefix = new QLabel("每隔",this);
    lblIntervalSuffix = new QLabel("秒，唤醒一次操作系统；",this);
    lblNumOfTimesPrefix = new QLabel("唤醒",this);
    lblNumOfTimesSuffix = new QLabel("次；",this);
    lblTakeTimePrefix = new QLabel("防休眠总时长:",this);
    lblTakeTimeSuffix = new QLabel("秒",this);
    lblTakeTime = new QLabel(this);
    lblState = new QLabel(this);

    //创建 按钮实例对象
    btnOnOrOff  =  new QPushButton("开始",this);//开始/结束按钮
    btnToSystemTray = new QPushButton("最小化到系统托盘",this);

    //创建 数字输入框 实例对象
    spnInterval = new QSpinBox(this);
    spnNumOfTimes = new QSpinBox(this);

    //创建 复选框实例对象
    chkIfPerpetual = new QCheckBox("永久防休眠",this);

    //创建 布局实例对象
    hlayout1 = new  QHBoxLayout;
    hlayout2 = new  QHBoxLayout;
    hlayout3 = new  QHBoxLayout;
    vlayoutMain = new  QVBoxLayout(this);


    //数字输入框初始值 以及 相关标签文本的显示设置  的设置
    spnInterval->setRange(1, 86400);  // 设置范围
    spnNumOfTimes->setRange(1, 31622400);
    spnInterval->setValue(defaultInterval);//设置初始值
    spnNumOfTimes->setValue(defaultNumOfTimes);
    lblTakeTime->setText(QString::number(spnInterval->value() * spnNumOfTimes->value()));


    //第1行布局排版
    hlayout1->addWidget(lblIntervalPrefix);
    hlayout1->addWidget(spnInterval);
    hlayout1->addWidget(lblIntervalSuffix);
    hlayout1->addWidget(lblNumOfTimesPrefix);
    hlayout1->addWidget(spnNumOfTimes);
    hlayout1->addWidget(lblNumOfTimesSuffix);
    hlayout1->addWidget(btnOnOrOff);

    //第2行布局排版
    hlayout2->addWidget(lblTakeTimePrefix);
    hlayout2->addWidget(lblTakeTime);
    hlayout2->addWidget(lblTakeTimeSuffix);
    hlayout2->addStretch();
    hlayout2->addWidget(lblState);
    hlayout2->addSpacing(10);

    //第3行布局排版
    hlayout3->addWidget(chkIfPerpetual);
    hlayout3->addSpacing(10);
    hlayout3->addWidget(btnToSystemTray);
    hlayout3->addStretch();

    //主布局 排版
    vlayoutMain->addLayout(hlayout1);
    vlayoutMain->addLayout(hlayout2);
    vlayoutMain->addLayout(hlayout3);

    // 设置窗口的布局
    setLayout(vlayoutMain);

    //绑定信号与槽
    connect(btnOnOrOff,&QPushButton::clicked,this,&Widget::OnOrOff);
    connect(spnInterval, QOverload<int>::of(&QSpinBox::valueChanged), this, &Widget::onSpinBoxValueChanged);
    connect(spnNumOfTimes, QOverload<int>::of(&QSpinBox::valueChanged), this, &Widget::onSpinBoxValueChanged);
    connect(btnToSystemTray,&QPushButton::clicked,this,&Widget::toSystemTray);

    shortcutOnOrOff = new QShortcut(QKeySequence(Qt::Key_Return),this);//Enter键
    connect(shortcutOnOrOff, &QShortcut::activated, this, [=](){ OnOrOff(); });

    //系统托盘
    trayIcon = new QSystemTrayIcon(this);// 创建托盘图标
    trayIconPath = ":/icons/Pictogrammers-Material-Sleep-off.ico";
    trayIcon->setIcon(QIcon(trayIconPath)); // 设置托盘图标
    trayIcon->show(); // 显示托盘图标
    connect(trayIcon, &QSystemTrayIcon::activated, this, &Widget::onTrayIconActivated);//双击托盘图标时，恢复窗口
}

Widget::~Widget() {}

//计算、获取防休眠总时长
void Widget::onSpinBoxValueChanged(){
    unsigned int TakeTimeValue = spnInterval->value() * spnNumOfTimes->value();
    lblTakeTime->setText(QString::number(TakeTimeValue));
}

void Widget::OnPreventSleep(){
    btnOnOrOff->setText("停止");
    lblState->setText("防休眠中...");
    lblState->setStyleSheet("color: green;");

    spnInterval->setEnabled(false);
    spnNumOfTimes->setEnabled(false);
    chkIfPerpetual->setEnabled(false);
}

void Widget::OffPreventSleep(){
    btnOnOrOff->setText("开始");
    lblState->setText("防休眠结束");

    spnInterval->setEnabled(true);
    spnNumOfTimes->setEnabled(true);
    chkIfPerpetual->setEnabled(true);
}

void Widget::OnOrOff(){
    interval = spnInterval->value();
    numOfTimes = spnNumOfTimes->value();
    if (chkIfPerpetual->isChecked()) {
        // 复选框被选中
        ifPerpetual = true; // 可以赋值给 bool 类型变量
    } else {
        // 复选框未被选中
        ifPerpetual = false; // 可以赋值给 bool 类型变量
    }

    if (btnOnOrOff->text() == "开始") {
        OnPreventSleep();
        timerStart();
    } else if (btnOnOrOff->text() == "停止"){
        // 立即停止定时器
        if (timer) {
            timer->stop();
            timer->deleteLater();
            timer = nullptr; // 清空定时器指针
            OffPreventSleep();
            return;
        }
        numOfTimes=0;
    }
}

//启动定时器
void Widget::timerStart() {
    timer = new QTimer;//创建一个定时器对象
    timer->setInterval(interval*1000);//设置成每隔多少毫秒 发一次信号
    connect(timer, &QTimer::timeout, this, &Widget::timeOut);//定时器信号绑定槽函数，每隔毫秒，就会触发一次，然后执行槽函数
    timer->start();  // 启动定时器
}

void Widget::timeOut() {
    if(numOfTimes<=0){
        timer->stop();//停止计时
        timer->deleteLater();//删除操作推迟到事件循环的空闲时刻，从而避免在定时器事件处理过程中直接删除对象可能引起的问题。
        timer = nullptr; // 清空定时器指针
        OffPreventSleep();
        return;
    }

    simulate_key_press(VK_SCROLL);

    if(ifPerpetual==false){
        numOfTimes--;
    }
}

void Widget::toSystemTray(){
    hide();
}


void Widget::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::DoubleClick) {
        // 双击托盘图标恢复窗口
        this->showNormal();
    }
}


void Widget::closeEvent(QCloseEvent *event)
{
    // 保存当前的间隔值defaultNumOfTimes
    settings.setValue("interval", spnInterval->value());
    settings.setValue("numOfTimes", spnNumOfTimes->value());

    // 调用基类的 closeEvent 以确保正常关闭
    QWidget::closeEvent(event);
}

