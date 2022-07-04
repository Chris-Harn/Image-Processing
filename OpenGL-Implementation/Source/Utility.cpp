#include "Utility.h"

#include <iostream>

void print_message( const char *message ) {
    std::cout << message << std::endl;
}

void print_error_message( const char *message ) {
    std::cerr << message << std::endl;
}