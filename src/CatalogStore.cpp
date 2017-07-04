#include "Catalog.h"
#include "CatalogStore.h"
#include "Glass.h"
#include "SqlHelper.h"

#include <QDebug>

using namespace Ori::Sql;

// TODO check if strings from this file are put in language file

//------------------------------------------------------------------------------

class FolderTableDef : public TableDef
{
public:
    FolderTableDef() : TableDef("Folder") {}

    const QString id = "Id";
    const QString parent = "Parent";
    const QString title = "Title";
    const QString info = "Info";

    QString sqlCreate() const override {
        return "CREATE TABLE IF NOT EXISTS Folder (Id, Parent, Title, Info)";
    }

    const QString sqlInsert =
        "INSERT INTO Folder (Id, Parent, Title, Info) "
        "VALUES (:Id, :Parent, :Title, :Info)";
};

//------------------------------------------------------------------------------

class GlassTableDef : public TableDef
{
public:
    GlassTableDef() : TableDef("Glass") {}

    const QString id = "Id";
    const QString parent = "Parent";
    const QString title = "Title";
    const QString info = "Info";
    const QString comment = "Comment";
    const QString formula = "Formula";
    const QString lambdaMin = "LambdaMin";
    const QString lambdaMax = "LambdaMax";

    QString sqlCreate() const override {
        return "CREATE TABLE IF NOT EXISTS Glass "
               "(Id, Parent, Title, Info, Comment, Formula, LambdaMin, LambdaMax)";
    }

    const QString sqlInsert =
        "INSERT INTO Glass (Id, Parent, Title, Info, Comment, Formula, LambdaMin, LambdaMax) "
        "VALUES (:Id, :Parent, :Title, :Info, :Comment, :Formula, :LambdaMin, :LambdaMax)";
};

//------------------------------------------------------------------------------

FolderTableDef* FolderManager::table() const { static FolderTableDef t; return &t; }

QString FolderManager::create(FolderItem* folder) const
{
    SelectQuery queryId(table()->sqlSelectMaxId());
    if (queryId.isFailed() || !queryId.next())
        return qApp->tr("Unable to generate id for new folder.\n\n%1").arg(queryId.error());

    folder->_id = queryId.record().value(0).toInt() + 1;

    auto res = ActionQuery(table()->sqlInsert)
                .param(table()->id, folder->id())
                .param(table()->parent, folder->parent() ? folder->parent()->asFolder()->id() : 0)
                .param(table()->title, folder->title())
                .param(table()->info, folder->info())
                .exec();
    if (!res.isEmpty())
        return qApp->tr("Failed to create new folder.\n\n%1").arg(res);

    return QString();
}

FoldersResult FolderManager::selectAll() const
{
    FoldersResult result;

    SelectQuery query(table()->sqlSelectAll());
    if (query.isFailed())
    {
        result.error = qApp->tr("Unable to load folder list.\n\n%1").arg(query.error());
        return result;
    }

    while (query.next())
    {
        auto r = query.record();
        int id = r.value(table()->id).toInt();
        if (!result.items.contains(id))
            result.items.insert(id, new FolderItem);
        auto item = result.items[id];
        item->_id = id;
        item->_title = r.value(table()->title).toString();
        item->_info = r.value(table()->info).toString();
        int parentId = r.value(table()->parent).toInt();
        if (parentId > 0)
        {
            if (!result.items.contains(parentId))
                result.items.insert(parentId, new FolderItem);
            auto parentItem = result.items[parentId];
            parentItem->_children.append(item);
            item->_parent = parentItem;
        }
    }

    return result;
}

//------------------------------------------------------------------------------

GlassTableDef* GlassManager::table() const { static GlassTableDef t; return &t; }

QString GlassManager::create(GlassItem* item) const
{
    SelectQuery queryId(table()->sqlSelectMaxId());
    if (queryId.isFailed() || !queryId.next())
        return qApp->tr("Unable to generate id for new material.\n\n%1").arg(queryId.error());

    if (!item->glass() || !item->formula())
        return "Bad method usage: poorly defined material: no glass or formula is defined";

    item->glass()->_id = queryId.record().value(0).toInt() + 1;

    auto res = ActionQuery(table()->sqlInsert)
            .param(table()->parent, item->parent() ? item->parent()->asFolder()->id() : 0)
            .param(table()->id, item->glass()->id())
            .param(table()->title, item->glass()->title())
            .param(table()->info, item->info())
            .param(table()->comment, item->glass()->comment())
            .param(table()->lambdaMin, item->glass()->lambdaMin())
            .param(table()->lambdaMax, item->glass()->lambdaMax())
            .param(table()->formula, item->glass()->formula()->name())
            .exec();
    if (!res.isEmpty())
        return qApp->tr("Failed to create new material.\n\n%1").arg(res);

    return QString();
}

GlassesResult GlassManager::selectAll() const
{
    GlassesResult result;

    SelectQuery query(table()->sqlSelectAll());
    if (query.isFailed())
    {
        result.error = qApp->tr("Unable to load materials.\n\n%1").arg(query.error());
        return result;
    }

    while (query.next())
    {
        auto r = query.record();

        int id = r.value(table()->id).toInt();
        QString title = r.value(table()->title).toString();
        QString formulaName = r.value(table()->formula).toString();
        if (!dispersionFormulas().contains(formulaName))
        {
            result.warnings.append(qApp->tr("Skip material #%1 '%2': "
                                            "unknown dispersion formula '%3'.")
                                   .arg(id).arg(title).arg(formulaName));
            continue;
        }

        Glass* glass = dispersionFormulas()[formulaName]->makeGlass();
        glass->_id = id;
        glass->_title = title;
        glass->_comment = r.value(table()->comment).toString();
        glass->_lambdaMin = r.value(table()->lambdaMin).toDouble();
        glass->_lambdaMax = r.value(table()->lambdaMax).toDouble();

        GlassItem *item = new GlassItem;
        item->_glass = glass;
        item->_formula = glass->formula();
        item->_title = glass->title();
        item->_info = r.value(table()->info).toString();

        int parentId = r.value(table()->parent).toInt();
        if (!result.items.contains(parentId))
            result.items.insert(parentId, QList<GlassItem*>());
        ((QList<GlassItem*>&)result.items[parentId]).append(item);

//        for (auto item: result.items[parentId])
//            qDebug() << parentId << item->glass()->id() << item->glass()->title();
    }

    return result;
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

    if (!__db.transaction())
        return qApp->tr("Unable to begin transaction to create database structure.\n\n%1")
                .arg(SqlHelper::errorText(__db.lastError()));

    QString res = createTable(folderManager()->table());
    if (!res.isEmpty()) return res;

    res = createTable(glassManager()->table());
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

FolderManager *folderManager() { static FolderManager m; return &m; }
GlassManager* glassManager() { static GlassManager m; return &m; }

} // namespace CatalogStore
