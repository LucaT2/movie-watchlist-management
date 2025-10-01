#include "Movies.h"
#include <typeinfo>
#include <stdexcept>

Movie::Movie(const std::string& title,
                const std::string& genre,
                const int year,
                const int likes,
                const std::string& trailer)
{
        this->trailer = trailer;
        this->genre = genre;
        this->title = title;
        this->likes = likes;
        this->year = year;
}

Movie::Movie()
{
        this->genre = "";
        this->title = "";
        this->trailer = "";
        this->likes = 0;
        this->year =0;
}



