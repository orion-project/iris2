#ifndef DISPERSIONPLOT_H
#define DISPERSIONPLOT_H

#include <QWidget>

class GlassItem;

class DispersionPlot : public QWidget
{
    Q_OBJECT

public:
    explicit DispersionPlot(QWidget *parent = nullptr);

    void addGlass(GlassItem* item);

private:
    QList<GlassItem*> _items;

    void updatePlot();

    QString makeWindowTitle() const;
};

#endif // DISPERSIONPLOT_H
