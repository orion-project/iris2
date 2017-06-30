#include "CatalogWidget.h"
#include "Catalog.h"
#include "helpers/OriLayouts.h"

#include <QAbstractItemModel>
#include <QContextMenuEvent>
#include <QMenu>
#include <QTreeView>

class CatalogModel : public QAbstractItemModel
{
public:
    CatalogModel(Catalog* catalog) : _catalog(catalog) {}

    CatalogItem* catalogItem(const QModelIndex &index)
    {
        return static_cast<CatalogItem*>(index.internalPointer());
    }

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override
    {
        auto parentItem = catalogItem(parent);
        auto childItem = parentItem
                ? parentItem->children().at(row)
                : _catalog->items().at(row);
        return createIndex(row, column, childItem);
    }

    QModelIndex parent(const QModelIndex &child) const override
    {
//        if (child.isValid())
//        {
//            auto childItem = catalogItem(child);
//            Node *parentNode = parent(childNode);
//            if (parentNode)
//                return createIndex(row(parentNode), 0, parentNode);
//        }
//        return QModelIndex();
    }

private:
    Catalog* _catalog;
};


CatalogWidget::CatalogWidget() : QWidget()
{

    //setContextMenuPolicy(Qt::CustomContextMenu);

    _folderMenu = new QMenu(this);
    _glassMenu = new QMenu(this);

    _folderMenu->addAction(tr("New Folder"));

    _glassMenu->addAction(tr("Open Plot"));

    _catalogView = new QTreeView;
    _catalogView->setHeaderHidden(true);

    Ori::Layouts::LayoutV({_catalogView}).useFor(this);
}

void CatalogWidget::setCatalog(Catalog* catalog)
{
    if (_catalogModel) delete _catalogModel;
    _catalogModel = new CatalogModel;
    _catalogView->setModel(_catalogModel);

//    for (CatalogItem* item : catalog->items())
//    {
//        auto itemView = new QTreeWidgetItem(this);
//        itemView->setText(0, item->title());
//        populateBranch(itemView, item);
//        addTopLevelItem(itemView);
//    }
}

//void CatalogWidget::populateBranch(QTreeWidgetItem* rootView, CatalogItem* rootItem)
//{
////    for (CatalogItem* item : rootItem->children())
////    {
////        auto itemView = new QTreeWidgetItem(rootView);
////        itemView->setText(0, item->title());
////        populateBranch(itemView, item);
////        rootView->addChild(itemView);
////    }
//}

void CatalogWidget::contextMenuEvent(QContextMenuEvent *event)
{
//    auto selection = selectedItems();
//    if (selection.isEmpty()) return;

//    auto menu = selection.first()->p ? _folderMenu

//    auto item = ;
//    if ()
//        _folderMenu->popup(event->pos());
//    else
//        _glassMenu->popup(event->pos());
}
