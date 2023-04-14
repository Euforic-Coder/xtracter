#ifndef MESSAGES_H
#define MESSAGES_H

#include <iostream>
#include <stdlib.h>

namespace uforia {

    // Prints a bulletin message
    void bulletin(std::string s, bool assert = true) {
        if (assert) {
            std::cout << "\033[32m • " << s << "\033[m" << std::endl;
        } else {
            std::cout << "\033[31m × " << s << "\033[m" << std::endl;
        }
    }

    // Prints a message
    void message(std::string s){
        std::cout << s << std::endl;
    }

    // Prints an error message
    void error(std::string s, bool exit = true) {
        std::cout << "\033[31m" << s << "\033[m" << std::endl;
        if (exit) {
            std::exit(0);
        }
    }

    // Prints a warning message
    void warning(std::string s){
        std::cout << "\033[33m" << s << "\033[m" << std::endl;
    }
}
#endif // MESSAGES_H