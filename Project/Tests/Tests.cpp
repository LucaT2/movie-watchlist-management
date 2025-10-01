#include "Tests.h"
#include <cassert>
#include <iostream>

#include "../Validators_Exceptions/Exceptions.h"
#include "../Repositories/WatchList.h"
#include "../Controller/Service.h"
using namespace std;
std::string file_string = "../Storage_files/test_repo.db";
void test_addMovie() {

      DataBaseRepository repo(file_string);
      repo.clearRepository();

      // Valid movie
      Movie m1 = repo.createMovie("Inception", "Sci-Fi", 2010, 2000000, "https://youtu.be/YoHD9XEInc0");
      assert(m1.getTitle() == "Inception");
      assert(m1.getGenre() == "Sci-Fi");
      assert(m1.getYear() == 2010);
      assert(m1.getLikes() == 2000000);

      assert(m1.getTrailer() == "https://youtu.be/YoHD9XEInc0");

      try {
            repo.addMovie(m1);
            assert(repo.getSize() == 1);
            repo.incrementMovieLikes("Inception");
            Movie incremented_m1 = repo.getMovie(0);
            assert(incremented_m1.getLikes() == 2000001);
      } catch (...) {
            cout << " Test failed: unexpected exception on valid movie\n";
      }
      m1 = repo.getMovie(0);

      // Try adding duplicate
      try {
            repo.addMovie(m1);  //  should throw
            std::cout << " Test failed: duplicate movie was added\n";
      } catch (const RepositoryException& msg) {
            //cout<<msg.what()<<endl;
            assert(string(msg.what()) == "Movie already exists or insert failed");
      }

      // Invalid movie (assuming validator throws on negative likes)
      Movie invalid = repo.createMovie("BadMovie", "Genre", 2020, -10, "http://link");

      try {
            repo.addMovie(invalid);  //  should throw from validator
      } catch (const ValidatorException& msg) {
            assert (string(msg.what()) == "Invalid movie likes");
      }
      Movie invalid1 = repo.createMovie("moviesae", "Genre", -2020, 10, "http://linkasdasd");
      try {
            repo.addMovie(invalid1);  //  should throw from validator
      } catch (const ValidatorException& msg) {
            //cout<<msg.what()<<endl;
            assert(string(msg.what()) == "Invalid movie year");
      }
      cout<<"Test add successful"<<endl;
}

void test_remove() {
      DataBaseRepository repository(file_string);
      repository.clearRepository();
      Movie movie1 = repository.createMovie("Movie1", "Genre1", 10, 10, "https://youtu.be/YoHD9XEInc1");
      Movie movie2 = repository.createMovie("Movie2", "Genre2", 10, 10, "https://youtu.be/YoHD9XEInc1");
      repository.addMovie(movie1);
      repository.addMovie(movie2);
      repository.removeMovie("Movie1");
      assert(repository.getSize() == 1);
      try {
            repository.removeMovie("Movie1");
      }
      catch (const RepositoryException& e) {
            assert(string(e.what()) == "Title not found");
      }
      cout<<"Test remove successful"<<endl;

}

void test_update() {
      DataBaseRepository repository(file_string);
      repository.clearRepository();
      Movie movie = repository.createMovie("movie", "genre", 1, 1, "http");
      Movie movie_updated = repository.createMovie("movie", "genre_updated", 10 ,10 ,"trailer_updated");
      repository.addMovie(movie);
      repository.updateMovie("movie", movie_updated);
      try {
            repository.updateMovie("movie1412", movie);
      }
      catch (const RepositoryException& msg) {
            assert(string(msg.what()) == "Title not found");
      }
      repository.clearRepository();
      cout<<"Test updated successful"<<endl;

}

void test_service() {
      DataBaseRepository repo(file_string);
      repo.readFile();
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

      std::unique_ptr<HTMLWatchList> watch_list_uptr = std::make_unique<HTMLWatchList>("Tests_watchlist.html");
      Service service(repo);
      service.setWatchList(watch_list_uptr.release());
      vector<Movie> movies = service.getMoviesByGenre("Drama");
      assert (movies[0].getTitle() == "The Shawshank Redemption");
      assert (movies[1].getTitle() == "Fight Club");
      assert (movies[2].getTitle() == "Forrest Gump");
      service.addMovieToWatchList(movies[1]);
      assert(service.getWatchList().size() == 1);
      //service.openURL(movies[0].getTrailer());

      service.removeMovieFromWatchList(0);
      assert(service.getWatchList().size() == 0);
      repo.clearRepository();
      cout<<"Test service successful"<<endl;
}

void testAll() {

      test_addMovie();
      test_remove();
      test_update();
      test_service();
      printf("All tests passed\n");

}