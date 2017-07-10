#ifndef DISPERSIONPLOT_H
#define DISPERSIONPLOT_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class QTextBrowser;
QT_END_NAMESPACE

class Catalog;
class GlassItem;
class GlassListWidget;

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
    GlassListWidget* _itemsView;
    QTextBrowser* _plotView;

    void updatePlot();

    QString makeWindowTitle() const;
    QLabel* makeHeaderLabel(const QString& title) const;

    void glassRemoved(GlassItem* item);
};

#endif // DISPERSIONPLOT_H
