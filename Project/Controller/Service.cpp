//
// Created by Filip on 28-Apr-25.
//

#include "Service.h"


void Service::undo() {
      if (undoStack.empty()) {
            return;
      }
      unique_ptr<Action> a = move(undoStack.back());
      undoStack.pop_back();
      a->executeUndo();
      redoStack.push_back(move(a));
}

void Service::redo() {
      if (redoStack.empty())
            return;
      unique_ptr<Action> a = move(redoStack.back());
      redoStack.pop_back();
      a->executeRedo();
      undoStack.push_back(move(a));
}

void Service::addMovie(const Movie &movie) {
      repo.addMovie(movie);
      unique_ptr<Action>act = make_unique<ActionAdd>(movie, repo);
      undoStack.push_back(move(act));
      redoStack.clear();
}

void Service::removeMovie(const string &title) {
      Movie movie = repo.getMovieByTitle(title);
      repo.removeMovie(title);
      unique_ptr<Action>act = make_unique<ActionRemove>(movie, repo);
      undoStack.push_back(move(act));
      redoStack.clear();

}

void Service::updateMovie(const string &title, const Movie &movie) {
      Movie old_movie = repo.getMovieByTitle(title);
      undoStack.push_back(make_unique<ActionUpdate>(old_movie, movie, repo));
      redoStack.clear();
      repo.updateMovie(title, movie);
}

void Service::openURL(const std::string& url) {
      //cout<<"Opening URL: "<<url<<endl;
      std::string command = "cmd.exe /C start \"\" " + url;

      FILE* pipe = popen(command.c_str(), "r");
      if (pipe) pclose(pipe);
}

std::map<std::string,int> Service::countMoviesByGenre() {
      std::map<std::string,int> cnt;
      for(auto &m : getRepo())
            ++cnt[m.getGenre()];
      return cnt;
}