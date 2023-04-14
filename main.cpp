#include <iostream>
#include <getopt.h>
#include <fstream>
#include <algorithm>
#include <chrono>
#include "uforia/uforia.h"

using namespace std;
using namespace uforia;

int maximal_chars = 12, minimal_chars = 6;
ifstream file;
ofstream write;
ofstream unextracted;
string buffer;
vector<string> dictionary = from("/etc/xtracter/english.dic", true);
vector<string> collection;
vector<string> temporary;
bool flag_file = false, flag_write = false, flag_unextracted = false, flag_verbose = true;

// Set language of the dictionary
void set_language(string language){
  if(language == "english"){
      dictionary = from("/etc/xtracter/english.dic", true);
    }else if(language == "dutch"){
      dictionary = from("/etc/xtracter/dutch.dic", true);
    }else if(language == "spanish"){
      dictionary = from("/etc/xtracter/spanish.dic", true);
    }else if(language == "french"){
      dictionary = from("/etc/xtracter/french.dic", true);
    }else if(language == "german"){
      dictionary = from("/etc/xtracter/german.dic", true);
    }else{
      error("Wrong language given (-l)", true);
    }
}

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

void help(){
  HelpMenu help("xtracter");
  help.add_usage("xtracter -f <FILE> -o <FILE> [OPTIONS]");
  help.add_option("-f", "Define the password file", true);
  help.add_option("-o", "Define the write file", true);
  help.add_option("-m", "Define the minimal length of characters", false);
  help.add_option("-M", "Define the maximal length of characters", false);
  help.add_option("-u", "Define the write file for unextracted passwords", false);
  help.add_option("-v", "Set verbosity off", false);
  help.add_option("-l", "Define the language of the dictionary", false);
  help.add_option("-h", "Show help menu", false);
  help.add_example("xtracter -f rockyou.txt -o words.txt -m 4 -M 10");
  help.print();
}

int main(int argc, char* argv[])
{
  if(argc < 2){
      help();
    }

  int c;

  while ((c = getopt (argc, argv, "f:o:m:M:u:vl:h")) != -1)
    switch (c)
      {
      case 'f':{
          file.open(optarg);
          if(file.good()){
              flag_file = true;
            }else{
              error("Wrong file given (-f)", true);
            }
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
      case 'u':
        unextracted.open(optarg);
        flag_unextracted = true;
        break;
      case 'v':
        flag_verbose = false;
        break;
      case 'l':
        dictionary.clear();
        set_language(optarg);
        break;
      case 'h':
        help();
        exit(0);
        break;
      default:
        abort();
      }

  // Checks if a file is given
  if(!flag_file){
      error("No file given (-f)", true);
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

  // Count and make unique
  temporary = uforia::count(collection);
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
