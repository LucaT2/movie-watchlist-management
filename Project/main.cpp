#include <iostream>
#include "GUI/GUI.h"
#include <QApplication>
#include <qlabel.h>
#include "Tests/Tests.h"
using namespace std;


void populate_database(DataBaseRepository& repo) {
      repo.addMovie(repo.createMovie("The Shawshank Redemption", "Drama", 1994, 0, "https://www.youtube.com/watch?v=6hB3S9bIaco"));
      repo.addMovie(repo.createMovie("Inception", "Sci-Fi", 2010, 5, "https://www.youtube.com/watch?v=YoHD9XEInc0"));
      repo.addMovie(repo.createMovie("The Dark Knight", "Action", 2008, 10, "https://www.youtube.com/watch?v=EXeTwQWrcwY"));
      repo.addMovie(repo.createMovie("Interstellar", "Adventure", 2014, 1800000, "https://www.youtube.com/watch?v=zSWdZVtXT7E"));
      repo.addMovie(repo.createMovie("Parasite", "Thriller", 2019, 1300000, "https://www.youtube.com/watch?v=5xH0HfJHsaY"));
      repo.addMovie(repo.createMovie("Pulp Fiction", "Crime", 1994, 1900000, "https://www.youtube.com/watch?v=s7EdQ4FqbhY"));
      repo.addMovie(repo.createMovie("The Matrix", "Sci-Fi", 1999, 1600000, "https://www.youtube.com/watch?v=vKQi3bBA1y8"));
      repo.addMovie(repo.createMovie("Fight Club", "Drama", 1999, 1750000, "https://www.youtube.com/watch?v=SUXWAEX2jlg"));
      repo.addMovie(repo.createMovie("Forrest Gump", "Drama", 1994, 2000000, "https://www.youtube.com/watch?v=bLvqoHBptjg"));
      repo.addMovie(repo.createMovie("The Godfather", "Crime", 1972, 2250000, "https://www.youtube.com/watch?v=sY1S34973zA"));
}


int main(int argc, char *argv[]) {
      QApplication a(argc, argv);
      testAll();

      std::string file_string_repo = "../Storage_files/admin_repo.db";

      auto repo = std::make_unique<DataBaseRepository>(file_string_repo);
      repo->clearRepository();
      populate_database(*repo);
      Service service(*repo);
      Gui gui(service);
      gui.show();

      return a.exec();
}
