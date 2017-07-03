#ifndef CATALOGSTORE_H
#define CATALOGSTORE_H

#include "Catalog.h"

#include <QVector>

//------------------------------------------------------------------------------

class FoldersManager
{
public:
    QString create(FolderItem* folder) const;
    QString selectAll(QList<CatalogItem*>& roots) const;
};

//------------------------------------------------------------------------------

namespace CatalogStore {

QString newDatabase(const QString fileName);
QString openDatabase(const QString fileName);

FoldersManager* foldersManager();

} // namespace CatalogStore

#endif // CATALOGSTORE_H
