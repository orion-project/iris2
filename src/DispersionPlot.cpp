#include "Catalog.h"
#include "GlassListWidget.h"
#include "DispersionPlot.h"
#include "helpers/OriLayouts.h"
#include "helpers/OriDialogs.h"

#include <QIcon>
#include <QDebug>
#include <QLabel>
#include <QSplitter>
#include <QTextBrowser>

using namespace Ori::Layouts;

DispersionPlot::DispersionPlot(Catalog* catalog) : QWidget(), _catalog(catalog)
{
    setWindowIcon(QIcon(":/icon/plot"));

    connect(_catalog, &Catalog::glassRemoved, this, &DispersionPlot::glassRemoved);

    _itemsView = new GlassListWidget(&_items);

    _plotView = new QTextBrowser;

    auto paramsPanel = LayoutV({
                                   makeHeaderLabel(tr("Materials")),
                                   _itemsView,
                                   Stretch()
                               })
            .setMargin(3)
            .setSpacing(3)
            .makeWidget();

    auto splitter = new QSplitter;
    splitter->addWidget(paramsPanel);
    splitter->addWidget(_plotView);

    Ori::Layouts::LayoutV({splitter}).setMargin(0).setSpacing(0).useFor(this);
}

DispersionPlot::~DispersionPlot()
{
}

void DispersionPlot::addGlass(GlassItem* item)
{
    if (_items.contains(item)) return;

    if (!item->glass())
    {
        auto res = _catalog->loadGlass(item);
        if (!res.isEmpty()) return Ori::Dlg::error(res);
    }

    _items.append(item);
    _itemsView->populate();

    updatePlot();
}

void DispersionPlot::updatePlot()
{
    setWindowTitle(makeWindowTitle());
}

QString DispersionPlot::makeWindowTitle() const
{
    if (_items.size() == 1)
         return tr("Dispersion Plot: %1").arg(_items.first()->title());
    QStringList glassTitles;
    for (GlassItem* item : _items) glassTitles << item->title();
    return tr("Dispersion Plots: %1").arg(glassTitles.join(", "));
}

void DispersionPlot::glassRemoved(GlassItem* item)
{
    if (!_items.contains(item)) return;
    _items.removeAll(item);
    _itemsView->populate();
    updatePlot();
}

QLabel* DispersionPlot::makeHeaderLabel(const QString& title) const
{
    QLabel* label = new QLabel(title);
    auto font = label->font();
    font.setBold(true);
    font.setPointSize(font.pointSize()+1);
    label->setFont(font);
    return label;
}

