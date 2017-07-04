#ifndef GLASSEDITOR_H
#define GLASSEDITOR_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QComboBox;
class QLineEdit;
QT_END_NAMESPACE

class Catalog;
class GlassItem;
class FolderItem;
class DispersionFormula;

namespace Ori {
namespace Widgets {
class ValueEdit;
}}

class GlassEditor : public QDialog
{
    Q_OBJECT

public:
    static bool createGlass(Catalog *catalog, FolderItem* parent);
    static bool editGlass(Catalog *catalog, GlassItem* item);

private:
    enum DialogMode { CreateGlass, EditGlass };

    explicit GlassEditor(DialogMode mode, Catalog* catalog);

    QLineEdit *_titleEditor;
    QComboBox *_formulaSelector;
    Ori::Widgets::ValueEdit *_lambdaMinEditor, *_lambdaMaxEditor;

    DialogMode _mode;
    Catalog *_catalog;
    GlassItem *_glassItem = nullptr;
    FolderItem *_parentFolder;

    bool populate(GlassItem* item);
    void apply();
    QString save();

    DispersionFormula* formula() const;
    QString glassTitle() const;
    double lambdaMin() const;
    double lambdaMax() const;
    QString generateInfo() const;
};

#endif // GLASSEDITOR_H

