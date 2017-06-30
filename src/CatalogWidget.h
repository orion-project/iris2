#ifndef CATALOGWIDGET_H
#define CATALOGWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QTreeView;
QT_END_NAMESPACE

class Catalog;
class CatalogItem;
class CatalogModel;
class FolderItem;
class GlassItem;

class CatalogWidget : public QWidget
{
    Q_OBJECT

public:
    CatalogWidget();

    void setCatalog(Catalog* catalog);

private:
    Catalog* _catalog;
    QTreeView* _catalogView;
    CatalogModel* _catalogModel = nullptr;
    QMenu *_folderMenu, *_glassMenu;
    QAction *_folderMenuHeader, *_glassMenuHeader;

    static QAction *makeHeaderItem(QMenu* menu);

    void contextMenuRequested(const QPoint &pos);

    void createFolder();
    void renameFolder();
    void deleteFolder();

    CatalogItem* selectedItem() const;
    FolderItem* selectedFolder() const;
    GlassItem* selectedGlass() const;
};

#endif // CATALOGWIDGET_H
