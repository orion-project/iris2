#ifndef CATALOGSTORE_H
#define CATALOGSTORE_H

#include <QVector>

class CatalogItem;
class GlassItem;
class FolderItem;
class FolderTableDef;
class GlassTableDef;

//------------------------------------------------------------------------------

class FolderManager
{
public:
    FolderTableDef* table() const;

    QString create(FolderItem* folder) const;
    QString selectAll(QList<CatalogItem*>& roots) const;
};

//------------------------------------------------------------------------------

class GlassManager
{
public:
    GlassTableDef* table() const;

    QString create(GlassItem* item) const;
};

//------------------------------------------------------------------------------

namespace CatalogStore {

QString newDatabase(const QString fileName);
QString openDatabase(const QString fileName);

FolderManager* folderManager();
GlassManager* glassManager();

} // namespace CatalogStore

#endif // CATALOGSTORE_H
