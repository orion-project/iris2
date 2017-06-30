#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QMdiArea;
QT_END_NAMESPACE

class CatalogWidget;
class InfoWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private:
    CatalogWidget* _catalogView;
    InfoWidget* _infoView;
    QMdiArea* _mdiArea;

    void createDocks();
    void saveSettings();
    void loadSettings();
};

#endif // MAINWINDOW_H
