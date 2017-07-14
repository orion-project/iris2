#include "Appearance.h"
#include "Catalog.h"
#include "Glass.h"
#include "GlassListWidget.h"
#include "DispersionPlot.h"
#include "helpers/OriWidgets.h"
#include "helpers/OriLayouts.h"
#include "helpers/OriDialogs.h"

#include <QIcon>
#include <QDebug>
#include <QLabel>
#include <QSplitter>
#include <QTextBrowser>

using namespace Ori::Layouts;

int getPlotPointsCount() { return 100; } // TODO make configurable

//------------------------------------------------------------------------------

void GlassPlot::calc()
{
    QStringList report;
    double lambdaMin = _glass->lambdaMin();
    double lambdaMax = _glass->lambdaMax();
    if (lambdaMax < lambdaMin) std::swap(lambdaMax, lambdaMin);
    int pointCount = getPlotPointsCount();
    double lambdaStep = (lambdaMax - lambdaMin) / double(pointCount-1);
    double lambda = lambdaMin;
    for (int i = 0; i < pointCount; i++)
    {
        double value = _glass->calcIndex(lambda);
        report.append(QString("%1\t%2\t%3").arg(i).arg(lambda).arg(value));
        lambda += lambdaStep;
    }
    result = report.join('\n');
}

//------------------------------------------------------------------------------

DispersionPlot::DispersionPlot(Catalog* catalog) : QWidget(), _catalog(catalog)
{
    setWindowIcon(QIcon(":/icon/plot"));

    connect(_catalog, &Catalog::glassRemoved, this, &DispersionPlot::glassRemoved);

    _itemsView = new GlassListWidget(&_items);

    _plotView = new QTextBrowser;
    Ori::Gui::setFontMonospace(_plotView);

    auto paramsPanel = LayoutV({
                                   Z::Gui::makeHeaderLabel(tr("Materials")),
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
    qDeleteAll(_plots.values());
}

void DispersionPlot::addGlass(GlassItem* item)
{
    if (_items.contains(item)) return;

    if (!item->glass())
    {
        auto res = _catalog->loadGlass(item);
        if (!res.isEmpty()) return Ori::Dlg::error(res);
    }

    auto res = item->glass()->prepare();
    if (!res.isEmpty()) return Ori::Dlg::error(res);

    auto plot = new GlassPlot(item->glass());
    plot->calc(); // TODO actual plotting
    _plotView->setPlainText(plot->result);

    _items.append(item);
    _plots.insert(item, plot);
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
    delete _plots[item];
    _plots.remove(item);
    _items.removeAll(item);
    _itemsView->populate();
    updatePlot();
}



