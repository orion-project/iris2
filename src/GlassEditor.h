#ifndef GLASSEDITOR_H
#define GLASSEDITOR_H

#include "dialogs/OriBasicConfigDlg.h"

QT_BEGIN_NAMESPACE
class QComboBox;
class QLineEdit;
class QTextEdit;
QT_END_NAMESPACE

class Catalog;
class GlassItem;
class FolderItem;
class DispersionFormula;

class FormulaView;

namespace Ori {
namespace Widgets {
class ValueEdit;
}}

class GlassEditor : public Ori::Dlg::BasicConfigDialog
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
    QTextEdit* _commentEditor;
    FormulaView* _formulaView;

    DialogMode _mode;
    Catalog *_catalog;
    GlassItem *_glassItem = nullptr;
    FolderItem *_parentFolder;

    bool populate(GlassItem* item);
    bool collect() override;

    DispersionFormula* formula() const;
    QString glassTitle() const;
    QString glassComment() const;
    double lambdaMin() const;
    double lambdaMax() const;

    QWidget* createGeneralPage();
    QWidget* createFormulaPage();
    QWidget* createCommentPage();
};

#endif // GLASSEDITOR_H

