#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <cstdio>
#include <algorithm>
#include "encrypter.h"

using namespace std;

Encrypter* encrypter = new Encrypter();

void Usage()
{
  cout << "Usage:" << endl;
  cout << "  encrypter            - Asks for every option" << endl;
  cout << "  encrypter -f <file>  - Encrypt or decrypt a file" << endl;
  cout << "  encrypter -s         - Shows the math behind the encryption" << endl;
  cout << endl;

  delete encrypter;
  exit(0);
}

//FUNCTIONS v
bool correct_usage(int argc, char* argv[]);

void run_message_program(bool show);

  void intro_description();

  int get_message_encryption_option();

  void run_single_key(bool show);
    void run_single_key_encrypt(bool show);
    void run_single_key_decrypt(bool show);
      string get_single_encryption_key();
      string get_single_decryption_key();

  int run_RSA();

    string get_message(bool encrypting);

void run_file_program(string file_name);

  int file_get_encryption_option(string file_name);

  void file_encrypt(string file_name);
    void file_encryption_warning_option(ofstream & output);
  void file_decrypt(string file_name);
//FUNCTIONS ^

int main(int argc, char* argv[])
{
  bool show = correct_usage(argc, argv);

  if(argc == 1 || argc == 2) run_message_program(show);
  else                       run_file_program(string (argv[2]));

  delete encrypter;
  return 0;
}

//The return bool is if the program should show the math behind the encryption
//This will call Usage() (which exits) if the command was improperly used
//Returning either true or false means the command arguments were acceptable
bool correct_usage(int argc, char* argv[])
{
  if(argc == 1) return false;

  if(argc == 2)
  {
    if(string(argv[1]) == "-s") return true;
    else                        Usage();
  }

  if(argc == 3)
  {
    if(string(argv[1]) != "-f") Usage();

    if(filesystem::exists(argv[2])) return false;
    else                            Usage();
  }

  Usage();     //<- Usage() will exit the program
  return true; //<- This is only to get rid of compiler warning
}

void run_message_program(bool show)
{
  intro_description();

  int option = get_message_encryption_option(); //can only return 1 or 2

  if      (option == 1) run_single_key(show);
  else if (option == 2) run_RSA();

  return;
}

void intro_description()
{
  cout << "This program will help you out with encryption. You can either work with" << endl;
  cout << "single-key encryption or 2-key (RSA) encryption." << endl << endl;

  cout << "You should only use 2-key encryption for the very first contact with another" << endl;
  cout << "person, as it is very resource intesive. You should include a single key for" << endl;
  cout << "further communication within that first message. You should use single-key" << endl;
  cout << "encryption for all further communication with that same person." << endl << endl;

  cout << "NOTE: The single key should be longer than 15 characters so it is difficult" << endl;
  cout << "to crack." << endl << endl;

  return;
}

int get_message_encryption_option()
{
  string option;
  do
  {
    cout << "Options:" << endl;
    cout << "1) Single-key" << endl;
    cout << "2) 2-key (RSA)" << endl << endl;
    cout << "What do you want to work with? ";

    do
    {
      getline(cin, option);
    }while(option.length() == 0);
    option.erase(remove(option.begin(), option.end(), ' '), option.end());
    option.erase(remove(option.begin(), option.end(), '\t'), option.end());
    cout << endl;

    if      (option == "1") return 1;
    else if (option == "2") return 2;
    else cout << " ** That is not a valid option. **" << endl << endl;
  }while(1);
}

void run_single_key(bool show)
{
  cout << "************************* SINGLE-KEY *************************" << endl << endl;

  string option;
  do
  {
    cout << "Options:" << endl;
    cout << "1) Encrypt" << endl;
    cout << "2) Decrypt" << endl << endl;
    cout << "What do you want to do? ";

    do
    {
      getline(cin, option);
    }while(option.length() == 0);
    option.erase(remove(option.begin(), option.end(), ' '), option.end());
    option.erase(remove(option.begin(), option.end(), '\t'), option.end());
    cout << endl;

    if      (option == "1") return run_single_key_encrypt(show);
    else if (option == "2") return run_single_key_decrypt(show);
    else cout << " ** That is not a valid option. **" << endl << endl;
  }while(1);
}

void run_single_key_encrypt(bool show)
{
  cout << "************************* ENCRYPTING *************************" << endl << endl;

  string msg = get_message(true);
  string key = get_single_encryption_key();

  string encrypted_msg = encrypter->single_key_encrypt(msg, key, show);

  cout << endl;
  cout << "\"" << msg << "\"" << endl;
  cout << "encrypted with \"" << key << "\" is:" << endl << endl;
  cout << "\"" << encrypted_msg << "\"" << endl;

  return;
}

void run_single_key_decrypt(bool show)
{
  cout << "************************* DECRYPTING *************************" << endl << endl;

  string encrypted_msg = get_message(false);
  string key = get_single_decryption_key();

  string msg = encrypter->single_key_decrypt(encrypted_msg, key, show);

  cout << endl;
  cout << "\"" << encrypted_msg << "\"" << endl;
  cout <<"decrypted with \"" << key << "\" is:" << endl << endl;
  cout << "\"" << msg << "\"" << endl;

  return;
}

string get_message(bool encrypting)
{
  string msg;
  bool valid_str;

  do
  {
    cout << "Give me a message to encrypt:" << endl;
    do
    {
      getline(cin, msg);
    }while(msg.length() == 0);
    cout << endl;

    if(msg.length() < 5)
    {
      valid_str = false;
      cout << " ** Message needs to be longer than 5 charaters **" << endl << endl;
    }
    else
    {
      valid_str = encrypter->is_valid_string(msg);
      if(!valid_str) cout << " ** You used invlaid characters **" << endl << endl;
    }
  }while(!valid_str);
  return msg;
}

string get_decrypting_message()
{
  string msg;
  bool valid_str;
  do
  {
    cout << "Copy/paste the encrypted message:" << endl;
    do
    {
      getline(cin, msg);
    }while(msg.length() == 0);
    cout << endl;

    valid_str = encrypter->is_valid_string(msg);
    if(!valid_str) cout << " ** Your message used invlaid characters **" << endl << endl;

  }while(!valid_str);

  return msg;
}

string get_single_encryption_key()
{
  string key;
  bool valid_str;

  do
  {
    cout << "Give me the key to encrypt with:" << endl;
    do
    {
      getline(cin, key);
    }while(key.length() == 0);
    cout << endl;

    if(key.length() < 5)
    {
      valid_str = false;
      cout << " ** Key needs to be longer than 5 charaters **" << endl << endl;
    }
    else
    {
      valid_str = encrypter->is_valid_string(key);
      if(!valid_str) cout << " ** You used invalid characters **" << endl << endl;
    }
  }while(!valid_str);

  if(key.length() < 15)
  {
     cout << " ** NOTE: You should think about making the key longer so that it is harder to crack. **" << endl << endl;
  }

  return key;
}

string get_single_decryption_key()
{
  string key;
  cout << "And what is the key?" << endl;
  do
  {
    getline(cin, key);
  }while(key.length() == 0);
  cout << endl;

  return key;
}

int run_RSA()
{
    cout << "************************* 2-KEY (RSA) ************************" << endl << endl;

  cout << "This is not avaliable yet. Sorry." << endl;

  return 0;
}

void run_file_program(string file_name)
{
  //We know that given file_name exists (via correct_usage())

  string option;
  do
  {
    cout << "Options:" << endl;
    cout << "1) Encrypt" << endl;
    cout << "2) Decrypt" << endl << endl;
    cout << "What do you want to do with " << file_name << "?" << endl;

    do
    {
      getline(cin, option);
    }while(option.length() == 0);
    option.erase(remove(option.begin(), option.end(), ' '), option.end());
    option.erase(remove(option.begin(), option.end(), '\t'), option.end());
    cout << endl;

    if      (option == "1") return file_encrypt(file_name);
    else if (option == "2") return file_decrypt(file_name);
    else cout << " ** That is not a valid option. **" << endl << endl;
  }while(1);
}

void file_encrypt(string file_name)
{
  //We know that the file name exists (correct_usage())
  ifstream input(file_name, ios::in);

  //We are going to use this file to write the encryption to and then
  //mv this file over the input file
  //I dont want to use a string in case the file is too large
  //for a single string
  string output_file_name = "file.encrypted.tmp";
  ofstream tmp_output(output_file_name, ios::out);

  if(!tmp_output)
  {
    cout << "Something went wrong with opening the tmp file needed to do the encrypting." << endl;
    cout << "Nothing has happened with your file." << endl << endl;
    cout << "Exiting" << endl;
    return;
  }

  file_encryption_warning_option(tmp_output);

  string key = get_single_encryption_key();

  char tmp;
  int key_i = 0;
  while(input >> tmp)
  {
    if(encrypter->is_valid_char(tmp))
    {
      tmp_output << encrypter->encrypt_char(tmp, key[key_i]);
      key_i++;
      key_i = key_i % key.length();
    }
    else
    {
      cout << "There was an invalid character ('" << tmp << "') somewhere in the file." << endl;
      cout << "Nothing has been done to your file." << endl << endl;

      tmp_output.close();
      remove(output_file_name.c_str());
 
      cout << "Exiting" << endl;
      return;
    }
  }
  
//  tmp_output << endl;
  tmp_output.close();

  rename(output_file_name.c_str(), file_name.c_str()); //over writes old file

  cout << file_name << " has been overwritten and there is no undoing it without the key" << endl << endl;

  return;
}

void file_encryption_warning_option(ofstream & output)
{
  string option;
  do
  {
    cout << "Do you want to put a warning at the top of the encrypted file (Y/N)? ";
    do
    {
      getline(cin, option);
    }while(option.length() == 0);
    option.erase(remove(option.begin(), option.end(), ' '), option.end());
    option.erase(remove(option.begin(), option.end(), '\t'), option.end());
    cout << endl;

    if(option == "Y" || option == "y")
    {
      output << "** This file has been encrypted using github.com/turtlehump/funsidestuff/encrypter\n";
      output << "** You will have to use the same executable to decrypt it\n";
      output << "** Making modifications to this file will result in modifications to the original file\n";
      output << "** There is no need to delete these comments above, decrypting will remove them\n\n";
      return;
    }
    else if(option == "N" || option == "n")
    {
      return;
    }
    else cout << " ** That is not a valid option **" << endl << endl;
  }while(1);
}

void file_decrypt(string filename)
{
  return;
}
