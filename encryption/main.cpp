#include <iostream>
#include <string>
#include <algorithm>
#include "encrypter.h"

using namespace std;

void intro_description();

//I wouldnt have to do all this if people would just input properly...
int get_encryption_option();

int run_single_key_option();
int run_RSA_option();

int main()
{
  intro_description();

  int option = get_encryption_option();

  if(option == 1)
  {
    return run_single_key_option();
  }
  else if(option == 2)
  {
    return run_RSA_option();
  }
  else
  {
    cout << "Error: Got out of get_encryption_option() with invalid option." << endl;
    return 1;
  }
}

void intro_description()
{
  cout << "This program will help you out with encryption. You can either work with" << endl;
  cout << "single-key encryption or 2-key (RSA) encryption." << endl << endl;

  cout << "You should only use 2-key encryption for the very first contact with another" << endl;
  cout << "person, as it is very resource intesive. You should include a single key for" << endl;
  cout << "further communication within that first message. You should use single-key" << endl;
  cout << "encryption for all further communication with that same person." << endl << endl;

  cout << "Note: The single key should be longer than 15 characters so it is difficult" << endl;
  cout << "to crack." << endl << endl;

  return;
}

int get_encryption_option()
{
  string option;
  do
  {
    cout << "Options:" << endl;
    cout << "1) Single-key" << endl;
    cout << "2) 2-key (RSA)" << endl << endl;
    cout << "What do you want to work with? ";

    getline(cin, option);
    option.erase(remove(option.begin(), option.end(), ' '), option.end());
    cout << endl;

    if      (option == "1") return 1;
    else if (option == "2") return 2;
    else cout << " ** That is not a valid option. **" << endl << endl;
  }while(1);
}

int run_single_key_option()
{
  Encrypter* encrypter = new Encrypter();
  cout << "************************* SINGLE-KEY *************************" << endl << endl;

  int option;
  do
  {
    string option_str;
    cout << "Options:" << endl;
    cout << "1) Encrypt" << endl;
    cout << "2) Decrypt" << endl << endl;
    cout << "What do you want to do? ";

    getline(cin, option_str);
    option_str.erase(remove(option_str.begin(), option_str.end(), ' '), option_str.end());
    cout << endl;

    if(option_str == "1")
    {
      option = 1;
      break;
    }
    else if(option_str == "2")
    {
      option = 2;
      break;
    }
    else
    {
      cout << " ** That is not a valid option. **" << endl << endl;
    }
  }while(1);

  if(option == 1)
  {
    cout << "************************* ENCRYPTING *************************" << endl << endl;
    string message;
    string key;

    cout << "Give me a message to encrypt:" << endl;
    getline(cin, message);
    cout << endl;

    cout << "Give me the key to encrypt the message with:" << endl;
    getline(cin, key);
    cout << endl;

    if(key.length() < 15)
    {
       cout << "NOTE: You should think about making the key longer so that it is harder to crack." << endl << endl;
    }

    encrypter->set_single_key(key);
    string encrypted_msg = encrypter->single_key_encrypt(message);

    cout << endl << endl;
    cout << "\"" << message << "\"" << endl;
    cout << "encrypted with \"" << key << "\" is:" << endl << endl;
    cout << "\"" << encrypted_msg << "\"" << endl;
    delete encrypter;
    return 0;
  }
  else if(option == 2)
  {
    cout << "************************* DECRYPTING *************************" << endl << endl;
    string encrypted_msg;
    string key;

    cout << "Give me the encrypted message: ";
    getline(cin, encrypted_msg);
    cout << endl;

    cout << "And what is the key? ";
    getline(cin, key);

    encrypter->set_single_key(key);
    string message = encrypter->single_key_decrypt(encrypted_msg);

    cout << endl << endl;
    cout << "\"" << encrypted_msg << "\"" << endl;
    cout <<"decrypted with \"" << key << "\" is:" << endl << endl;
    cout << "\"" << message << "\"" << endl;
    delete encrypter;
    return 0;
  }
  else
  {
    cout << "Error: Got out of Single-Key Option selection with invalid option." << endl;
    return 1;
  }
}

int run_RSA_option()
{
    cout << "************************* 2-KEY (RSA) ************************" << endl << endl;

  cout << "This is not avaliable yet. Sorry." << endl;

  return 0;
}
