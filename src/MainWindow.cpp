#include "MainWindow.h"
#include "Catalog.h"
#include "CatalogWidget.h"
#include "InfoWidget.h"
#include "helpers/OriDialogs.h"
#include "helpers/OriWindows.h"
#include "tools/OriMruList.h"
#include "tools/OriWaitCursor.h"
#include "widgets/OriMruMenu.h"

#include <QDockWidget>
#include <QFileDialog>
#include <QFileInfo>
#include <QLabel>
#include <QMdiArea>
#include <QMenuBar>
#include <QStatusBar>

MainWindow::MainWindow() : QMainWindow()
{
    Ori::Wnd::setWindowIcon(this, ":/icon/main");

    _catalogView = new CatalogWidget;
    _infoView = new InfoWidget;

    _mruList = new Ori::MruFileList(this);
    connect(_mruList, &Ori::MruFileList::clicked, this, &MainWindow::openCatalog);

    createMenu();
    createDocks();
    createStatusBar();

    _mdiArea = new QMdiArea;
    setCentralWidget(_mdiArea);

    loadSettings();
}

MainWindow::~MainWindow()
{
    if (_catalog) delete _catalog;

    saveSettings();
}

void MainWindow::createMenu()
{
    QMenu* menuFile = menuBar()->addMenu(tr("&File"));
    menuFile->addAction(tr("New..."), this, &MainWindow::newCatalog);
    menuFile->addAction(tr("Open..."), this, &MainWindow::openCatalogViaDialog, QKeySequence::Open);
    menuFile->addSeparator();
    auto actionExit = menuFile->addAction(tr("Exit"), this, &MainWindow::close, QKeySequence::Quit);
    new Ori::Widgets::MruMenuPart(_mruList, menuFile, actionExit, this);
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

void MainWindow::createStatusBar()
{
    statusBar()->addWidget(_statusGlassCount = new QLabel);
    statusBar()->addWidget(_statusFileName = new QLabel);
    statusBar()->showMessage(tr("Ready"));

    _statusGlassCount->setMargin(2);
    _statusFileName->setMargin(2);
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

void MainWindow::newCatalog()
{
    QString fileName = QFileDialog::getSaveFileName(
                this, tr("Create Catalog"), QString(), Catalog::fileFilter());
    if (fileName.isEmpty()) return;

    Ori::WaitCursor c;

    catalogClosed();

    auto res = Catalog::create(fileName);
    if (res.ok())
    {
        catalogOpened(res.result());
        statusBar()->showMessage(tr("Catalog created"), 2000);
    }
    else Ori::Dlg::error(tr("Unable to create catalog.\n\n%1").arg(res.error()));
}

void MainWindow::openCatalog(const QString &fileName)
{
    Ori::WaitCursor c;

    catalogClosed();

    auto res = Catalog::open(fileName);
    if (res.ok())
    {
        catalogOpened(res.result());
        statusBar()->showMessage(tr("Catalog loaded"), 2000);
    }
    else Ori::Dlg::error(tr("Unable to load catalog.\n\n%1").arg(res.error()));
}

void MainWindow::openCatalogViaDialog()
{
    QString fileName = QFileDialog::getOpenFileName(
                this, tr("Open Catalog"), QString(), Catalog::fileFilter());
    if (!fileName.isEmpty())
        openCatalog(fileName);
}

void MainWindow::catalogOpened(Catalog* catalog)
{
    _catalog = catalog;
    _catalogView->setCatalog(_catalog);
    auto filePath = _catalog->fileName();
    auto fileName = QFileInfo(filePath).fileName();
    setWindowTitle(fileName % " - " % qApp->applicationName());
    _mruList->append(filePath);
    _statusFileName->setText(tr("Catalog: %1").arg(QDir::toNativeSeparators(filePath)));
    updateCounter();
}

void MainWindow::catalogClosed()
{
    delete _catalog;
    _catalog = nullptr;
    _catalogView->setCatalog(nullptr);
    setWindowTitle(qApp->applicationName());
    _statusFileName->clear();
    _statusGlassCount->clear();
}

void MainWindow::updateCounter()
{
    auto res = _catalog->countGlasses();
    if (res.ok())
    {
        _statusGlassCount->setToolTip(QString());
        _statusGlassCount->setText(tr("Materials: %1").arg(res.result()));
    }
    else
    {
        _statusGlassCount->setToolTip(res.error());
        _statusGlassCount->setText(tr("Materials: ERROR"));
    }
}
