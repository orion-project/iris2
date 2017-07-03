#include "CatalogStore.h"
#include "SqlHelper.h"

#include <QDebug>

using namespace Ori::Sql;

// TODO check if strings from this file are put in language file

//------------------------------------------------------------------------------

class FoldersTableDef : public TableDef
{
public:
    FoldersTableDef() : TableDef("Folder") {}

    const QString id = "Id";
    const QString parent = "Parent";
    const QString title = "Title";
    const QString info = "Info";

    QString sqlCreate() const override {
        return "CREATE TABLE IF NOT EXISTS Folder (Id, Parent, Title, Info)";
    }
    const QString sqlInsert =
        "INSERT INTO Folder (Id, Parent, Title, Info) VALUES (:Id, :Parent, :Title, :Info)";
    const QString sqlSelectAll = "SELECT * FROM Folder";
    const QString sqlMaxId = "SELECT MAX(Id) FROM Folder";
};

//------------------------------------------------------------------------------

FoldersTableDef* foldersTable() { static FoldersTableDef t; return &t; }

//------------------------------------------------------------------------------

QString FoldersManager::create(FolderItem* folder) const
{
    auto table = foldersTable();

    SelectQuery queryId(table->sqlMaxId);
    if (queryId.isFailed() || !queryId.next())
        return qApp->tr("Unable to generate id for new folder.\n\n%1").arg(queryId.error());

    folder->_id = queryId.record().value(0).toInt() + 1;

    auto res = ActionQuery(table->sqlInsert)
                .param(table->id, folder->id())
                .param(table->parent, folder->parent() ? folder->parent()->id() : 0)
                .param(table->title, folder->title())
                .param(table->info, folder->info())
                .exec();
    if (!res.isEmpty())
        return qApp->tr("Failed to create new folder.\n\n%1").arg(res);

    return QString();
}

QString FoldersManager::selectAll(QList<CatalogItem*>& roots) const
{
    auto table = foldersTable();
    SelectQuery query(table->sqlSelectAll);
    if (query.isFailed())
        return qApp->tr("Unable to load folder list.\n\n%1").arg(query.error());

    QMap<int, FolderItem*> items;

    while (query.next())
    {
        auto r = query.record();
        int id = r.value(table->id).toInt();
        if (!items.contains(id))
            items.insert(id, new FolderItem);
        auto item = items[id];
        item->_id = id;
        item->_title = r.value(table->title).toString();
        item->_info = r.value(table->info).toString();
        int parentId = r.value(table->parent).toInt();
        if (parentId > 0)
        {
            if (!items.contains(parentId))
                items.insert(parentId, new FolderItem);
            auto parentItem = items[parentId];
            parentItem->_children.append(item);
            item->_parent = parentItem;
        }
    }

    for (auto item: items.values())
        if (!item->parent())
            roots.append(item);

    return QString();
}

//------------------------------------------------------------------------------

namespace CatalogStore {

QSqlDatabase __db;

QString createTable(TableDef *table);

void closeDatabase()
{
    if (!__db.isOpen()) return;
    QString connection = __db.connectionName();
    __db.close();
    __db = QSqlDatabase();
    QSqlDatabase::removeDatabase(connection);
}

QString newDatabase(const QString fileName)
{
    closeDatabase();

    if (QFile::exists(fileName))
        if (!QFile::remove(fileName))
            return qApp->tr("Unable to overwrite existing file. Probably file is locked.");

    QString res = openDatabase(fileName);
    if (!res.isEmpty())
        return res;

    return QString();
}

QString openDatabase(const QString fileName)
{
    closeDatabase();

    __db = QSqlDatabase::addDatabase("QSQLITE");
    __db.setDatabaseName(fileName);
    if (!__db.open())
        return qApp->tr("Unable to establish a database connection.\n\n%1")
                .arg(SqlHelper::errorText(__db.lastError()));

    QSqlQuery query;
    if (!query.exec("PRAGMA foreign_keys = ON;"))
        return qApp->tr("Failed to enable foreign keys.\n\n%1").arg(SqlHelper::errorText(query));

    QString res;

    if (!__db.transaction())
        return qApp->tr("Unable to begin transaction to create database structure.\n\n%1")
                .arg(SqlHelper::errorText(__db.lastError()));

    res = createTable(foldersTable());
    if (!res.isEmpty()) return res;

    __db.commit();

    return QString();
}

QString createTable(TableDef *table)
{
    auto res = Ori::Sql::ActionQuery(table->sqlCreate()).exec();
    if (!res.isEmpty())
    {
        __db.rollback();
        return qApp->tr("Unable to create table '%1'.\n\n%2").arg(table->tableName()).arg(res);
    }
    return QString();
}

FoldersManager *foldersManager() { static FoldersManager m; return &m; }

} // namespace CatalogStore
