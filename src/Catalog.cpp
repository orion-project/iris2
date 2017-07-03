#include "Catalog.h"
#include "CatalogStore.h"
#include "Glass.h"

#include <QDebug>

Glass* ShottFormula::makeGlass() { return new GlassShott(this); }
Glass* SellmeierFormula::makeGlass() { return new GlassSellmeier(this); }
Glass* ReznikFormula::makeGlass() { return new GlassReznik(this); }
Glass* CustomFormula::makeGlass() { return new GlassCustom(this); }

//------------------------------------------------------------------------------

CatalogItem::~CatalogItem()
{
    qDeleteAll(_children);
}

bool CatalogItem::isFolder() const { return dynamic_cast<const FolderItem*>(this); }
bool CatalogItem::isGlass() const { return dynamic_cast<const GlassItem*>(this); }
FolderItem* CatalogItem::asFolder() { return dynamic_cast<FolderItem*>(this); }
GlassItem* CatalogItem::asGlass() { return dynamic_cast<GlassItem*>(this); }

//------------------------------------------------------------------------------

GlassItem::~GlassItem()
{
    if (_glass) delete _glass;
}

//------------------------------------------------------------------------------

QString Catalog::fileFilter()
{
    return tr("Iris Catalog Files (*.iris);;All files (*.*)");
}

CatalorResult Catalog::open(const QString& fileName)
{
    QString res = CatalogStore::openDatabase(fileName);
    if (!res.isEmpty())
        return CatalorResult::fail(res);

    Catalog* catalog = new Catalog;
    catalog->_fileName = fileName;

    res = CatalogStore::foldersManager()->selectAll(catalog->_items);
    if (!res.isEmpty())
    {
        delete catalog;
        return CatalorResult::fail(res);
    }

    return CatalorResult::ok(catalog);
}

CatalorResult Catalog::create(const QString& fileName)
{
    QString res = CatalogStore::newDatabase(fileName);
    if (!res.isEmpty())
        return CatalorResult::fail(res);

    Catalog* catalog = new Catalog;
    catalog->_fileName = fileName;

    return CatalorResult::ok(catalog);
}

Catalog::Catalog() : QObject()
{
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

    auto res = CatalogStore::foldersManager()->create(folder);
    if (!res.isEmpty())
    {
        delete folder;
        return res;
    }

    (parent ? parent->_children : _items).append(folder);
    // TODO sort items after inserting
    return QString();
}

QString Catalog::removeFolder(FolderItem* item)
{
    (item->parent() ? item->parent()->_children : _items).removeOne(item);
    delete item;
    // TODO remove from database, return error
    return QString();
}

QString Catalog::createGlass(FolderItem* parent, Glass *glass)
{
    // TODO make item id
    auto item = new GlassItem;
    item->_glass = glass;
    item->_parent = parent;
    item->_formula = glass->formula();
    item->_title = glass->title();
    // TODO update item info
    // TODO update item icon
    // TODO save to database, add to chindren only if no error
    (parent ? parent->_children : _items).append(item);
    // TODO sort items after inserting
    return QString();
}

QString Catalog::updateGlass(GlassItem* item, Glass *glass)
{
    // TODO save to database, update objects in internal lists only if no error
    // TODO copy glass props instead of assigning pointer
    item->_glass = glass;
    item->_formula = glass->formula();
    item->_title = glass->title();
    // TODO update item info
    // TODO update item icon
    // TODO sort items after renaming
    return QString();
}

QString Catalog::removeGlass(GlassItem* item)
{
    (item->parent() ? item->parent()->_children : _items).removeOne(item);
    delete item;
    // TODO remove from database, return error
    return QString();
}

QString Catalog::loadGlass(GlassItem* item)
{
    //item->_glass = new Glass;
    // TODO load glass from database
    return QString();
}

IntResult Catalog::countGlasses() const
{
    // TODO count glasses
    return IntResult::ok(42);
}
