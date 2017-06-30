#include "MainWindow.h"
#include "CatalogWidget.h"
#include "InfoWidget.h"
#include "helpers/OriWindows.h"

#include <QDockWidget>
#include <QMdiArea>

MainWindow::MainWindow() : QMainWindow()
{
    Ori::Wnd::setWindowIcon(this, ":/windows/main"); // TODO make icon

    _catalogView = new CatalogWidget;
    _infoView = new InfoWidget;

    createDocks();

    _mdiArea = new QMdiArea;
    setCentralWidget(_mdiArea);

    loadSettings();
}

MainWindow::~MainWindow()
{
    saveSettings();
}

void MainWindow::createDocks()
{
    auto dockCatalog = new QDockWidget(tr("Catalog"));
    dockCatalog->setWidget(_catalogView);

    auto dockInfo = new QDockWidget(tr("Info"));
    dockInfo->setWidget(_infoView);

    addDockWidget(Qt::LeftDockWidgetArea, dockCatalog);
    addDockWidget(Qt::LeftDockWidgetArea, dockInfo);
}

void MainWindow::saveSettings()
{
    // TODO save dock configuration
    // TODO save window geometry
}

void MainWindow::loadSettings()
{
    // TODO load dock configuration
    // TODO load window geometry
}

