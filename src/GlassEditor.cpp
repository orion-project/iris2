#include "Catalog.h"
#include "GlassEditor.h"

#include "helpers/OriDialogs.h"

bool GlassEditor::createGlass(Catalog* catalog, FolderItem* parent)
{
    // TODO open real editor
    auto title = Ori::Dlg::inputText(tr("Enter a title for new glass"), "");
    if (title.isEmpty()) return false;

    auto res = catalog->createGlass(parent, title);
    if (!res.isEmpty())
    {
        Ori::Dlg::error(res);
        return false;
    }

    return true;
}

bool GlassEditor::editGlass(Catalog *catalog, GlassItem* item)
{
    // TODO open real editor
    auto title = Ori::Dlg::inputText(tr("Enter new title for material"), item->title());
    if (title.isEmpty()) return false;

    auto res = catalog->updateGlass(item, title);
    if (!res.isEmpty())
    {
        Ori::Dlg::error(res);
        return false;
    }

    return true;
}

GlassEditor::GlassEditor(QWidget *parent) : QWidget(parent)
{

}
