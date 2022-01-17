#include <iostream>
#include <string>
#include <algorithm>
#include "encrypter.h"

using namespace std;

Encrypter* encrypter = new Encrypter();

void intro_description();

int get_encryption_option();

string get_message(bool encrypting);

void run_single_key();
void run_single_key_encrypt();
void run_single_key_decrypt();
string get_single_key(bool encrypting);

int run_RSA();

int main()
{
  intro_description();

  int option = get_encryption_option(); //can only return 1 or 2

  if      (option == 1) run_single_key();
  else if (option == 2) run_RSA();

  delete encrypter;
  return 0;
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

void run_single_key()
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

    if      (option == "1") return run_single_key_encrypt();
    else if (option == "2") return run_single_key_decrypt();
    else cout << " ** That is not a valid option. **" << endl << endl;
  }while(1);
}

void run_single_key_encrypt()
{
  cout << "************************* ENCRYPTING *************************" << endl << endl;

  string msg = get_message(true);
  string key = get_single_key(true);

  encrypter->set_single_key(key);
  string encrypted_msg = encrypter->single_key_encrypt(msg);

  cout << endl << endl;
  cout << "\"" << msg << "\"" << endl;
  cout << "encrypted with \"" << key << "\" is:" << endl << endl;
  cout << "\"" << encrypted_msg << "\"" << endl;

  return;
}

void run_single_key_decrypt()
{
  cout << "************************* DECRYPTING *************************" << endl << endl;

  string encrypted_msg = get_message(false);
  string key = get_single_key(false);

  encrypter->set_single_key(key);
  string msg = encrypter->single_key_decrypt(encrypted_msg);

  cout << endl << endl;
  cout << "\"" << encrypted_msg << "\"" << endl;
  cout <<"decrypted with \"" << key << "\" is:" << endl << endl;
  cout << "\"" << msg << "\"" << endl;

  return;
}

string get_message(bool encrypting)
{
  string msg;
  bool valid_str;

  if(encrypting) //**More restrictive for creating a message**
  {
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
  }
  else //if you are trying to decrypt a message that was made outside this program/class then
       //it's not going to work and you can waste as much time as your heart desires
  {

    cout << "Copy/paste the encrypted message:" << endl;
    do
    {
      do
      {
        getline(cin, msg);
      }while(msg.length() == 0);
      cout << endl;

      valid_str = encrypter->is_valid_string(msg);
      if(!valid_str) cout << " ** Your message used invlaid characters **" << endl << endl;

    }while(!valid_str);
  }

  return msg;
}

string get_single_key(bool encrypting)
{
  string key;
  bool valid_str;

  if(encrypting) //More restrictive for creating a message
  {
    do
    {
      cout << "Give me the key to encrypt the message with:" << endl;
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
       cout << "NOTE: You should think about making the key longer so that it is harder to crack." << endl << endl;
    }
  }
  else //If you are trying to decrypt a message that was made outside this program/class then
       //it's not going to work and you can waste as much time as your heart desires
  {
    cout << "And what is the key?" << endl;
    do
    {
      getline(cin, key);
    }while(key.length() == 0);
  }
  cout << endl;

  return key;
}

int run_RSA()
{
    cout << "************************* 2-KEY (RSA) ************************" << endl << endl;

  cout << "This is not avaliable yet. Sorry." << endl;

  return 0;
}
