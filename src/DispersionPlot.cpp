#include "Catalog.h"
#include "DispersionPlot.h"

#include <QIcon>
#include <QDebug>

DispersionPlot::DispersionPlot(Catalog* catalog) : QWidget(), _catalog(catalog)
{
    setWindowIcon(QIcon(":/icon/plot"));

    connect(_catalog, &Catalog::glassRemoved, this, &DispersionPlot::glassRemoved);
}

DispersionPlot::~DispersionPlot()
{
    qDebug() << "destructor";
}

void DispersionPlot::addGlass(GlassItem* item)
{
    if (_items.contains(item)) return;

    _items.append(item);

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
    updatePlot();
}
