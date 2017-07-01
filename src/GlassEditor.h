#ifndef GLASSEDITOR_H
#define GLASSEDITOR_H

#include <QWidget>

class Catalog;
class GlassItem;
class FolderItem;

class GlassEditor : public QWidget
{
    Q_OBJECT

public:
    static bool createGlass(Catalog *catalog, FolderItem* parent);
    static bool editGlass(Catalog *catalog, GlassItem* item);

private:
    explicit GlassEditor(QWidget *parent = 0);
};

#endif // GLASSEDITOR_H
