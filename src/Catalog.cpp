#include "Catalog.h"

#include <QDebug>

CatalogItem::~CatalogItem()
{
    for (auto item: _children) delete item;
}

bool CatalogItem::isFolder() const { return dynamic_cast<FolderItem*>(this); }
bool CatalogItem::isGlass() const { return dynamic_cast<GlassItem*>(this); }


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
            item->_children.append(subitem);
        }

        _items.append(item);
    }
}


QString GlassItem::getInfo() const
{
    return "Lorem ipsum";
}


Catalog::~Catalog()
{
    for (auto item: _items) delete item;
}
