//
// Created by Filip on 30-May-25.
//

#include "WatchlistModel.h"

int WatchlistModel::rowCount(const QModelIndex &parent) const {
      return static_cast<int>(this->serv.getWatchList().size());
}

int WatchlistModel::columnCount(const QModelIndex &parent) const {
      return 5;
}

QVariant WatchlistModel::data(const QModelIndex &index, int role) const {
      int row = index.row();
      int column = index.column();
      Movie movie = this->serv.getWatchList()[row];
      if (role == Qt::DisplayRole) {
            switch (column) {
                  case 0:
                        return QString::fromStdString(movie.getTitle());
                  case 1:
                        return QString::fromStdString(movie.getGenre());
                  case 2:
                        return QString::fromStdString(std::to_string(movie.getYear()));
                  case 3:
                        return QString::fromStdString(std::to_string(movie.getLikes()));
                  case 4:
                        return QString::fromStdString(movie.getTrailer());
                  default:
                        break;
            }
      }
      return QVariant();
}

QVariant WatchlistModel::headerData(int section, Qt::Orientation orientation, int role) const {
      if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
            switch (section) {
                  case 0:
                        return "Title";
                  case 1:
                        return "Genre";
                  case 2:
                        return "Year";
                  case 3:
                        return "Likes";
                  case 4:
                        return "Trailer";
                  default:
                        break;
            }
      }
      return QVariant();
}

void WatchlistModel::addMovie(const Movie& mv) {
      int newRow = static_cast<int>(serv.getWatchList().size());

      beginInsertRows(QModelIndex(), newRow, newRow);

      serv.addMovieToWatchList(mv);

      endInsertRows();
}


