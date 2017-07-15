#include <iostream>
#include <string>
#include "encrypter.h"

using namespace std;

int main()
{
  Encrypter* encrypter = new Encrypter();

  int option;
  cout << "1) Encrypt a message with a single key." << endl;
  cout << "2) decrypt a message with a single key." << endl;
  cout << "What do you want to do? ";
  cin >> option;

  if(option == 1)
  {
    string single_key;
    string message;

    cout << "Give me a message to encrypt: ";
    cin.ignore();
    getline(cin, message);
    cin.unget();


    cout << "Give me a single key to encrypt with: ";
    cin.ignore();
    getline(cin, single_key);
    cin.unget();

    encrypter->set_single_key(single_key);
    string encryption = encrypter->single_key_encrypt(message);

    cout << endl << endl;
    cout << "\"" << message << "\" encrypted with \"" << single_key << "\" is:" << endl;
    cout << "\"" << encryption << "\"" << endl;
    delete encrypter;
    return 0;
  }
  else if(option == 2)
  {
    string single_key;
    string encryption;

    cout << "Give me the encrypted message: ";
    cin.ignore();
    getline(cin, encryption);
    cin.unget();

    cout << "And what is the key? ";
    cin.ignore();
    getline(cin, single_key);
    cin.unget();

    encrypter->set_single_key(single_key);
    string message = encrypter->single_key_decrypt(encryption);

    cout << endl << endl;
    cout << "\"" << encryption << "\" decrypted with \"" << single_key << "\" is:" << endl;
    cout << "\"" << message << "\"" << endl;
    delete encrypter;
    return 0;
  }
  else
  {
    cout << "Thats not a valid option.";
    return 0;
  }
}
