#include "Catalog.h"
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
    return editor.exec() != QDialog::Accepted;
}

bool GlassEditor::editGlass(Catalog *catalog, GlassItem* item)
{
    GlassEditor editor(EditGlass, catalog);
    editor._glassItem = item;
    return editor.exec() != QDialog::Accepted;
}

GlassEditor::GlassEditor(DialogMode mode, Catalog *catalog) : QDialog(qApp->activeWindow()), _mode(mode), _catalog(catalog)
{
    setWindowTitle(tr("Material Properties"));
    setWindowIcon(QIcon(":/icon/glass"));

    _nameEditor = new QLineEdit;
//    auto formulaSelector = new QComboBox;
//    auto lambdaMinEditor = new Ori::Widgets::ValueEdit;
//    auto lambdaMaxEditor = new Ori::Widgets::ValueEdit;

//    Ori::Layouts::Grid()
//            .row(tr("Dispersion formula", formulaSelector))
//            .row(tr("Min wavelength, <b>μm</b>"), lambdaMinEditor)
//            .row(tr("Max wavelength, <b>μm</b>"), lambdaMaxEditor)
//            .useFor(this);

    auto layout = new QFormLayout;
    layout->addRow(new QLabel(tr("Material name")), _nameEditor);

    auto buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    qApp->connect(buttons, &QDialogButtonBox::accepted, this, &GlassEditor::apply);
    qApp->connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    Ori::Layouts::LayoutV({layout, buttons}).useFor(this);
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
    if (!_glassItem)
        _glassItem = new GlassItem;

    _glassItem->setTitle(glassTitle());

    if (_mode == CreateGlass)
    {
        return _catalog->createGlass(_parentFolder, _glassItem);
    }
    else
    {
        return _catalog->updateGlass(_glassItem);
    }
}

QString GlassEditor::glassTitle()
{
    return _nameEditor->text().trimmed();
}
