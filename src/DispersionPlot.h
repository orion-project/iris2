#ifndef DISPERSIONPLOT_H
#define DISPERSIONPLOT_H

#include <QWidget>
#include <QMap>

QT_BEGIN_NAMESPACE
class QLabel;
class QTextBrowser;
QT_END_NAMESPACE

class Catalog;
class Glass;
class GlassItem;
class GlassListWidget;

class GlassPlot
{
public:
    GlassPlot(Glass* glass) : _glass(glass) {}

    void calc();

    QString result;

private:
    Glass* _glass;
};

//------------------------------------------------------------------------------

class PlotWindow
{
public:
    virtual void addGlass(GlassItem* item) = 0;
};


class DispersionPlot : public QWidget, public PlotWindow
{
    Q_OBJECT

public:
    explicit DispersionPlot(Catalog* catalog);
    ~DispersionPlot();

    void addGlass(GlassItem* item) override;

private:
    Catalog* _catalog;

    // TODO combine into one object
    QList<GlassItem*> _items;
    QMap<GlassItem*, GlassPlot*> _plots;

    GlassListWidget* _itemsView;
    QTextBrowser* _plotView;

    void updatePlot();

    QString makeWindowTitle() const;

    void glassRemoved(GlassItem* item);
};

#endif // DISPERSIONPLOT_H
