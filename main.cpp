#include <iostream>
#include <getopt.h>
#include <fstream>
#include "uforia/uforia.h"

#define DEBUG 1
#if DEBUG
#define LOG(x) cout << "Debug: " << x << endl
#else
#define LOG(x)
#endif

using namespace std;

ifstream file;
ofstream write;
string language;
vector<string> dictionary = from("english.dic", true);
vector<string> collection;
bool flag_file, flag_write;

void set_dictionary(string language){
    // TODO: Add languages
    if(language == "english"){
        dictionary = from("english.dic", false);
        LOG("Dictionary is set to English");
    }else if(language == "spanish"){
        dictionary == from("spanish.dic", false);
    }else{
        error("Wrong language is given", true);
    }
}

void extract(string password){
    for(int i = 0; i < dictionary.size(); ++i){
        string word = dictionary[i];
        if(has(word, password) && word.length() < password.length()){
            collection.push_back(word);
            LOG(word + " added to collection");
            password.erase(password.find(word), word.length());
            LOG(password);
        }
    }
    extract(password);
}

int main(int argc, char* argv[])
{
    int c;

    while ((c = getopt (argc, argv, "f:l:o:")) != -1)
        switch (c)
          {
          case 'f':
            file.open(optarg);
            flag_file = true;
            break;
          case 'l':
            language = optarg;
            break;
          case 'o':
            write.open(optarg);
            flag_write = true;
            break;
          case '?':
            if (optopt == 'c')
              fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint (optopt))
              fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            else
              fprintf (stderr,
                       "Unknown option character `\\x%x'.\n",
                       optopt);
            return 1;
          default:
            abort ();
          }

    return 0;
}
