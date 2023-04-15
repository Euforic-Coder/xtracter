#include <iostream>
#include <getopt.h>
#include "uforia/uforia.h"

using namespace std;
using namespace uforia;

int maximal_chars = 12, minimal_chars = 6;
ifstream file;
ofstream write, unextracted;
vector<string> dictionary = from("/etc/xtracter/english.dic", true);
vector<string> collection, temporary;
bool flag_file = false, flag_write = false, flag_unextracted = false, flag_verbose = false;

// Set language of the dictionary
void set_language(const string &language) {
    if (language == "english") {
        dictionary = from("/etc/xtracter/english.dic", true);
    } else if (language == "dutch") {
        dictionary = from("/etc/xtracter/dutch.dic", true);
    } else if (language == "spanish") {
        dictionary = from("/etc/xtracter/spanish.dic", true);
    } else if (language == "french") {
        dictionary = from("/etc/xtracter/french.dic", true);
    } else if (language == "german") {
        dictionary = from("/etc/xtracter/german.dic", true);
    } else {
        error("Wrong language given (-l)", true);
    }
}

// Extract words from the password
int index = 0;
void extract(string password) {
    string unextract;
    for (unsigned short int i = 0; i < dictionary.size(); ++i) {
        const string &word = dictionary[i];
        if ((word.length() <= maximal_chars && word.length() >= minimal_chars && password == word)
            || (has(word, password) && word.length() < password.length() &&
            word.length() <= maximal_chars && word.length() >= minimal_chars)) {

            collection.push_back(word);

            // Print words in 7 columns
            if (flag_verbose) {
                if (index == 6) {
                    cout << colored(word, Green, Bold) << endl;
                    index = 0;
                } else {
                    cout << spacer(colored(word, Green, Bold), 30);
                    index++;
                }
            }

            // The found word is removed from the password and reused
            password.erase(password.find(word), word.length());
            extract(password);
        } else if (flag_unextracted) {
            unextract = password;
        }
    }
    if (flag_unextracted && !unextract.empty()) {
        unextracted << password << endl;
    }
}

// Help menu
void help(){
    HelpMenu help("xtracter");
    help.add_usage("xtracter -f <FILE> -o <FILE> [OPTIONS]");
    help.add_option("-f\t--file", "Define the password file", true);
    help.add_option("-o\t--write", "Define the write file", true);
    help.add_option("-m\t--minimal", "Define the minimal length of characters", false);
    help.add_option("-M\t--maximal", "Define the maximal length of characters", false);
    help.add_option("-u\t--unextracted", "Define the write file for un-extracted passwords", false);
    help.add_option("-l\t--language", "Define the language of the dictionary", false);
    help.add_option("-v\t--verbose", "Verbose output", false);
    help.add_option("-h\t--help", "Show help menu", false);
    help.add_example("xtracter -f rockyou.txt -o words.txt -m 4 -M 10");
    help.print();
    exit(0);
}

int main(int argc, char *argv[]) {
    int opt;
    int option_index;

    // Define the long options
    static struct option long_options[] = {
            {"file",        required_argument, nullptr, 'f'},
            {"write",       required_argument, nullptr, 'o'},
            {"minimal",     required_argument, nullptr, 'm'},
            {"maximal",     required_argument, nullptr, 'M'},
            {"language",    required_argument, nullptr, 'l'},
            {"unextracted", required_argument, nullptr, 'u'},
            {"verbose",     no_argument,       nullptr, 'v'},
            {"help",        no_argument,       nullptr, 'h'},
            {nullptr,       0,                 nullptr, 0}
    };

    while ((opt = getopt_long(argc, argv, "", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'f':
                file.open(optarg);
                if(file.good()){
                    flag_file = true;
                }else{
                    error("Wrong file given [-f | --file]", true);
                }
                break;
            case 'o':
                write.open(optarg, ios_base::trunc);
                flag_write = true;
                break;
            case 'm':
                minimal_chars = stoi(optarg);
                break;
            case 'M':
                maximal_chars = stoi(optarg);
                break;
            case 'l':
                set_language(optarg);
                break;
            case 'u':
                unextracted.open(optarg, ios_base::trunc);
                flag_unextracted = true;
                break;
            case 'v':
                flag_verbose = true;
                break;
            case 'h':
                help();
                break;
            default:
                exit(0);
        }
    }

    // Checks if a file is defined
    if(!flag_file){
        error("No file given (-f)", true);
    }

    // Checks if the dictionary is empty
    if(dictionary.empty()){
        error("Failed to open the dictionary", true);
    }


    // Extract the words
    string password;
    while(file >> password){
        extract(lowercase(password));
    }
    cout << endl;
    file.close();
    if(flag_unextracted){
        unextracted.close();
    }

    // Count the words
    temporary = uforia::count(collection);

    // Makes the words unique
    collection = uforia::unique(temporary);

    // Write to file and print
    for(int i = 0; i < collection.size(); ++i){
        string collected = collection[i];
        if(flag_write){
            write << collected << endl;
        }
        cout << collected << endl;
    }

    if(flag_write){
        write.close();
    }

    return 0;
}