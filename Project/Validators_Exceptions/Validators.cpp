//
// Created by Filip on 28-Apr-25.
//

#include "Validators.h"
#include "Exceptions.h"
void MovieValidator::validateMovie(const Movie &movie){

      if(typeid(movie.getLikes()) != typeid(int) ||
               movie.getLikes() < 0){
            throw ValidatorException("Invalid movie likes");
               }
      else if(typeid(movie.getYear()) != typeid(int)
            || movie.getYear() < 0){
            throw ValidatorException("Invalid movie year");
            }

}