#include "MainWindow.h"
#include "tools/OriDebug.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    //Ori::Debug::installMessageHandler();

    QApplication app(argc, argv);
    app.setApplicationName("Iris");
    app.setOrganizationName("orion-project.org");

    MainWindow w;
    w.show();

    return app.exec();
}
