#include "GUI.h"
#include <qboxlayout.h>
#include <qlabel.h>
#include <qformlayout.h>
#include <qradiobutton.h>
#include<qmessagebox.h>
#include "../Validators_Exceptions/Exceptions.h"
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarSet>
#include <QPalette>
#include <QShortcut>

Gui::Gui(Service &serv): service(serv) {
      this->initGui();
}

void Gui::initGui() {
      auto *mainLayout = new QVBoxLayout(this);
      modePage = new QWidget;
      auto *modeLayout = new QHBoxLayout(modePage);
      auto *adminBtn = new QPushButton("Admin");
      auto *userBtn = new QPushButton("User");
      htmlRadio = new QRadioButton("HTML");
      csvRadio = new QRadioButton("CSV");
      modeLayout->addWidget(adminBtn);
      modeLayout->addWidget(userBtn);
      modeLayout->addWidget(htmlRadio);
      modeLayout->addWidget(csvRadio);

      adminPage = new QWidget;
      setupAdminUI();
      userPage = new QWidget;

      stacked = new QStackedWidget;
      stacked->addWidget(modePage);
      stacked->addWidget(adminPage);
      stacked->addWidget(userPage);

      mainLayout->addWidget(stacked);

      connect(adminBtn, &QPushButton::clicked, this, [=]() {
            stacked->setCurrentWidget(adminPage);
      });
      connect(userBtn, &QPushButton::clicked, this, [=]() {
            FileWatchList *wl = nullptr;
            if (csvRadio->isChecked())
                  wl = new CSVWatchList("../Storage_files/watch_list.csv");
            else
                  wl = new HTMLWatchList("../Storage_files/watch_list.html");
            service.setWatchList(wl);
            try {
                  Movie mv = Movie("The Godfather", "Crime", 1972, 2250000,
                                   "https://www.youtube.com/watch?v=sY1S34973zA");
                  service.addMovieToWatchList(mv);
                  populateWatchList();
            } catch (const std::exception &e) {
                  QMessageBox::warning(nullptr, "Error", e.what());
            }
            setupUserUI();
            //populateWatchList();


            stacked->setCurrentWidget(userPage);
      });


      auto *chartBtn = new QPushButton("Chart");
      modeLayout->addWidget(chartBtn);

      chartPage = new QWidget;
      setupChartUI();

      stacked->addWidget(chartPage);

      connect(chartBtn, &QPushButton::clicked, this, [=]() {
            stacked->setCurrentWidget(chartPage);
      });
}

void Gui::populateList() {
      this->repoList->clear();
      bool showDetailed = adminComboBox->currentData().toBool();

      std::vector<Movie> movies = this->service.getMoviesByGenre("");
      for (auto &movie: movies) {
            std::string str = movie.getTitle() + " - " + movie.getGenre() + " - " + std::to_string(movie.getYear()) +
                              " - " +
                              std::to_string(movie.getLikes()) + " - " + movie.getTrailer();
            if (showDetailed)
                  this->repoList->addItem(QString::fromStdString(str));
            else
                  this->repoList->addItem(QString::fromStdString(movie.getTitle()));
      }
}

void Gui::setupAdminUI() {
      auto *mainLayout = new QHBoxLayout(adminPage);
      this->repoList = new QListWidget();
      mainLayout->addWidget(this->repoList);


      //this->adminComboBox = new QComboBox(&window);
      auto *rightLayout = new QVBoxLayout();

      adminComboBox = new QComboBox(adminPage);
      adminComboBox->addItem("Detailed", QVariant(true));
      adminComboBox->addItem("Short", QVariant(false));
      adminComboBox->setCurrentIndex(0);
      rightLayout->addWidget(adminComboBox);

      auto *moviesAttributes = new QFormLayout();
      auto *titleLabel = new QLabel("Title");
      auto *genreLabel = new QLabel("Genre");
      auto *yearLabel = new QLabel("Year");
      auto *likesLabel = new QLabel("Likes");
      auto *trailerLabel = new QLabel("Trailer");
      this->titleEdit = new QLineEdit();
      this->genreEdit = new QLineEdit();
      this->yearEdit = new QLineEdit();
      this->likesEdit = new QLineEdit();
      this->trailerEdit = new QLineEdit();
      moviesAttributes->addRow(titleLabel, this->titleEdit);
      moviesAttributes->addRow(genreLabel, this->genreEdit);
      moviesAttributes->addRow(yearLabel, this->yearEdit);
      moviesAttributes->addRow(likesLabel, this->likesEdit);
      moviesAttributes->addRow(trailerLabel, this->trailerEdit);
      this->addRepoButton = new QPushButton("Add Movie");
      this->deleteRepoButton = new QPushButton("Delete Movie");
      this->updateRepoButton = new QPushButton("Update Movie");

      this->undoButton = new QPushButton("Undo");
      this->redoButton = new QPushButton("Redo");

      rightLayout->addLayout(moviesAttributes);
      rightLayout->addWidget(this->addRepoButton);
      addRepoButton->setStyleSheet(R"(
    QPushButton {
        /* left→right gradient: white → gray → green */
        background: qlineargradient(
            x1: 0, y1: 0, x2: 1, y2: 0,
            stop: 0.0 white,
            stop: 0.4 gray,
            stop: 1.0 green
        );
        border: 1px solid #777;
        border-radius: 4px;
        padding: 6px 12px;
        color: black;
    }
)");
      rightLayout->addWidget(this->deleteRepoButton);
      rightLayout->addWidget(this->updateRepoButton);

      rightLayout->addWidget(this->undoButton);
      rightLayout->addWidget(this->redoButton);

      mainLayout->addLayout(rightLayout);

      this->populateList();
      connect(addRepoButton, &QPushButton::clicked, this, &Gui::addRepo);
      connect(deleteRepoButton, &QPushButton::clicked, this, &Gui::deleteRepo);
      connect(updateRepoButton, &QPushButton::clicked, this, &Gui::updateRepo);

      connect(adminComboBox,
              QOverload<int>::of(&QComboBox::currentIndexChanged),
              this,
              &Gui::populateList);

      connect(undoButton, &QPushButton::clicked, this, &Gui::undo);
      connect(redoButton, &QPushButton::clicked, this, &Gui::redo);

      auto *undoSc = new QShortcut(QKeySequence::Undo, adminPage);
      connect(undoSc, &QShortcut::activated, this, undo);

      auto *redoSc = new QShortcut(QKeySequence::Redo, adminPage);
      connect(redoSc, &QShortcut::activated, this, redo);
}

void Gui::undo() {
      //QMessageBox::information(adminPage, "Undo", tr("Undo"));
      this->service.undo();
      populateList();
}

void Gui::redo() {
      //QMessageBox::information(adminPage, "Redo", tr("Redo"));
      this->service.redo();
      populateList();
}

void Gui::addRepo() {
      QString title = this->titleEdit->text();
      QString genre = this->genreEdit->text();
      QString year = this->yearEdit->text();
      QString likes = this->likesEdit->text();
      QString trailer = this->trailerEdit->text();
      try {
            this->service.addMovie(Movie(title.toStdString(), genre.toStdString(),
                                         year.toInt(), likes.toInt(), trailer.toStdString()));
            populateList();
      } catch (const RepositoryException &error) {
            QMessageBox::information(nullptr, "Alert", error.what());
      }
}

void Gui::deleteRepo() {
      QString title = this->titleEdit->text();
      try {
            this->service.removeMovie(title.toStdString());
            populateList();
      } catch (const RepositoryException &error) {
            QMessageBox::information(nullptr, "Alert", error.what());
      }
}

void Gui::updateRepo() {
      QString title = this->titleEdit->text();
      QString genre = this->genreEdit->text();
      QString year = this->yearEdit->text();
      QString likes = this->likesEdit->text();
      QString trailer = this->trailerEdit->text();
      try {
            Movie mv = Movie(title.toStdString(), genre.toStdString(), year.toInt(), likes.toInt(),
                             trailer.toStdString());
            this->service.updateMovie(title.toStdString(), mv);
            populateList();
      } catch (const RepositoryException &error) {
            QMessageBox::information(nullptr, "Alert", error.what());
      }
}

void Gui::populateWatchList() {
      // this->watchList->clear();
      // std::vector<Movie> movies = this->service.getWatchList();
      // if (movies.size() == 0)
      //       return;
      // int count = 0;
      // for (auto& movie : movies) {
      //       std::string str = std::to_string(count)+" - "+ movie.getTitle()+" - "+movie.getGenre()+ " - "+ std::to_string(movie.getYear())+ " - " +
      //             std::to_string(movie.getLikes())+" - "+ movie.getTrailer();
      //       count++;
      //       this->watchList->addItem(QString::fromStdString(str));
      // }
      if (model != nullptr && tableView != nullptr) {
            model->refresh();
            tableView->resizeColumnsToContents();
      }
}


void Gui::setupUserUI() {
      auto *mainLayout = new QHBoxLayout(userPage);
      this->watchList = new QListWidget();
      auto *rightLayout = new QVBoxLayout();
      auto *leftLayout = new QVBoxLayout();
      //leftLayout->addWidget(this->watchList);
      //mainLayout->addWidget(this->watchList);

      //auto* watchListLayout = new QVBoxLayout();
      //auto* genreLayout = new QVBoxLayout();

      //auto* watchListLabel = new QLabel("This is the watch list");
      auto *getGenreLabel = new QLabel("This is the current movie that you might want to add");
      QFont f1 = getGenreLabel->font();
      f1.setBold(true);
      f1.setPointSize(14);
      getGenreLabel->setFont(f1);
      //genreLayout->addWidget(getGenreLabel);

      this->genreLabel = new QLabel();
      //genreLayout->addWidget(genreLabel);
      this->genreLabel->setMinimumSize(QSize(200, 50));
      QFont f = this->genreLabel->font();
      f.setPointSize(14);
      this->genreLabel->setFont(f);
      //leftLayout->addWidget(genreList);


      //watchListLayout->addWidget(watchListLabel);
      //watchListLayout->addWidget(this->watchList);

      //leftLayout->addLayout(watchListLayout);
      //leftLayout->addLayout(genreLayout);
      leftLayout->addWidget(getGenreLabel);
      leftLayout->addWidget(genreLabel);
      mainLayout->addLayout(leftLayout);

      auto *getListBtn = new QPushButton("Get movies by Genre");
      this->userGenreEdit = new QLineEdit();
      this->userGenreEdit->setPlaceholderText("Enter Genre");

      auto *openWatchListBtn = new QPushButton("Open Watch List in the given format");
      auto *addBtn = new QPushButton("Add Movie to Watch List");
      auto *removeBtn = new QPushButton("Remove Movie from Watch List");
      removeEdit = new QLineEdit();
      removeEdit->setPlaceholderText("Enter Title");
      auto *getLayout = new QVBoxLayout();
      auto *removeLayout = new QVBoxLayout();
      auto *clearBtn = new QPushButton("Clear the current execution");
      auto *continueBtn = new QPushButton("Continue without adding to the list");
      auto *contAddLayout = new QVBoxLayout();
      contAddLayout->addWidget(addBtn);
      contAddLayout->addWidget(continueBtn);

      getLayout->addWidget(getListBtn);
      getLayout->addWidget(this->userGenreEdit);

      rightLayout->addLayout(getLayout);
      rightLayout->addWidget(clearBtn);
      rightLayout->addWidget(openWatchListBtn);
      rightLayout->addLayout(contAddLayout);


      removeLayout->addWidget(removeBtn);
      removeLayout->addWidget(removeEdit);

      rightLayout->addLayout(removeLayout);
      mainLayout->addLayout(rightLayout);

      if (model == nullptr) {
            model = new WatchlistModel(this->service);
      }
      if (tableView == nullptr) {
            tableView = new QTableView();
            tableView->setParent(nullptr);
      }

      tableView->setModel(this->model);
      tableView->resizeColumnsToContents();
      tableView->setWindowTitle("My watchlist");
      tableView->show();

      this->populateWatchList();

      connect(openWatchListBtn, &QPushButton::clicked, this, &Gui::openWatchList);

      connect(getListBtn, &QPushButton::clicked, this, &Gui::getMoviesByGenre);

      connect(addBtn, &QPushButton::clicked, this, &Gui::addToWatchList);

      connect(continueBtn, &QPushButton::clicked, this, &Gui::continueWatchList);

      connect(removeBtn, &QPushButton::clicked, this, &Gui::deleteFromWatchList);

      connect(clearBtn, &QPushButton::clicked, this, [=] {
            //this->genreList->clear();
            this->genreLabel->setText("");
            positionInGenreList = 0;
      });
}

void Gui::addToWatchList() {
      try {
            //this->service.addMovieToWatchList(service.getMoviesByGenre(currentGenre)[positionInGenreList]);
            model->addMovie(service.getMoviesByGenre(currentGenre)[positionInGenreList]);
      } catch (...) {
            QMessageBox::warning(nullptr, "Error", "Movie Already Exists");
            return;
      }

      //populateWatchList();
      positionInGenreList++;
      if (positionInGenreList == this->service.getMoviesByGenre(currentGenre).size())
            positionInGenreList = 0;
      this->getMoviesByGenre();
      //this->service.openURL(service.getMoviesByGenre(currentGenre)[positionInGenreList].getTrailer());
}

void Gui::continueWatchList() {
      positionInGenreList++;
      if (this->service.getMoviesByGenre(currentGenre).size() == positionInGenreList)
            positionInGenreList = 0;
      this->getMoviesByGenre();
      //this->service.openURL(service.getMoviesByGenre(currentGenre)[positionInGenreList].getTrailer());
}

void Gui::deleteFromWatchList() {
      string str = this->removeEdit->text().toStdString();
      bool found = false;
      int num;
      Movie mv{};
      for (int i = 0; i < this->service.getWatchList().size(); i++) {
            if (service.getWatchList()[i].getTitle() == str) {
                  found = true;
                  mv = service.getWatchList()[i];
                  num = i;
                  break;
            }
      }
      if (!found) {
            QMessageBox::warning(nullptr,
                                 "Invalid Title",
                                 QString::fromStdString("Please enter a valid title "));
            return;
      }


      //cout<<mv.getTitle()<<"\n";
      QString title = QString::fromUtf8(mv.getTitle().c_str());
      QString text = QObject::tr("Did you like \"%1\"?").arg(title);
      QMessageBox likeBox;
      likeBox.setWindowTitle("Rate Movie");
      likeBox.setText(text);
      likeBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
      likeBox.setDefaultButton(QMessageBox::Yes);
      int response = likeBox.exec();


      if (response == QMessageBox::Yes) {
            this->service.incrementMovieLikes(mv.getTitle());
      }

      this->service.removeMovieFromWatchList(num);
      populateWatchList();
}

void Gui::openWatchList() {
      this->service.saveFile();
      this->service.displayFile();
}

void Gui::getMoviesByGenre() {
      QString genre = this->userGenreEdit->text();
      currentGenre = genre.toStdString();
      vector<Movie> movies = this->service.getMoviesByGenre(currentGenre);
      if (movies.empty()) {
            QMessageBox::information(nullptr, "No matches", "No movies found for genre “" + genre + "”");
            return;
      }
      Movie movie = movies[positionInGenreList % movies.size()];
      std::string str = std::to_string(positionInGenreList) + " - " + movie.getTitle() + " - " + movie.getGenre() +
                        " - " + std::to_string(movie.getYear()) + " - " +
                        std::to_string(movie.getLikes()) + " - " + movie.getTrailer();
      this->genreLabel->setText(QString::fromStdString(str));
      this->service.openURL(movie.getTrailer());
}


void Gui::setupChartUI() {
      auto *layout = new QVBoxLayout(chartPage);

      auto counts = service.countMoviesByGenre();
      QStringList categories;
      QBarSet *set = new QBarSet("Movies");
      int maxCount = 0;

      for (auto &p: counts) {
            categories << QString::fromStdString(p.first);
            *set << p.second;
            maxCount = std::max(maxCount, p.second);
      }

      QBarSeries *series = new QBarSeries();
      series->append(set);

      QChart *chart = new QChart();
      chart->addSeries(series);
      chart->setTitle("Number of Movies per Genre");

      QBarCategoryAxis *axisX = new QBarCategoryAxis();
      axisX->append(categories);
      axisX->setTitleText("Genre");
      chart->addAxis(axisX, Qt::AlignBottom);
      series->attachAxis(axisX);

      QValueAxis *axisY = new QValueAxis();
      axisY->setRange(0, maxCount);
      axisY->setLabelFormat("%d");
      axisY->setTickCount(maxCount + 1);
      axisY->setMinorTickCount(0);
      axisY->setTitleText("Count");
      chart->addAxis(axisY, Qt::AlignLeft);
      series->attachAxis(axisY);

      QChartView *chartView = new QChartView(chart);
      chartView->setRenderHint(QPainter::Antialiasing);

      layout->addWidget(chartView);
}
