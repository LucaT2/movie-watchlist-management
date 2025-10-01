#pragma once
#include "../Repositories/File_Repository.h"
#include "../Repositories/WatchList.h"
#include "../Repositories/DB_Repository.h"
#include <map>
#include <memory>

using namespace std;

class Action {
public:
      virtual void executeUndo() = 0;
      virtual void executeRedo() = 0;
      virtual ~Action() = default;
};

class ActionAdd: public Action {
private:
      Movie addedMovie;
      DataBaseRepository& repo;
public:
      ActionAdd(const Movie& movie, DataBaseRepository& _repo):addedMovie(movie), repo(_repo) {};
      void executeUndo() override {
            repo.removeMovie(addedMovie.getTitle());
      };
      void executeRedo() override{
            repo.addMovie(addedMovie);
      };

};

class ActionRemove: public Action {
private:
      Movie removedMovie;
      DataBaseRepository& repo;
public:
      ActionRemove(const Movie& movie, DataBaseRepository& _repo):removedMovie(movie), repo(_repo) {};
      void executeUndo() override {
            repo.addMovie(removedMovie);
      };
      void executeRedo() override{
            repo.removeMovie(removedMovie.getTitle());
      };
};

class ActionUpdate: public Action {
private:
      Movie oldMovie;
      Movie newMovie;
      DataBaseRepository& repo;
public:
      ActionUpdate(const Movie& old, const Movie& newm, DataBaseRepository& _repo):oldMovie(old), newMovie(newm) ,repo(_repo) {};
      void executeUndo() override {
            repo.updateMovie(newMovie.getTitle(), oldMovie);
      }
      void executeRedo() override {
            repo.updateMovie(oldMovie.getTitle(), newMovie);
      }

};

class Service {
private:
      DataBaseRepository& repo;
      std::unique_ptr<FileWatchList> watchList = nullptr;
      vector<unique_ptr<Action>> undoStack;
      vector<unique_ptr<Action>> redoStack;

public:

      Service(DataBaseRepository& r): repo(r){}

      void undo();;
      void redo();;

      void setWatchList(FileWatchList* wl){     watchList = std::unique_ptr<FileWatchList>(wl); };
      void addMovie(const Movie &movie);;

      void removeMovie(const string& title);;

      void updateMovie(const string& title, const Movie& movie);;

      vector<Movie> getMoviesByGenre(const string& genre){return repo.getMoviesByGenre(genre);};

      void openURL(const std::string& url);

      void removeMovieByTitle(string title);

      void addMovieToWatchList(const Movie& movie) {watchList->addMovieToWatchList(movie);};

      void removeMovieFromWatchList(int number){watchList->removeMovieFromWatchList(number);};

      vector<Movie> getWatchList(){return this->watchList->getWatchList();};

      void incrementMovieLikes(const string& title){repo.incrementMovieLikes(title);};

      int getSize(){return repo.getSize();};

      void saveFile(){watchList->writeToWatchList();};

      void displayFile(){watchList->displayWatchList();};

      vector<Movie> getRepo(){return repo.getAllMovies();};

      std::map<std::string,int> countMoviesByGenre();

      Movie getMovie(int i) {
            return repo.getMovie(i);
      }

};