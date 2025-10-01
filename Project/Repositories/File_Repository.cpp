#include "File_Repository.h"
#include <filesystem>
#include "../Validators_Exceptions/Validators.h"
#include "../Validators_Exceptions/Exceptions.h"
//#include <stdexcept>
using namespace std;


void File_Repository::readFile() {

      ifstream fin(filename);
      //std::cout << "Full path: " << std::filesystem::absolute(filename) << "\n";

      string line;
      Movie movie;
      while (fin>>movie) {
                  movies.push_back(movie);

      }
      fin.close();
}

void File_Repository::writeFile() {
      std::ofstream fout(filename);

      for (const auto &movie : movies) {
            fout << movie;
      }

      fout.close();
}


void File_Repository::addMovie(const Movie& movie){
  MovieValidator validator;
  validator.validateMovie(movie);
  auto it = find_if(movies.begin(), movies.end(),
        [&](const Movie& movie_to_add){return movie_to_add.getTitle() == movie.getTitle();});
      if (it != movies.end()) {
            throw RepositoryException("Movie already exists");
      }
  this->movies.push_back(movie);
      writeFile();

}
vector<Movie> File_Repository::getMoviesByGenre(const string& genre) {
      vector<Movie> list_to_return;
      for (auto it : this->getAllMovies()){
            if (it.getGenre() == genre || genre == "") {
                  list_to_return.push_back(it);
            }
      }
      return list_to_return;}
void File_Repository::removeMovie(const string& Title){
      bool found = false;
      auto it = find_if(movies.begin(), movies.end(),
            [&](const Movie& movie){return movie.getTitle() == Title;});

      if (it != movies.end()) {
            found = true;
            movies.erase(it);
            writeFile();
      }
      if (!found){
            throw RepositoryException("Title not found");
  }
}

void File_Repository::updateMovie(const string& Title, const Movie& movie){
      bool found = false;
      auto it = find_if(movies.begin(), movies.end(),
                [&](const Movie& movie){return movie.getTitle() == Title;});
      if (it != movies.end()) {
            found = true;
            it->setGenre(movie.getGenre());
            //it->setTitle(movie.getTitle());
            it->setYear(movie.getYear());
            it->setLikes(movie.getLikes());
            it->setTrailer(movie.getTrailer());
            writeFile();
      }
      if (!found){
            throw RepositoryException("Title not found");
      }
}

Movie File_Repository::createMovie(const std::string& title,const std::string& genre,const int year,const int likes,const std::string& trailer){
        Movie movie;
        movie = Movie(title, genre, year, likes, trailer);
        return movie;}

void File_Repository::incrementMovieLikes(const std::string& title) {
      auto it = find_if(movies.begin(), movies.end(),
                [&](const Movie& movie){return movie.getTitle() == title;});
      if (it != movies.end()) {
            it->incrementLikes();
            writeFile();
      }
}

Movie File_Repository::getMovieByTitle(const std::string &title) {
      for (auto mv: movies) {
            if (mv.getTitle() == title) {
                  return mv;
            }
      }
      return Movie{};
}


