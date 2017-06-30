#ifndef CATALOG_H
#define CATALOG_H

#include "Glass.h"

#include <QObject>
#include <QList>

class CatalogItem
{
public:
    virtual ~CatalogItem();

    int id() const { return _id; }
    const QString& title() const { return _title; }

    const QList<CatalogItem*>& children() const { return _children; }

    bool isFolder() const;
    bool isGlass() const;

private:
    int _id;
    QString _title;

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

private:
    QList<CatalogItem*> _items;
};

#endif // CATALOG_H
