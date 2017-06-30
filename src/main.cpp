#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Iris");
    app.setOrganizationName("orion-project.org");

    MainWindow w;
    w.show();

    return app.exec();
}
