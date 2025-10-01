#pragma once
#include <qwidget.h>
#include "../Controller/Service.h"
#include <qlistwidget.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <QStackedWidget>
#include <QComboBox>
#include <QLabel>
#include <QTableView>
#include "WatchlistModel.h"

class Gui :public QWidget {
private:
      Service& service;

      QListWidget* repoList;
      QListWidget* watchList;
      //QListWidget* genreList;

      QLineEdit* titleEdit;
      QLineEdit* genreEdit;
      QLineEdit* trailerEdit;
      QLineEdit* yearEdit;
      QLineEdit* likesEdit;

      QLabel* genreLabel;

      QPushButton* addRepoButton;
      QPushButton* deleteRepoButton;
      QPushButton* updateRepoButton;
      QPushButton* HTMLOption;
      QPushButton* CSVOption;
      QPushButton* undoButton;
      QPushButton* redoButton;

      QStackedWidget* stacked;

      QWidget* userPage;
      QWidget* adminPage;
      QWidget* modePage;
      QWidget* chartPage;

      QRadioButton* csvRadio;
      QRadioButton* htmlRadio;

      QLineEdit* userGenreEdit;

      int positionInGenreList = 0;

      string currentGenre ="";

      QLineEdit* removeEdit;

      QComboBox* adminComboBox;

      WatchlistModel* model = nullptr;

      QTableView* tableView = nullptr;

      public:
      Gui(Service& service);
      void initGui();
      void populateList();
      void setupAdminUI();
      void setupUserUI();
      void populateWatchList();
      void addRepo();
      void deleteRepo();
      void updateRepo();
      void addToWatchList();
      void deleteFromWatchList();
      void openWatchList();
      void getMoviesByGenre();
      // void likeMovie();
      // void dontLikeMovie();
      void continueWatchList();
      void setupChartUI();
      void undo();
      void redo();


};