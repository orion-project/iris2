#ifndef DISPERSIONPLOT_H
#define DISPERSIONPLOT_H

#include <QWidget>

class Catalog;
class GlassItem;


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
    QList<GlassItem*> _items;

    void updatePlot();

    QString makeWindowTitle() const;

    void glassRemoved(GlassItem* item);
};

#endif // DISPERSIONPLOT_H
