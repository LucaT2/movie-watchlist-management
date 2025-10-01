#include "WatchList.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include "../Validators_Exceptions/Exceptions.h"
#include <qurl.h>
#include <qdesktopservices.h>
#include <qstring.h>
#include <QProcess>
#include <QStandardPaths>
using namespace std;


void WatchList::addMovieToWatchList(const Movie& movie) {
      try {
            auto it = find_if(watch_list.begin(), watch_list.end(),
                      [&](const Movie& watched_movie){
                          return movie.getTitle() == watched_movie.getTitle();
                      });
            if (it != watch_list.end()) {
                  throw ValidatorException("Movie already exists");
            }
            this->watch_list.push_back(movie);
      } catch (const std::bad_alloc& e) {
            throw ValidatorException("Failed to allocate memory for new movie");
      }

}
void WatchList::removeMovieFromWatchList(int position) {

      for (size_t i = position; i + 1 < watch_list.size(); ++i) {
            watch_list[i] = std::move(watch_list[i+1]);
      }
      watch_list.pop_back();
}




vector<Movie>& WatchList::getWatchList() {
      return this->watch_list;
}

FileWatchList::FileWatchList(const std::string& fileName):WatchList(),
      filename{fileName}{}

CSVWatchList::CSVWatchList( const std::string &fileName):FileWatchList(fileName){}

void CSVWatchList::writeToWatchList() {
      ofstream fout{filename};
      for (Movie& movie : this->getWatchList()) {
            fout << movie;
      }
      fout.close();
}

void CSVWatchList::displayWatchList() {
      std::string command = "cmd.exe /C start \"\" " + filename;

      FILE* pipe = popen(command.c_str(), "r");
      if (pipe) pclose(pipe);

}

HTMLWatchList::HTMLWatchList(const std::string &fileName):FileWatchList(fileName){}

void HTMLWatchList::writeToWatchList() {
      std::ofstream fout(filename);
      if (!fout.is_open()) return;

      fout << "<!DOCTYPE html>\n<html>\n<head>\n<title>Watchlist</title>\n</head>\n<body>\n";
      fout << "<h1>My Movie Watchlist</h1>\n";
      fout << "<table border=\"1\">\n";
      fout << "<tr><th>Title</th><th>Genre</th><th>Year</th><th>Likes</th><th>Trailer</th></tr>\n";

      for (const Movie& movie : this->getWatchList()) {
            fout << "<tr>\n";
            fout << "<td>" << movie.getTitle() << "</td>\n";
            fout << "<td>" << movie.getGenre() << "</td>\n";
            fout << "<td>" << movie.getYear() << "</td>\n";
            fout << "<td>" << movie.getLikes() << "</td>\n";
            fout << "<td><a href=\"" << movie.getTrailer() << "\">Link</a></td>\n";
            fout << "</tr>\n";
      }

      fout << "</table>\n</body>\n</html>\n";
      fout.close();
}



void HTMLWatchList::displayWatchList() {
      std::string command = "cmd.exe /C start \"\" \"" + filename + "\"";
      FILE* pipe = _popen(command.c_str(), "r");
      if (pipe) {
            _pclose(pipe);
      }
}

