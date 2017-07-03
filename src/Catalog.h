#ifndef CATALOG_H
#define CATALOG_H

#include <QObject>
#include <QList>

class FolderItem;
class GlassItem;
class Glass;

class CatalogItem
{
public:
    virtual ~CatalogItem();

    int id() const { return _id; }
    const QString& title() const { return _title; }
    CatalogItem* parent() const { return _parent; }
    const QList<CatalogItem*>& children() const { return _children; }

    bool isFolder() const;
    bool isGlass() const;
    FolderItem* asFolder();
    GlassItem* asGlass();

private:
    int _id;
    QString _title;
    CatalogItem* _parent = nullptr;
    QList<CatalogItem*> _children;

    friend class Catalog;
};

//------------------------------------------------------------------------------

class FolderItem : public CatalogItem
{
};

//------------------------------------------------------------------------------

class GlassItem : public CatalogItem
{
public:
    ~GlassItem();

    Glass* glass() const { return _glass; }

private:
    Glass* _glass = nullptr;

    friend class Catalog;
};

//------------------------------------------------------------------------------

class Catalog : public QObject
{
    Q_OBJECT

public:
    Catalog(QObject* parent);
    ~Catalog();

    const QList<CatalogItem*>& items() const { return _items; }

    QString renameFolder(FolderItem* item, const QString& title);
    QString createFolder(FolderItem* parent, const QString& title);
    QString removeFolder(FolderItem* item);
    QString createGlass(FolderItem* parent, Glass *glass);
    QString updateGlass(GlassItem* item, Glass* glass);
    QString removeGlass(GlassItem* item);
    QString loadGlass(GlassItem* item);

private:
    QList<CatalogItem*> _items;
};

#endif // CATALOG_H
