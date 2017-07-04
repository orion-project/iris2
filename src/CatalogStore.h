#ifndef CATALOGSTORE_H
#define CATALOGSTORE_H

#include <QVector>

class CatalogItem;
class GlassItem;
class FolderItem;
class FolderTableDef;
class GlassTableDef;

//------------------------------------------------------------------------------

struct FoldersResult
{
    QString error;
    QMap<int, FolderItem*> items;
};

class FolderManager
{
public:
    FolderTableDef* table() const;

    QString create(FolderItem* folder) const;
    FoldersResult selectAll() const;
};

//------------------------------------------------------------------------------

struct GlassesResult
{
    QString error;
    QStringList warnings;
    QMap<int, QList<GlassItem*>> items;
};

class GlassManager
{
public:
    GlassTableDef* table() const;

    QString create(GlassItem* item) const;
    GlassesResult selectAll() const;
};

//------------------------------------------------------------------------------

namespace CatalogStore {

QString newDatabase(const QString fileName);
QString openDatabase(const QString fileName);

FolderManager* folderManager();
GlassManager* glassManager();

} // namespace CatalogStore

#endif // CATALOGSTORE_H
