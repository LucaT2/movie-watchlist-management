//
// Created by Filip on 30-May-25.
//

#ifndef WATCHLISTMODEL_H
#define WATCHLISTMODEL_H
#include <QAbstractTableModel>
#include <QAbstractItemModel>

#include "../Controller/Service.h"


class WatchlistModel: public QAbstractTableModel {
private:
      Service& serv;
public:
      WatchlistModel(Service& service):serv{service}{};
      int rowCount(const QModelIndex& parent = QModelIndex()) const override;
      int columnCount(const QModelIndex& parent = QModelIndex()) const override;
      QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
      QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
      void refresh() {
            beginResetModel();
            endResetModel();
      };
      void addMovie(const Movie& mv);
};



#endif //WATCHLISTMODEL_H
