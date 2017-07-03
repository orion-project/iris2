#ifndef CATALOG_H
#define CATALOG_H

#include <QObject>
#include <QList>
#include <QIcon>

class FolderItem;
class GlassItem;
class Glass;

//------------------------------------------------------------------------------

class DispersionFormula
{
public:
    virtual const char* name() const = 0;
    virtual const QIcon& icon() const = 0;
    virtual Glass* makeGlass() = 0;
};

class ShottFormula : public DispersionFormula
{
public:
    const char* name() const { return QT_TRANSLATE_NOOP("Formula", "Shott"); }
    const QIcon& icon() const { static QIcon icon(":/icon/glass_green"); return icon; }
    Glass* makeGlass();
};

class SellmeierFormula : public DispersionFormula
{
public:
    const char* name() const { return QT_TRANSLATE_NOOP("Formula", "Sellmeier"); }
    const QIcon& icon() const { static QIcon icon(":/icon/glass_red"); return icon; }
    Glass* makeGlass();
};

class ReznikFormula : public DispersionFormula
{
public:
    const char* name() const { return QT_TRANSLATE_NOOP("Formula", "Reznik"); }
    const QIcon& icon() const { static QIcon icon(":/icon/glass_violet"); return icon; }
    Glass* makeGlass();
};

class CustomFormula : public DispersionFormula
{
public:
    const char* name() const { return QT_TRANSLATE_NOOP("Formula", "Custom"); }
    const QIcon& icon() const { static QIcon icon(":/icon/glass_blue"); return icon; }
    Glass* makeGlass();
};

inline const QMap<QString, DispersionFormula*>& dispersionFormulas()
{
    static ShottFormula shott;
    static SellmeierFormula sellmeier;
    static ReznikFormula reznik;
    static CustomFormula custom;
    static QMap<QString, DispersionFormula*> formulas;
    formulas.insert(shott.name(), &shott);
    formulas.insert(sellmeier.name(), &sellmeier);
    formulas.insert(reznik.name(), &reznik);
    formulas.insert(custom.name(), &custom);
    return formulas;
}

//------------------------------------------------------------------------------

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
    DispersionFormula* formula() { return _formula; }

private:
    Glass* _glass = nullptr;
    DispersionFormula* _formula = nullptr;

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
