#ifndef CATALOGSTORE_H
#define CATALOGSTORE_H

#include <QString>

namespace CatalogStore {

QString newDatabase(const QString fileName);
QString openDatabase(const QString fileName);

} // namespace CatalogStore

#endif // CATALOGSTORE_H
