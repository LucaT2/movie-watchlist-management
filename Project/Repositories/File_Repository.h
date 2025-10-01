#pragma once
#include "Movies.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;
// Repository class handles storage and management of Movie objects.
class File_Repository {
private:
      // Internal dynamic vector to store Movie objects.
      vector<Movie> movies;
      string filename;


public:
      // Default constructor.
      File_Repository(const string& file) : filename(file) {
            readFile();
      }

      // Default destructor.
      ~File_Repository() = default;

      // Creates and returns a Movie object using the provided data.
      virtual Movie createMovie(const std::string& title,
                        const std::string& genre,
                        const int year,
                        const int likes,
                        const std::string& trailer);


      void readFile();

      void writeFile();

      virtual vector<Movie> getMoviesByGenre(const std::string& genre);

      //Movie fromString(string line);
      // Adds a new movie to the repository.
      virtual void addMovie(const Movie& movie);

      // Removes a movie identified by its title.
      virtual void removeMovie(const std::string& title);

      // Updates the movie with the given title using the new movie data.
      virtual void updateMovie(const std::string& title, const Movie& movie);

      // Increments the number of likes for the movie with the given title.
      virtual void incrementMovieLikes(const std::string& title);

      // Returns the movie at the specified position in the vector.
      virtual Movie getMovie(const int position) { return movies[position]; }

      // Returns the number of movies stored in the repository.
      virtual int getSize() { return movies.size(); }

      virtual vector<Movie> getAllMovies() { return movies; }

      virtual void clearRepository() {
            movies.clear();
            ofstream fout(filename);
            fout.close();
      };

      Movie getMovieByTitle(const std::string& title);;


};


