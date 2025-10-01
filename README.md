This is a C++, QT-based movie watchlist management app that has two main parts, Admin and User.

## ADMIN
This is the part that is responsible for what movies are accessible for the user to add in his watchlist.
## 🚀 Features
- Add, Remove and Update movies
- Undo and Redo
- Validations for every crud operations
- Sqlite database for storing the movies
- Unit tests for all operations
- There is also a QT Chart for representing all the movies in the database
## USER
The user can add, remove, filter by genre, open the watchlist in two different formats.
## 🚀 Features
- In order to select a movie, the user first has to filter by genre ( empty input for all genres)
- Whenever a movie is available to the user, the youtube trailer of that specific movie will play in the user's browser
- The watchlist is available in either CSV or HTML
- The user can iterate through the movies that belong to a specific genre
- Unit tests for all operations
## 🏗️ Techstack
- Qt6 (Widgets, ModelView framework)
- C++ 20
- Sqlite for admin database

