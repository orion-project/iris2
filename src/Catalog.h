#ifndef CATALOG_H
#define CATALOG_H

#include "Glass.h"

#include <QObject>
#include <QList>

class FolderItem;
class GlassItem;

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


class FolderItem : public CatalogItem
{
};


class GlassItem : public CatalogItem
{
public:
    QString getInfo() const;
};


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
    QString createGlass(FolderItem* parent, const QString& title);
    QString updateGlass(GlassItem* item, const QString &title);
    QString removeGlass(GlassItem* item);

private:
    QList<CatalogItem*> _items;
};

#endif // CATALOG_H
