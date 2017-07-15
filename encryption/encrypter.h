#ifndef ENCRYPTER_H
#define ENCRYPTER_H

#include <iostream>
#include <string>

using namespace std;

class Encrypter
{
  public:
    Encrypter() {m_single_key = "";
                 m_public_key = "";
                 m_private_key1 = "";
                 m_private_key2 = "";}

    void set_single_key(string single_key) {m_single_key = single_key;}
    string single_key_encrypt(string message);
    string single_key_decrypt(string encrypted_message);

    void set_public_key(string public_key) {m_public_key = public_key;}
    string encrypt_with_public_key(string message);

    void set_private_keys(string private_key1, string private_key2)
    {m_private_key1 = private_key1; m_private_key2 = private_key2;}
    string generate_public_key_from_private_keys();
    
  private:
    int atoi(char c);
    char itoa(int i);

    string m_single_key;
    string m_public_key;
    string m_private_key1;
    string m_private_key2;
};
#endif
