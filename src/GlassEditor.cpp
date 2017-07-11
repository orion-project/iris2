#include "Catalog.h"
#include "Glass.h"
#include "GlassEditor.h"
#include "helpers/OriWidgets.h"
#include "helpers/OriDialogs.h"
#include "helpers/OriLayouts.h"
#include "widgets/OriValueEdit.h"
#include "qwt-mml-dev/formulaview.h"

#include <QApplication>
#include <QComboBox>
#include <QDebug>
#include <QFile>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>

bool GlassEditor::createGlass(Catalog* catalog, FolderItem* parent)
{
    GlassEditor editor(CreateGlass, catalog);
    editor._parentFolder = parent;
    return editor.run();
}

bool GlassEditor::editGlass(Catalog *catalog, GlassItem* item)
{
    GlassEditor editor(EditGlass, catalog);
    if (!editor.populate(item)) return false;
    return editor.run();
}

GlassEditor::GlassEditor(DialogMode mode, Catalog *catalog) :
    Ori::Dlg::BasicConfigDialog(qApp->activeWindow()), _mode(mode), _catalog(catalog)
{
    setWindowTitle(tr("Material Properties"));
    setWindowIcon(QIcon(":/icon/main"));
    setObjectName("GlassEditor");

    createPages({createGeneralPage(), createFormulaPage(), createCommentPage()});
}

QWidget* GlassEditor::createGeneralPage()
{
    auto page = new Ori::Dlg::BasicConfigPage(tr("General"), ":/icon/glass_blue");

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

    Ori::Gui::adjustFont(_titleEditor);
    Ori::Gui::adjustFont(_lambdaMinEditor);
    Ori::Gui::adjustFont(_lambdaMaxEditor);

    _titleEditor->setFocus();

    page->add({layout});
    return page;
}

QWidget* GlassEditor::createFormulaPage()
{
    auto page = new Ori::Dlg::BasicConfigPage(tr("Formula"));

    _formulaView = new FormulaView();

    // TODO load glass specific formula
    QFile file(":/formula/shott");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        _formulaView->setFormula(file.readAll());
        file.close();
    }

    page->add({_formulaView});
    return page;
}

QWidget* GlassEditor::createCommentPage()
{
    auto page = new Ori::Dlg::BasicConfigPage(tr("Comment"), ":/icon/comment");

    _commentEditor = new QTextEdit;
    _commentEditor->setAcceptRichText(false);
    Ori::Gui::adjustFont(_commentEditor);

    _commentEditor->setFocus();

    page->add({_commentEditor});
    return page;
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

bool GlassEditor::collect()
{
    if (glassTitle().isEmpty())
    {
        Ori::Dlg::warning(tr("Material name can not be empty."));
        return false;
    }

    Glass *glass = formula()->makeGlass();
    if (_mode == EditGlass)
        glass->assign(_glassItem->glass());

    glass->_title = glassTitle();
    glass->_lambdaMin = lambdaMin();
    glass->_lambdaMax = lambdaMax();
    glass->_comment = glassComment();

    auto res = _mode == CreateGlass
            ? _catalog->createGlass(_parentFolder, glass)
            : _catalog->updateGlass(_glassItem, glass);

    if (!res.isEmpty())
    {
        Ori::Dlg::error(res);
        return false;
    }

    return true;
}

QString GlassEditor::glassTitle() const { return _titleEditor->text().trimmed(); }
QString GlassEditor::glassComment() const  { return _commentEditor->toPlainText(); }
double GlassEditor::lambdaMin() const { return _lambdaMinEditor->value(); }
double GlassEditor::lambdaMax() const { return _lambdaMaxEditor->value(); }
DispersionFormula* GlassEditor::formula() const { return dispersionFormulas()[_formulaSelector->currentData().toString()]; }
