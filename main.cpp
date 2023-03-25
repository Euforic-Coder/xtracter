#include <iostream>
#include <getopt.h>
#include <fstream>
#include <algorithm>
#include <chrono>
#include "uforia/uforia.h"

using namespace std;

int maximal_chars = 12, minimal_chars = 6;
ifstream file;
ofstream write;
ofstream unextracted;
string buffer;
vector<string> dictionary = from("/etc/xtracter/english.dic", true);
vector<string> collection;
bool flag_file = false, flag_write = false, flag_unextracted = false, flag_verbose = true;

// Extract words from the password
int index = 0;
void extract(string password){
    string unextract;
    for(unsigned short int i = 0; i < dictionary.size(); ++i){
        const string& word = dictionary[i];
        if((word.length() <= maximal_chars && word.length() >= minimal_chars && password == word)
                || (has(word, password) && word.length() < password.length() && word.length()
                    <= maximal_chars && word.length() >= minimal_chars)){

            collection.push_back(word);

            // Print words in 7 columns
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
            extract(password);
        }else if(flag_unextracted){
            unextract = password;
        }
    }
    if(flag_unextracted && !unextract.empty()){
        unextracted << password << endl;

    }
}

struct Extract{
    Extract(string s){
        this->word = s;
    }
    string word;
    int count = 1;
};

vector<Extract> extracted;
void count(){
    for(unsigned short i = 0; i < collection.size(); i++){
        const string collect = collection[i];

        // Add the first extract
        if(extracted.empty()){
            Extract extract(collect);
            extracted.push_back(extract);
            continue;
        }

        // Check if te collected word is present in extracted
        for(unsigned short j = 0; j < extracted.size(); ++j){
            const Extract extract = extracted[j];
            if(extract.word == collect){
                int c = extract.count;
                Extract e(collect);
                e.count = c + 1;
                extracted.erase(extracted.begin() + j);
                extracted.emplace_back(e);
            }
        }

        // Add an Extract to extracted
        Extract e(collect);
        extracted.push_back(e);
    }

    // Sort extracted by frequence
    sort(extracted.begin(), extracted.end(), [](Extract a, Extract b){
        return a.count > b.count;
    });

    // Write results
    cout << endl << endl;
    if(flag_write){
        for(unsigned short k = 0; k < extracted.size(); k++){
            Extract extract = extracted[k];
            write << extract.word << endl;
        }
        write.close();
    }
}

void help(){
    HelpMenu help("xtracter");
    help.add_usage("xtracter -f <FILE> -o <FILE> [OPTIONS]");
    help.add_option("-f", "Define the password file", true);
    help.add_option("-o", "Define the write file", true);
    help.add_option("-m", "Define the minimal length of characters", false);
    help.add_option("-M", "Define the maximal length of characters", false);
    help.add_option("-u", "Define the write file for unextracted passwords", false);
    help.add_option("-v", "Set verbosity off", false);
    help.add_option("-h", "Show help menu", false);
    help.add_example("xtracter -f rockyou.txt -o words.txt -m 4 -M 10");
    help.print();
}

int main(int argc, char* argv[])
{
    if(argc < 2){
        help();
    }

    // Getopt
    int c;

    while ((c = getopt (argc, argv, "f:o:m:M:u:vh")) != -1)
        switch (c)
        {
        case 'f':{
            file.open(optarg);
            flag_file = true;
        }
            break;
        case 'o':
            write.open(optarg);
            flag_write = true;
            break;
        case 'm':
            minimal_chars = stoi(optarg);
            break;
        case 'M':
            maximal_chars = stoi(optarg);
            break;
        case 'u':
            unextracted.open(optarg);
            flag_unextracted = true;
            break;
        case 'v':
            flag_verbose = false;
            break;
        case 'h':
            help();
            exit(0);
            break;
        default:
            abort();
        }

    // Assert if the dictionary is open
    if(dictionary.empty()){
        error("Failed to open the dictionary", true);
    }

    string password;

    // Extract words
    while(file >> password){
        extract(lowercase(password));
    }
    cout << endl;
    file.close();
    if(flag_unextracted){
        unextracted.close();
    }

    // Count words
    count();

    return 0;
}
