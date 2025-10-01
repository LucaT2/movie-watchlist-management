#pragma once
#include "File_Repository.h"

// Service class manages movie operations and a user watch list.
class WatchList {
private:
      // Watch list storing movies the user added.
      vector<Movie> watch_list;

public:
      // Reference to the main repository storing all movies.

      // Constructor: Initializes the service with a reference to a repository.
      WatchList(){};

      // Destructor: Default, no dynamic memory cleanup needed.
      ~WatchList() = default;

      // Returns all movies of a given genre from the repository.
      // If genre is empty, returns all movies.

      // Adds a movie to the user's watch list.
      void addMovieToWatchList(const Movie& movie);

      // Removes the movie at the given position from the watch list.
      void removeMovieFromWatchList(int position);

      void removeByTitle(string title);

      // Returns the user's current watch list.
      vector<Movie>& getWatchList();

      // Opens the given URL in the default browser.
      // Typically used to open a movie trailer link.

};

class FileWatchList : public WatchList {
      protected:
      std::string filename;
      public:
      FileWatchList(const std::string& fileName);
      virtual void writeToWatchList() = 0;
      virtual void displayWatchList() = 0;
      virtual ~FileWatchList(){};
};

class CSVWatchList : public FileWatchList {
      public:
      CSVWatchList(const std::string& filename);
      virtual ~CSVWatchList(){};
      void writeToWatchList() override;
      void displayWatchList() override;
};

class HTMLWatchList : public FileWatchList {
      public:
      HTMLWatchList(const std::string& filename);
      virtual ~HTMLWatchList(){};
      void writeToWatchList() override;
      void displayWatchList() override;
};
