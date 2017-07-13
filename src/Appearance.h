#ifndef APPEARANCE_H
#define APPEARANCE_H

#include <QLabel>

namespace Z {
namespace Gui {

inline void setValueFont(QWidget *widget)
{
    QFont f = widget->font();
    if (f.pointSize() < 10)
    {
        f.setPointSize(10);
        widget->setFont(f);
    }
}

inline void setSymbolFont(QWidget *widget)
{
    QFont f = widget->font();
    f.setBold(true);
    f.setPointSize(14);
    f.setFamily("Times New Roman");
    widget->setFont(f);
}

inline QLabel* makeSymbolLabel(const QString& text)
{
    auto label = new QLabel(text);
    setSymbolFont(label);
    return label;
}

} // namespace Gui
} // namespace Z

#endif // APPEARANCE_H
