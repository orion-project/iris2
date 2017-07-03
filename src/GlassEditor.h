#ifndef GLASSEDITOR_H
#define GLASSEDITOR_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE

class Catalog;
class GlassItem;
class FolderItem;

class GlassEditor : public QDialog
{
    Q_OBJECT

public:
    static bool createGlass(Catalog *catalog, FolderItem* parent);
    static bool editGlass(Catalog *catalog, GlassItem* item);

private:
    enum DialogMode { CreateGlass, EditGlass };

    explicit GlassEditor(DialogMode mode, Catalog* catalog);

    QLineEdit* _nameEditor;

    DialogMode _mode;
    Catalog* _catalog;
    GlassItem* _glassItem = nullptr;
    FolderItem* _parentFolder;

    void apply();
    QString save();

    QString glassTitle();
};

#endif // GLASSEDITOR_H
