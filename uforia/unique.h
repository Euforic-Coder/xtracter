#ifndef UNIQUE_H
#define UNIQUE_H

#include <vector>

namespace uforia {

    // Makes a vector<string> unique
    std::vector <std::string> unique(std::vector <std::string> vec) {
        std::vector <std::string> buffer;
        for (unsigned int i = 0; i < vec.size(); ++i) {
            bool in_buffer = false;
            const std::string &line = vec[i];

            // If the buffer is empty, the first string will be added
            if (i == 0) {
                buffer.push_back(line);
            }

            for (unsigned int j = 0; j < buffer.size(); ++j) {
                const std::string &buffered = buffer[j];

                // If the line is already present, in_buffer will be flagged
                if (line == buffered) {
                    in_buffer = true;
                    break;
                }
            }

            // If the line is not present, the string will be added
            if (!in_buffer) {
                buffer.push_back(line);
            }
        }
        return buffer;
    }
}

#endif // UNIQUE_H
