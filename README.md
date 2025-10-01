This is a  C++ application built with the Qt6 framework for managing a movie database and a personal user watchlist. The app operates in two distinct modes: Administrator and User.

## 👨‍💼 ADMIN Mode
This is the part that is responsible for what movies are accessible for the user to add in his watchlist.
## 🚀 Features
- **CRUD Operations:** Add, remove, and update movies in the database.
- **Undo/Redo:** Available for all CRUD operations.
- **Robust Validation:** All movie data is validated before being saved.
- **Data Visualization:** A dynamic Qt Chart displays a breakdown of movies by genre, available in the initial window.
- **SQLite Database:** All movies are stored in a SQLite Database for persistence.
- **Unit Tested:** Core functionalities are verified with comprehensive unit tests.
## 🎬  USER Mode
The user can browse the movie database, manage their personal watchlist, and view the watchlist in two different formats.
## 🚀 Features
- **Genre Filtering:** Browse all movies or filter them by a specific genre.
- **Trailer Previews:** Automatically opens the movie's YouTube trailer in a browser when it is selected.
- **Iterative Browsing:**  Cycle through movies one by one.
- **Watchlist Management:** Add or remove any movie from a personal watchlist.
- **Multiple Viewing Options:** Save and open the watchlist as either a CSV or an HTML file.
- **Unit Tested:** All user operations are covered by unit tests.

## 🏗️ Techstack
- **C++ 20**
- **Qt6 Framework** (Widgets, Charts, Model/View programming)
- **SQLite** for the database backend
