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
        if (!parent.isValid())
        {
            if (row < _catalog->items().size())
                return createIndex(row, column, _catalog->items().at(row));
            return QModelIndex();
        }

        auto parentItem = catalogItem(parent);
        if (!parentItem) return QModelIndex();

        if (row < parentItem->children().size())
            return createIndex(row, column, parentItem->children().at(row));

        return QModelIndex();
    }

    QModelIndex parent(const QModelIndex &child) const override
    {
        if (!child.isValid()) return QModelIndex();

        auto childItem = catalogItem(child);
        if (!childItem) return QModelIndex();

        auto parentItem = childItem->parent();
        if (!parentItem) return QModelIndex();

        int row = parentItem->parent()
                ? parentItem->parent()->children().indexOf(parentItem)
                : _catalog->items().indexOf(parentItem);

        return createIndex(row, 0, parentItem);
    }

    int rowCount(const QModelIndex &parent) const override
    {
        if (!parent.isValid())
            return _catalog->items().size();

        auto item = catalogItem(parent);
        return item ? item->children().size() : 0;
    }

    int columnCount(const QModelIndex &parent) const override
    {
        Q_UNUSED(parent)
        return 1;
    }

    bool hasChildren(const QModelIndex &parent) const override
    {
        if (!parent.isValid())
            return !_catalog->items().isEmpty();

        auto item = catalogItem(parent);
        return item ? !item->children().isEmpty() : false;
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
