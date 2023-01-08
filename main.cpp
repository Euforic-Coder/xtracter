#include <iostream>
#include <getopt.h>
#include <fstream>
#include "uforia/uforia.h"

#define DEBUG 0
#if DEBUG
#define LOG(x) cout << "Debug: " << x << endl
#define COMPARE(x, y) cout << colored(x, Red, Italic) << " " << colored(y, Blue, Italic) << endl
#else
#define LOG(x)
#endif

using namespace std;

int maximal_chars = 12, minimal_chars = 4;
ifstream file;
ofstream write;
string language;
vector<string> dictionary = from("english.dic", true);
vector<string> collection;
bool flag_file, flag_write, flag_verbose;

// Set the dictionary language
void set_dictionary(string language){
    // TODO: Add languages
    if(language == "english"){
        dictionary = from("english.dic", true);
        LOG("Dictionary is set to English");
    }else if(language == "spanish"){
        dictionary = from("spanish.dic", false);
    }else{
        error("Wrong language is given", true);
    }
}

// Extract words from the password
int index = 0;
void extract(string password){
    for(unsigned short int i = 0; i < dictionary.size(); ++i){
        string word = dictionary[i];
	if(has(word, password) && word.length() < password.length() && word.length() <= maximal_chars && word.length() >= minimal_chars){
            collection.push_back(word);

            // Print words in 6 columns
            if(flag_verbose){
                if(index == 6){
                    cout << colored(word, Green, Bold) << endl;
                    index = 0;
                }else{
                    cout << spacer(colored(word, Green, Bold), 30);
                    index++;
                }
            }

            // Write words to file
            if(flag_write){
                write << word << endl;
            }

            // The found word is removed from the password and reused
            password.erase(password.find(word), word.length());
            COMPARE(word, password);
            extract(password);
        }
    }
}

void help(){
    HelpMaker help("xtracter");
    help.add_usage("xtracter -f <FILE> -o <FILE> [OPTIONS]");
    help.add_argument("-f", "Define the password file");
    help.add_argument("-o", "Define the write file");
    help.add_argument("-l", "Define the language");
    help.add_argument("-m", "Define the minimal length of characters");
    help.add_argument("-M", "Define the maximal length of characters");
    help.add_argument("-v", "Set verbosity");
    help.add_example("xtracter -f rockyou.txt -o words.txt -l english -m 4 -M 10 -v");
    help.print();
}

int main(int argc, char* argv[])
{
    if(argc < 2){
        help();
    }

    // Getopt
    int c;

    while ((c = getopt (argc, argv, "f:l:o:m:M:v")) != -1)
        switch (c)
        {
        case 'f':
            file.open(optarg);
            flag_file = true;
            LOG(optarg);
            break;
        case 'l':
            language = optarg;
            LOG(optarg);
            break;
        case 'o':
            write.open(optarg);
            flag_write = true;
            LOG(optarg);
            break;
        case 'm':
            minimal_chars = stoi(optarg);
            break;
        case 'M':
            maximal_chars = stoi(optarg);
            break;
        case 'v':
            flag_verbose = true;
            break;
        default:
            abort();
        }

    // Assert if the dictionary is open
    if(dictionary.empty()){
        error("Failed to open the dictionary", true);
    }

    // Extract words
    string password;
    while(file >> password){
        extract(lowercase(password));
    }

    return 0;
}
