#pragma once
#include <string>
#include <sstream>

// The Movie class represents a movie with title, genre, year, likes, and trailer URL.
class Movie {
private:
      std::string trailer; // URL to the movie trailer.
      int likes; // Number of likes the movie has.
      std::string title; // Title of the movie.
      std::string genre; // Genre of the movie.
      int year; // Release year of the movie.

public:
      // Constructor: Initializes a movie with the given details.
      Movie(const std::string &title,
            const std::string &genre,
            const int year,
            const int likes,
            const std::string &trailer);

      // Default constructor.
      Movie();

      // Increments the number of likes for the movie by one.
      void incrementLikes() { this->likes++; }

      // Returns the number of likes the movie has.
      int getLikes() const { return this->likes; }

      // Returns the title of the movie.
      std::string getTitle() const { return this->title; }

      // Returns the genre of the movie.
      std::string getGenre() const { return this->genre; }

      // Returns the release year of the movie.
      int getYear() const { return this->year; }

      // Returns the trailer URL of the movie.
      std::string getTrailer() const { return this->trailer; }

      void setTitle(const std::string &title) { this->title = title; }
      void setGenre(const std::string &genre) { this->genre = genre; }
      void setYear(int year) { this->year = year; }
      void setTrailer(const std::string &trailer) { this->trailer = trailer; }
      void setLikes(int likes) { this->likes = likes; }

      friend std::istream &operator>>(std::istream &is, Movie &movie) {
            std::string line;
            if (!std::getline(is, line)) return is;

            std::stringstream ss(line);
            std::string title, genre, strYear, strLikes, trailer;

            std::getline(ss, title, ',');
            std::getline(ss, genre, ',');
            std::getline(ss, strYear, ',');
            std::getline(ss, strLikes, ',');
            std::getline(ss, trailer);

            if (!title.empty() && !genre.empty() && !strYear.empty() && !strLikes.empty() && !trailer.empty()) {
                  movie = Movie(title, genre, std::stoi(strYear), std::stoi(strLikes), trailer);
            }

            return is;
      };

      friend std::ostream &operator<<(std::ostream &os, const Movie &movie) {
            os << movie.getTitle() << ","
                  << movie.getGenre() << ","
                  << movie.getYear() << ","
                  << movie.getLikes() << ","
                  << movie.getTrailer() << "\n";
            return os;
      };
};
