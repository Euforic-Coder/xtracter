#ifndef COLORED_H
#define COLORED_H

#include <iostream>

namespace uforia {

    enum Color {
        Red, Green, Blue, Yellow, Magenta, Cyan, Grey
    };
    enum Style {
        Regular, Bold, Slim, Italic, Underline
    };

    std::string colored(std::string s, Color color, Style style = Regular) {
        std::string suffix = "\033[", prefix = "\033[0m", str_color, str_style;

        // Set the color of the string
        switch (color) {
            case Red:
                str_color = "31m";
                break;
            case Green:
                str_color = "32m";
                break;
            case Yellow:
                str_color = "33m";
                break;
            case Blue:
                str_color = "34m";
                break;
            case Magenta:
                str_color = "35m";
                break;
            case Cyan:
                str_color = "36m";
                break;
            case Grey:
                str_color = "37m";
                break;

        }

        // Indicates the style of the sting
        switch (style) {
            case Regular:
                str_style = "0;";
                break;
            case Bold:
                str_style = "1;";
                break;
            case Slim:
                str_style = "2;";
                break;
            case Italic:
                str_style = "3;";
                break;
            case Underline:
                str_style = "4;";
                break;
        }

        std::string buffer = suffix + str_style + str_color + s + prefix;
        return buffer;
    }

    // Colorizes part of a string
    std::string highlight(const std::string &s, const std::string &other, Color color, Style style) {
        std::string buffer = other;
        if (s.length() <= other.length()) {
            buffer.replace(buffer.find(s), s.length(), colored(s, color, style));
        }
        return buffer;
    }
}
#endif // COLORED_H
