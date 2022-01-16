#include <iostream>
#include <string>
#include <algorithm>
#include "encrypter.h"

using namespace std;

void intro_description();

int get_encryption_option();

int run_single_key();
int run_single_key_encrypt();
int run_single_key_decrypt();

int run_RSA();

int main()
{
  intro_description();

  int option = get_encryption_option(); //can only return 1 or 2

  if      (option == 1) return run_single_key();
  else if (option == 2) return run_RSA();
  else                  return 1; //There was an error in selecting the encryption type
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
    option.erase(remove(option.begin(), option.end(), '\t'), option.end());
    cout << endl;

    if      (option == "1") return 1;
    else if (option == "2") return 2;
    else cout << " ** That is not a valid option. **" << endl << endl;
  }while(1);
}

int run_single_key()
{
  cout << "************************* SINGLE-KEY *************************" << endl << endl;

  string option;
  do
  {
    cout << "Options:" << endl;
    cout << "1) Encrypt" << endl;
    cout << "2) Decrypt" << endl << endl;
    cout << "What do you want to do? ";

    getline(cin, option);
    option.erase(remove(option.begin(), option.end(), ' '), option.end());
    option.erase(remove(option.begin(), option.end(), '\t'), option.end());
    cout << endl;

    if      (option == "1") return run_single_key_encrypt();
    else if (option == "2") return run_single_key_decrypt();
    else cout << " ** That is not a valid option. **" << endl << endl;
  }while(1);
}

int run_single_key_encrypt()
{
  cout << "************************* ENCRYPTING *************************" << endl << endl;

  Encrypter* encrypter = new Encrypter();
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

int run_single_key_decrypt()
{
  cout << "************************* DECRYPTING *************************" << endl << endl;

  Encrypter* encrypter = new Encrypter();
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

int run_RSA()
{
    cout << "************************* 2-KEY (RSA) ************************" << endl << endl;

  cout << "This is not avaliable yet. Sorry." << endl;

  return 0;
}
