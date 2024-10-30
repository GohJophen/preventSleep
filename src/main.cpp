#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    //设置任务栏图标、标题栏图标以及其他子窗口的图标
    QIcon appIcon(":/icons/Pictogrammers-Material-Sleep-off.ico");
    a.setWindowIcon(appIcon);

    w.setFixedSize(w.sizeHint());//维持窗口默认大小，不可拉伸
    w.setWindowTitle("preventSleep");
    w.setWindowFlags(w.windowFlags() & ~Qt::WindowMaximizeButtonHint);//使用最大化按钮变灰色，不可点击

    w.show();
    return a.exec();
}
