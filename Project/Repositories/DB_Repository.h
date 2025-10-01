#pragma once

#include "File_Repository.h";
#include "../sqlite3/sqlite3.h"
#include "../Validators_Exceptions/Exceptions.h"
#include "../Validators_Exceptions/Validators.h"

using namespace std;

class DataBaseRepository : public File_Repository {
public:
      // Constructor initializes the base Repository with a dummy file name
      DataBaseRepository(const string& dbFile = "movies.db");

      ~DataBaseRepository();

      void addMovie(const Movie& movie) override;
      void removeMovie(const string& title) override;
      void updateMovie(const string& title, const Movie& movie) override;
      void incrementMovieLikes(const string& title) override;
      std::vector<Movie> getMoviesByGenre(const string& genre) override;
      Movie getMovie(int position) override;
      int getSize() override;
      std::vector<Movie> getAllMovies() override;
      Movie createMovie(const string& title,
                        const string& genre,
                        int year,
                        int likes,
                        const string& trailer) override;
      void clearRepository() override;
private:
      sqlite3* db;
      std::string dbFilePath;
      void openDb();
      void closeDb();
      void ensureTable();
      void execute(const std::string& sql);
      sqlite3_stmt* prepare(const std::string& sql);
};