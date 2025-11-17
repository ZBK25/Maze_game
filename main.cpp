#include "widget.h"

#include <QApplication>
#include <QWidget>

#include "my_widget.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

int main(int argc, char *argv[])
{
    #ifdef Q_OS_WIN
        // 重定向到控制台
        if (AttachConsole(ATTACH_PARENT_PROCESS) || AllocConsole()) {
            freopen("CONOUT$", "w", stdout);
            freopen("CONOUT$", "w", stderr);
        }
    #endif

    QApplication a(argc, argv);

    //初始化线程池
    QThreadPool::globalInstance()->setMaxThreadCount(4);

    MainUI window;
    window.show();
    
    return a.exec();
}
