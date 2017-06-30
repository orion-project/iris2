#include "Catalog.h"
#include "CatalogWidget.h"
#include "CatalogModel.h"
#include "helpers/OriLayouts.h"
#include "helpers/OriDialogs.h"

#include <QMenu>
#include <QTreeView>

CatalogWidget::CatalogWidget() : QWidget()
{
    _folderMenu = new QMenu(this);
    _folderMenuHeader = makeHeaderItem(_folderMenu);
    _folderMenu->addSeparator();
    _folderMenu->addAction(tr("New Folder..."), this, &CatalogWidget::createFolder);
    _folderMenu->addAction(tr("Rename..."), this, &CatalogWidget::renameFolder);
    _folderMenu->addSeparator();
    _folderMenu->addAction(tr("Delete"), this, &CatalogWidget::deleteFolder);

    _glassMenu = new QMenu(this);
    _glassMenuHeader = makeHeaderItem(_glassMenu);
    _glassMenu->addSeparator();

    _catalogView = new QTreeView;
    _catalogView->setHeaderHidden(true);
    _catalogView->setAlternatingRowColors(true);
    _catalogView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_catalogView, &QTreeView::customContextMenuRequested, this, &CatalogWidget::contextMenuRequested);

    Ori::Layouts::LayoutV({_catalogView})
            .setMargin(0)
            .setSpacing(0)
            .useFor(this);
}

QAction* CatalogWidget::makeHeaderItem(QMenu* menu)
{
    QAction* item = menu->addAction("");
    auto font = item->font();
    font.setBold(true);
    font.setPointSize(font.pointSize()+2);
    item->setFont(font);
    item->setEnabled(false);
    return item;
}

void CatalogWidget::setCatalog(Catalog* catalog)
{
    _catalog = catalog;
    if (_catalogModel) delete _catalogModel;
    _catalogModel = new CatalogModel(_catalog);
    _catalogView->setModel(_catalogModel);
}

void CatalogWidget::contextMenuRequested(const QPoint &pos)
{
    auto item = selectedItem();
    if (!item) return;

    if (item->isFolder())
    {
        _folderMenuHeader->setText(item->title());
        _folderMenu->popup(_catalogView->mapToGlobal(pos));
    }
    else
    {
        _glassMenuHeader->setText(item->title());
        _glassMenu->popup(_catalogView->mapToGlobal(pos));
    }
}

CatalogItem* CatalogWidget::selectedItem() const
{
    if (!_catalogModel) return nullptr;

    auto index = _catalogView->currentIndex();
    if (!index.isValid()) return nullptr;

    return CatalogModel::catalogItem(index);;
}

FolderItem* CatalogWidget::selectedFolder() const
{
    auto item = selectedItem();
    return item ? item->asFolder() : nullptr;
}

GlassItem* CatalogWidget::selectedGlass() const
{
    auto item = selectedItem();
    return item ? item->asGlass() : nullptr;
}

void CatalogWidget::createFolder()
{
    auto index = _catalogView->currentIndex();
    if (!index.isValid()) return;

    auto item = CatalogModel::catalogItem(index);
    if (!item) return;

    auto folder = item->asFolder();

    auto title = Ori::Dlg::inputText(tr("Enter a title for new folder"), "");
    if (title.isEmpty()) return;

    auto res = _catalog->createFolder(folder, title);
    if (!res.isEmpty()) return Ori::Dlg::error(res);

    auto newIndex = _catalogModel->itemAdded(index);
    if (!_catalogView->isExpanded(index))
        _catalogView->expand(index);
    _catalogView->setCurrentIndex(newIndex);
}

void CatalogWidget::renameFolder()
{
    auto index = _catalogView->currentIndex();
    if (!index.isValid()) return;

    auto item = CatalogModel::catalogItem(index);
    if (!item) return;

    auto folder = item->asFolder();
    if (!folder) return;

    auto title = Ori::Dlg::inputText(tr("Enter new title for folder"), folder->title());
    if (title.isEmpty()) return;

    auto res = _catalog->renameFolder(folder, title);
    if (!res.isEmpty()) return Ori::Dlg::error(res);

    _catalogModel->itemRenamed(index);
}

void CatalogWidget::deleteFolder()
{
    auto index = _catalogView->currentIndex();
    if (!index.isValid()) return;

    auto item = CatalogModel::catalogItem(index);
    if (!item) return;

    auto folder = item->asFolder();
    if (!folder) return;

    auto confirm = tr("Are you sure to delete folder '%1' and all its content?\n\n"
                      "This action can't be undone.").arg(folder->title());
    if (!Ori::Dlg::yes(confirm)) return;

    auto res = _catalog->removeFolder(folder);
    if (!res.isEmpty()) return Ori::Dlg::error(res);

    auto parentIndex = _catalogModel->itemRemoved(index);
    _catalogView->setCurrentIndex(parentIndex);
}
