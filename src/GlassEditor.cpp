#include "Catalog.h"
#include "Glass.h"
#include "GlassEditor.h"
#include "helpers/OriDialogs.h"
#include "helpers/OriLayouts.h"
#include "widgets/OriValueEdit.h"

#include <QApplication>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>

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

    Ori::Layouts::LayoutV({layout, buttons}).useFor(this);
}

bool GlassEditor::populate(GlassItem* item)
{
    if (!item->glass())
    {
        auto res = _catalog->loadGlass(item);
        if (!res.isEmpty())
        {
            Ori::Dlg::error(res);
            return false;
        }
    }

    _glassItem = item;
    _titleEditor->setText(item->title());
    _lambdaMinEditor->setValue(item->glass()->lambdaMin());
    _lambdaMaxEditor->setValue(item->glass()->lambdaMax());
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
    Glass *glass = _mode == CreateGlass
            ? new Glass
            : _glassItem->glass();

    glass->_title = glassTitle();
    glass->_lambdaMin = lambdaMin();
    glass->_lambdaMax = lambdaMax();

    return _mode == CreateGlass
            ? _catalog->createGlass(_parentFolder, glass)
            : _catalog->updateGlass(_glassItem, glass);
}

QString GlassEditor::glassTitle() const { return _titleEditor->text().trimmed(); }
double GlassEditor::lambdaMin() const { return _lambdaMinEditor->value(); }
double GlassEditor::lambdaMax() const { return _lambdaMaxEditor->value(); }
