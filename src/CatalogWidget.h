#ifndef CATALOGWIDGET_H
#define CATALOGWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QMenu;
class QTreeView;
QT_END_NAMESPACE

class Catalog;
class CatalogItem;
class CatalogModel;

class CatalogWidget : public QWidget
{
    Q_OBJECT

public:
    CatalogWidget();

    void setCatalog(Catalog* catalog);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    //void populateBranch(QTreeWidgetItem* rootView, CatalogItem* rootItem);

    QTreeView* _catalogView;
    CatalogModel* _catalogModel = nullptr;
    QMenu *_folderMenu, *_glassMenu;
};

#endif // CATALOGWIDGET_H
