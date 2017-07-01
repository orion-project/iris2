#include "Catalog.h"

#include <QDebug>

CatalogItem::~CatalogItem()
{
    qDeleteAll(_children);
}

bool CatalogItem::isFolder() const { return dynamic_cast<const FolderItem*>(this); }
bool CatalogItem::isGlass() const { return dynamic_cast<const GlassItem*>(this); }
FolderItem* CatalogItem::asFolder() { return dynamic_cast<FolderItem*>(this); }
GlassItem* CatalogItem::asGlass() { return dynamic_cast<GlassItem*>(this); }


Catalog::Catalog(QObject* parent) : QObject(parent)
{
    for (int i = 0; i < 10; i++)
    {
        auto item = new FolderItem;
        item->_id = i;
        item->_title = QString("Item %1").arg(i);

        for (int j = 0; j < 10; j++)
        {
            auto subitem = new GlassItem;
            subitem->_id = j*i;
            subitem->_title = QString("Subitem %1").arg(j*i);
            subitem->_parent = item;
            item->_children.append(subitem);
        }

        _items.append(item);
    }
}


QString GlassItem::getInfo() const
{
    // TODO make glass description
    return "Lorem ipsum";
}


Catalog::~Catalog()
{
    qDeleteAll(_items);
}

QString Catalog::renameFolder(FolderItem* item, const QString& title)
{
    item->_title = title;
    // TODO sort items after renaming
    // TODO save to database, return error
    return QString();
}

QString Catalog::createFolder(FolderItem* parent, const QString& title)
{
    FolderItem* folder = new FolderItem;
    folder->_title = title;
    folder->_parent = parent;
    (parent ? parent->_children : _items).append(folder);
    // TODO sort items after inserting
    // TODO save to database, return error
    return QString();
}

QString Catalog::removeFolder(FolderItem* item)
{
    (item->parent() ? item->parent()->_children : _items).removeOne(item);
    delete item;
    // TODO remove from database, return error
    return QString();
}

QString Catalog::createGlass(FolderItem* parent, const QString& title)
{
    GlassItem* glass = new GlassItem;
    glass->_title = title;
    glass->_parent = parent;
    (parent ? parent->_children : _items).append(glass);
    // TODO sort items after inserting
    // TODO save to database, return error
    return QString();
}

QString Catalog::updateGlass(GlassItem* item, const QString& title)
{
    item->_title = title;
    // TODO sort items after renaming
    // TODO save to database, return error
    return QString("err");
}

QString Catalog::removeGlass(GlassItem* item)
{
    (item->parent() ? item->parent()->_children : _items).removeOne(item);
    delete item;
    // TODO remove from database, return error
    return QString();
}
