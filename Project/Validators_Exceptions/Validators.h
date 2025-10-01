#pragma once
#include "../Repositories/Movies.h"



// The MovieValidator class is responsible for validating Movie objects.
class MovieValidator {
public:
      // Default constructor.
      MovieValidator() = default;

      // Validates the properties of a Movie object.
      // Throws an exception if any field is invalid (e.g., empty strings, negative year or likes).
      void validateMovie(const Movie& movie);
};