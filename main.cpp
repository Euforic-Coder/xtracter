#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <getopt.h>
#include "uforia/uforia.h"

using namespace std;
using namespace uforia;

string temp;
vector<string> list, buffer, additional, removal;
ofstream write;
int minimal_chars = 0, maximal_chars = 255, min_count = 1;
bool flag_file = false, flag_write = false;
bool flag_sort_ascending = false, flag_sort_descending = false, flag_sort_length_ascending = false, flag_sort_length_descending = false,
        flag_filter_char = false, flag_add = false, flag_remove = false, flag_digital_include = false, flag_digital_exclude = false,
        flag_lowercase = false, flag_capitalize = false, flag_include = false, flag_exclude = false, flag_unique = false, flag_count = false,
        flag_verbose = false;
void help() {
    HelpMenu help("lister");
    help.add_usage("lister -f <FILE> -o <FILE> -a <FILE> -A <FILE> [OPTIONS]");
    help.add_option("-f", "Define working file", true);
    help.add_option("-o", "Define output file", true);
    help.add_option("-m", "Define minimal characters per line", false);
    help.add_option("-M", "Define maximal characters per line", false);
    help.add_option("-a", "Add lines from another file", false);
    help.add_option("-A", "Remove lines from another file", false);
    help.add_option("-s", "Sort lines alphabetically ascending", false);
    help.add_option("-S", "Sort lines alphabetically descending", false);
    help.add_option("-u", "Make lines unique", false);
    help.add_option("-U", "Sort lines by count", false);
    help.add_option("-w", "Define the minimal count per line to include", true);
    help.add_option("-i", "Include lines containing given string", false);
    help.add_option("-I", "Exclude lines containing given string", false);
    help.add_option("-c", "Set lines to lowercase", false);
    help.add_option("-C", "Capitalize lines", false);
    help.add_option("-d", "Include lines containing digital characters", false);
    help.add_option("-D", "Exclude lines containing digital characters", false);
    help.add_option("-v", "Verbose output", false);
    help.add_option("-h", "Prints helpmenu", false);
    help.print();
}

int main(int argc, char *argv[]) {
    // Print help if there is no argument
    if (argc < 2) {
        help();
        return 0;
    }

    int c;

    while ((c = getopt(argc, argv, "f:o:m:M:a:A:sSuUw:lLi:I:cCdD")) != -1)
        switch (c) {
            case 'f':
                flag_file = true;
                list = from(optarg, true);
                break;
            case 'o':
                flag_write = true;
                write.open(optarg);
                break;
            case 'm':
                flag_filter_char = true;
                if (is_numeric(optarg)) {
                    minimal_chars = stoi(optarg);
                } else {
                    error("Wrong argument -m", true);
                }
                break;
            case 'M':
                flag_filter_char = true;
                if (is_numeric(optarg)) {
                    maximal_chars = stoi(optarg);
                } else {
                    error("Wrong argument -M", true);
                }
                break;
            case 'a':
                flag_add = true;
                additional = from(optarg, true);
                break;
            case 'A':
                flag_remove = true;
                removal = from(optarg, true);
            case 's':
                flag_sort_ascending = true;
                break;
            case 'S':
                flag_sort_descending = true;
                break;
            case 'u':
                flag_unique = true;
                break;
            case 'U':
                flag_count = true;
                break;
            case 'w':
            min_count = stoi(optarg);
            break;
            case 'l':
                flag_sort_length_ascending = true;
                break;
            case 'L':
                flag_sort_length_descending = true;
                break;
            case 'i':
                flag_include = true;
                temp = optarg;
                break;
            case 'I':
                flag_exclude = true;
                temp = optarg;
                break;
            case 'c':
                flag_lowercase = true;
                break;
            case 'C':
                flag_capitalize = true;
                break;
            case 'd':
                flag_digital_include = true;
                break;
            case 'D':
                flag_digital_exclude = true;
                break;
            case 'h':
                help();
                break;
        }

    if (flag_file) {

        // Add lines to list
        if (flag_add) {
            if(flag_verbose) {
                message("Adding lines from file");
            }
            for (int i = 0; i < additional.size(); ++i) {
                string line = additional[i];
                list.push_back(line);
            }
        }

        // Remove lines from a list
        if (flag_remove) {
            if(flag_verbose) {
                message("Removing lines from file");
            }
            for (int i = 0; i < list.size(); ++i) {
                string line = list[i];
                for (int j = 0; j < removal.size(); ++j) {
                    string remove = removal[j];
                    if (line == remove) {
                        list.erase(list.begin() + i);
                    }
                }
            }
        }

        // Filter lines by minimal and maximal char count
        if (flag_filter_char) {
            if(flag_verbose) {
                message("Filtering lines by line length");
            }
            for (int i = 0; i < list.size(); ++i) {
                string line = list[i];
                if (line.length() <= maximal_chars && line.length() >= minimal_chars) {
                    buffer.push_back(line);
                }
            }
            list = buffer;
            buffer.clear();
        }

        // Sort lines by count
        if (flag_count) {
            if(flag_verbose) {
                message("Sorting lines by count");
            }
            buffer = uforia::count(list, min_count);
            list = buffer;
            buffer.clear();
        }
        // Make lines unique
        if (flag_unique) {
            if(flag_verbose) {
                message("Making lines unique");
            }
            buffer = uforia::unique(list);
            list = buffer;
            buffer.clear();
        }

        // Sort list alphabetically ascending
        if (flag_sort_ascending) {
            if(flag_verbose) {
                message("Sorting lines");
            }
            sort(list.begin(), list.end());
        }

        // Sort list alphabetically descending
        if (flag_sort_descending) {
            if(flag_verbose) {
                message("Sorting lines");
            }
            sort(list.begin(), list.end(), [](string a, string b) {
                return a > b;
            });
        }

        // Sort list by length ascending
        if (flag_sort_length_ascending) {
            if(flag_verbose) {
                message("Sorting lines by line length");
            }
            sort(list.begin(), list.end(), [](string a, string b) {
                return a.length() < b.length();
            });
        }

        // Sort list by length descending
        if (flag_sort_length_descending) {
            if(flag_verbose) {
                message("Sorting lines by line length");
            }
            sort(list.begin(), list.end(), [](string a, string b) {
                return a.length() > b.length();
            });
        }

        // Include lines containing given string
        if (flag_include) {
            if(flag_verbose) {
                message("Filtering lines by inclusion");
            }
            for (int i = 0; i < list.size(); ++i) {
                string line = list[i];
                if (has(temp, line)) {
                    buffer.push_back(line);
                }
            }
            list = buffer;
            buffer.clear();
        }

        // Exclude lines containing given string
        // TODO: Fix bug that still includes whole matches
        if (flag_exclude) {
            if(flag_verbose) {
                message("Filtering lines by exclusion");
            }
            for (int i = 0; i < list.size(); ++i) {
                string line = list[i];
                if (!has(temp, line)) {
                    buffer.push_back(line);
                }
            }
            list = buffer;
            buffer.clear();
        }

        // Include lines containing digital characters
        if (flag_digital_include) {
            if(flag_verbose) {
                message("Filtering lines if digital");
            }
            for (int i = 0; i < list.size(); ++i) {
                string line = list[i];
                if (has_number(line)) {
                    buffer.push_back(line);
                }
            }
            list = buffer;
            buffer.clear();
        }

        // Exclude lines containing digital characters
        if (flag_digital_exclude) {
            if(flag_verbose) {
                message("Filtering lines if non-digital");
            }
            for (int i = 0; i < list.size(); ++i) {
                string line = list[i];
                if (!has_number(line)) {
                    buffer.push_back(line);
                }
            }
            list = buffer;
            buffer.clear();
        }

        // Set lines to lowercase
        if (flag_lowercase) {
            if(flag_verbose) {
                message("Making lines lowercase");
            }
            for (int i = 0; i < list.size(); ++i) {
                string line = list[i];
                buffer.push_back(lowercase(line));
            }
            list = buffer;
            buffer.clear();
        }

        // Capitalize lines
        if (flag_capitalize) {
            if(flag_verbose) {
                message("Capitalizing lines");
            }
            for (int i = 0; i < list.size(); ++i) {
                string line = list[i];
                buffer.push_back(capitalized(line));
            }
            list = buffer;
            buffer.clear();
        }

        // Write to file
        if (flag_write) {
            if(flag_verbose) {
                message("Writing lines to file");
            }
            for (int i = 0; i < list.size(); ++i) {
                string line = list[i];
                write << line << endl;
            }
            write.close();
        }

        // Print lines
        for (int i = 0; i < list.size(); ++i) {
            string line = list[i];
            cout << line << endl;
        }

    } else {
        error("No file given (-f)", true);
    }

    return 0;
}
