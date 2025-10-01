#include "DB_Repository.h"

DataBaseRepository::DataBaseRepository(const std::string& dbFile)
    : File_Repository("database_temp.txt"), dbFilePath(dbFile) {
    openDb();
    ensureTable();
}

DataBaseRepository::~DataBaseRepository() {
    closeDb();
}

void DataBaseRepository::openDb() {
    if (sqlite3_open(dbFilePath.c_str(), &db) != SQLITE_OK) {
        throw std::runtime_error("Could not open database");
    }
}

void DataBaseRepository::closeDb() {
    if (db) sqlite3_close(db);
}

void DataBaseRepository::ensureTable() {
    const std::string sql =
        "CREATE TABLE IF NOT EXISTS Movies ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "title TEXT UNIQUE NOT NULL,"
        "genre TEXT NOT NULL,"
        "year INTEGER NOT NULL,"
        "likes INTEGER NOT NULL,"
        "trailer TEXT NOT NULL"
        ");";
    execute(sql);
}

void DataBaseRepository::execute(const std::string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::string error = errMsg;
        sqlite3_free(errMsg);
        throw std::runtime_error("SQL error: " + error);
    }
}

sqlite3_stmt* DataBaseRepository::prepare(const std::string& sql) {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare statement");
    }
    return stmt;
}

void DataBaseRepository::addMovie(const Movie& movie) {
    MovieValidator validator;
    validator.validateMovie(movie);
    const std::string sql =
        "INSERT INTO Movies(title, genre, year, likes, trailer) VALUES(?,?,?,?,?);";
    auto stmt = prepare(sql);
    sqlite3_bind_text(stmt, 1, movie.getTitle().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, movie.getGenre().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, movie.getYear());
    sqlite3_bind_int(stmt, 4, movie.getLikes());
    sqlite3_bind_text(stmt, 5, movie.getTrailer().c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw RepositoryException("Movie already exists or insert failed");
    }
    sqlite3_finalize(stmt);
}

void DataBaseRepository::removeMovie(const std::string& title) {
    const std::string sql = "DELETE FROM Movies WHERE title = ?;";
    auto stmt = prepare(sql);
    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw RepositoryException("Title not found or delete failed");
    }
    sqlite3_finalize(stmt);
}

void DataBaseRepository::updateMovie(const std::string& title, const Movie& movie) {
    const std::string sql =
        "UPDATE Movies SET genre=?, year=?, likes=?, trailer=? WHERE title=?;";
    auto stmt = prepare(sql);
    sqlite3_bind_text(stmt, 1, movie.getGenre().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, movie.getYear());
    sqlite3_bind_int(stmt, 3, movie.getLikes());
    sqlite3_bind_text(stmt, 4, movie.getTrailer().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, title.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw RepositoryException("Title not found or update failed");
    }
    sqlite3_finalize(stmt);
}

void DataBaseRepository::incrementMovieLikes(const std::string& title) {
    const std::string sql =
        "UPDATE Movies SET likes = likes + 1 WHERE title = ?;";
    auto stmt = prepare(sql);
    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw RepositoryException("Title not found or increment failed");
    }
    sqlite3_finalize(stmt);
}

std::vector<Movie> DataBaseRepository::getMoviesByGenre(const std::string& genre) {
    const std::string sql = genre.empty()
        ? "SELECT title,genre,year,likes,trailer FROM Movies;"
        : "SELECT title,genre,year,likes,trailer FROM Movies WHERE genre = ?;";
    auto stmt = prepare(sql);
    if (!genre.empty()) sqlite3_bind_text(stmt, 1, genre.c_str(), -1, SQLITE_TRANSIENT);

    std::vector<Movie> result;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        result.emplace_back(
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
            sqlite3_column_int(stmt, 2),
            sqlite3_column_int(stmt, 3),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))
        );
    }
    sqlite3_finalize(stmt);
    return result;
}

Movie DataBaseRepository::getMovie(int position) {
    const std::string sql =
        "SELECT title,genre,year,likes,trailer FROM Movies ORDER BY id LIMIT 1 OFFSET ?;";
    auto stmt = prepare(sql);
    sqlite3_bind_int(stmt, 1, position);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        Movie m(
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
            sqlite3_column_int(stmt, 2),
            sqlite3_column_int(stmt, 3),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))
        );
        sqlite3_finalize(stmt);
        return m;
    }
    sqlite3_finalize(stmt);
    throw RepositoryException("Position out of range");
}

int DataBaseRepository::getSize() {
    const std::string sql = "SELECT COUNT(*) FROM Movies;";
    auto stmt = prepare(sql);
    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return count;
}

std::vector<Movie> DataBaseRepository::getAllMovies() {
    return getMoviesByGenre("");
}

Movie DataBaseRepository::createMovie(const std::string& title,
                                     const std::string& genre,
                                     int year,
                                     int likes,
                                     const std::string& trailer) {
    return Movie(title, genre, year, likes, trailer);
}
void DataBaseRepository::clearRepository() {
      const std::string sql = "DELETE FROM Movies;";
      execute(sql);
}