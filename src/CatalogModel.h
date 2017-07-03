#ifndef CATALOGMODEL_H
#define CATALOGMODEL_H

#include <QAbstractItemModel>
#include <QDebug>
#include <QIcon>

#include "Catalog.h"

class CatalogModel : public QAbstractItemModel
{
public:
    CatalogModel(Catalog* catalog) : _catalog(catalog)
    {
        _iconGlass = QIcon(":/icon/glass_blue");
        _iconFolder = QIcon(":/icon/folder_closed");
    }

    static CatalogItem* catalogItem(const QModelIndex &index)
    {
        return static_cast<CatalogItem*>(index.internalPointer());
    }

    QModelIndex index(int row, int column, const QModelIndex &parent) const override
    {
        auto parentItem = catalogItem(parent);
        CatalogItem *childItem = nullptr;
        if (parentItem && row < parentItem->children().size())
            childItem = parentItem->children().at(row);
        else if (row < _catalog->items().size())
            childItem = _catalog->items().at(row);
        return createIndex(row, column, childItem);
    }

    QModelIndex parent(const QModelIndex &child) const override
    {
        if (!child.isValid()) return QModelIndex();

        auto childItem = catalogItem(child);
        auto parentItem = childItem ? childItem->parent() : nullptr;
        if (!parentItem) return QModelIndex();

        int row = parentItem->parent()
                ? parentItem->children().indexOf(parentItem)
                : _catalog->items().indexOf(parentItem);

        return createIndex(row, 0, parentItem);
    }

    int rowCount(const QModelIndex &parent) const override
    {
        auto item = catalogItem(parent);
        return item ? item->children().size() : _catalog->items().size();
    }

    int columnCount(const QModelIndex &parent) const override
    {
        Q_UNUSED(parent)
        return 1;
    }

    bool hasChildren(const QModelIndex &parent) const override
    {
        auto item = catalogItem(parent);
        return !(item ? item->children().isEmpty() : _catalog->items().isEmpty());
    }

    QVariant data(const QModelIndex &index, int role) const override
    {
        if (!index.isValid())
            return QVariant();
        if (role == Qt::DisplayRole)
        {
            auto item = catalogItem(index);
            return item ? item->title() : QVariant();
        }
        if (role == Qt::DecorationRole)
        {
            auto item = catalogItem(index);
            if (!item) return QVariant();
            // TODO different icons for opened and closed folder
            if (item->isFolder())
                return _iconFolder;
            if (item->asGlass()->formula())
                return item->asGlass()->formula()->icon();
            return _iconGlass;
        }
        return QVariant();
    }

    void itemRenamed(const QModelIndex &index)
    {
        emit dataChanged(index, index);
    }

    QModelIndex itemAdded(const QModelIndex &parent)
    {
        int row = rowCount(parent) - 1;
        beginInsertRows(parent, row, row);
        endInsertRows();
        return index(row, 0, parent);
    }

    QModelIndex itemRemoved(const QModelIndex &index)
    {
        auto parentIndex = parent(index);
        beginRemoveRows(parentIndex, index.row(), index.row());
        endRemoveRows();
        return parentIndex;
    }

private:
    Catalog* _catalog;
    QIcon _iconFolder, _iconGlass;
};

#endif // CATALOGMODEL_H
