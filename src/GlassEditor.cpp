#include "Catalog.h"
#include "Glass.h"
#include "GlassEditor.h"
#include "helpers/OriWidgets.h"
#include "helpers/OriDialogs.h"
#include "helpers/OriLayouts.h"
#include "widgets/OriValueEdit.h"

#include <QApplication>
#include <QComboBox>
#include <QDebug>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTabWidget>
#include <QTextEdit>

bool GlassEditor::createGlass(Catalog* catalog, FolderItem* parent)
{
    GlassEditor editor(CreateGlass, catalog);
    editor._parentFolder = parent;
    return editor.exec() == QDialog::Accepted;
}

bool GlassEditor::editGlass(Catalog *catalog, GlassItem* item)
{
    GlassEditor editor(EditGlass, catalog);
    if (!editor.populate(item)) return false;
    return editor.exec() == QDialog::Accepted;
}

GlassEditor::GlassEditor(DialogMode mode, Catalog *catalog) : QDialog(qApp->activeWindow()), _mode(mode), _catalog(catalog)
{
    setWindowTitle(tr("Material Properties"));
    setWindowIcon(QIcon(":/icon/glass"));

    _titleEditor = new QLineEdit;

    _formulaSelector = new QComboBox;
    for (DispersionFormula* formula: dispersionFormulas().values())
        _formulaSelector->addItem(formula->icon(), tr(formula->name()), QString(formula->name()));
    _lambdaMinEditor = new Ori::Widgets::ValueEdit;
    _lambdaMaxEditor = new Ori::Widgets::ValueEdit;

    auto layout = new QFormLayout;
    layout->addRow(new QLabel(tr("Material name")), _titleEditor);
    layout->addRow(new QLabel(tr("Dispersion formula")), _formulaSelector);
    layout->addRow(new QLabel(tr("Min wavelength, <b>μm</b>")), _lambdaMinEditor);
    layout->addRow(new QLabel(tr("Max wavelength, <b>μm</b>")), _lambdaMaxEditor);

    auto buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    qApp->connect(buttons, &QDialogButtonBox::accepted, this, &GlassEditor::apply);
    qApp->connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    _commentEditor = new QTextEdit;
    _commentEditor->setAcceptRichText(false);

    Ori::Gui::adjustFont(_titleEditor);
    Ori::Gui::adjustFont(_lambdaMinEditor);
    Ori::Gui::adjustFont(_lambdaMaxEditor);
    Ori::Gui::adjustFont(_commentEditor);

    QTabWidget* tabs = new QTabWidget;
    tabs->addTab(_commentEditor, tr("Comment"));

    Ori::Layouts::LayoutV({layout, tabs, buttons}).useFor(this);
}

bool GlassEditor::populate(GlassItem* item)
{
    if (!item->glass())
    {
        QString res = _catalog->loadGlass(item);
        if (!res.isEmpty())
        {
            Ori::Dlg::error(res);
            return false;
        }
    }

    _glassItem = item;
    Glass* glass = item->glass();
    _titleEditor->setText(glass->title());
    _lambdaMinEditor->setValue(glass->lambdaMin());
    _lambdaMaxEditor->setValue(glass->lambdaMax());
    _commentEditor->setPlainText(glass->comment());

    QString formulaName(_glassItem->formula()->name());
    for (int i = 0; i < _formulaSelector->count(); i++)
        if (_formulaSelector->itemData(i).toString() == formulaName)
        {
            _formulaSelector->setCurrentIndex(i);
            break;
        }

    return true;
}

void GlassEditor::apply()
{
    if (glassTitle().isEmpty())
        return Ori::Dlg::warning(tr("Material name can not be empty."));

    auto res = save();
    if (!res.isEmpty())
        return Ori::Dlg::error(res);

    accept();
}

QString GlassEditor::save()
{
    Glass *glass = formula()->makeGlass();
    if (_mode == EditGlass)
        glass->assign(_glassItem->glass());

    glass->_title = glassTitle();
    glass->_lambdaMin = lambdaMin();
    glass->_lambdaMax = lambdaMax();
    glass->_comment = glassComment();

    return _mode == CreateGlass
            ? _catalog->createGlass(_parentFolder, glass)
            : _catalog->updateGlass(_glassItem, glass);
}

QString GlassEditor::glassTitle() const { return _titleEditor->text().trimmed(); }
QString GlassEditor::glassComment() const  { return _commentEditor->toPlainText(); }
double GlassEditor::lambdaMin() const { return _lambdaMinEditor->value(); }
double GlassEditor::lambdaMax() const { return _lambdaMaxEditor->value(); }
DispersionFormula* GlassEditor::formula() const { return dispersionFormulas()[_formulaSelector->currentData().toString()]; }
