#include "Catalog.h"
#include "DispersionPlot.h"

#include <QIcon>

DispersionPlot::DispersionPlot(QWidget *parent) : QWidget(parent)
{
    setWindowIcon(QIcon(":/icon/plot"));
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
