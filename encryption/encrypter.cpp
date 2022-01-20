#include "encrypter.h"

bool Encrypter::is_valid_string(string tmp)
{
  for(unsigned int i = 0; i < tmp.length(); i++)
  {
    if(tmp[i] == ' ' || ctoi(tmp[i])) continue;
    else return false;
  }
  return true;
}

string Encrypter::single_key_encrypt(string message, string key, bool show)
{
  string encrypted_message = "";
  int key_i;
  char next_char;
  for(unsigned int i = 0; i < message.length(); i++)
  {
    key_i = i % key.length();
    next_char = itoc((ctoi(message[i]) + ctoi(key[key_i])) % m_alphabet_size);

    if(show) cout << message[i] << " encrypted with " << key[key_i] << " is " << next_char << endl;

    encrypted_message += next_char;
  }
  return encrypted_message;
}

string Encrypter::single_key_decrypt(string encrypted_msg, string key, bool show)
{
  string message = "";
  int key_i;
  char next_char;
  for(unsigned int i = 0; i < encrypted_msg.length(); i++)
  {
    key_i = i % key.length();
    next_char = itoc((ctoi(encrypted_msg[i]) - ctoi(key[key_i]) + m_alphabet_size) % m_alphabet_size);
    //Need to add alphabet_size in case the key char value was higher than the msg char value

    if(show) cout << encrypted_msg[i] << " decrypted with " << key[key_i] << " is " << next_char << endl;

    message += next_char;
  }
  return message;
}

string Encrypter::RSA_generate_public_key(string private_key1, string private_key2)
{
  return "Not implemented yet.";
}

string Encrypter::RSA_encrypt(string message, string public_key)
{
  return "Not implemented yet";
}

//This will return 0 if given an invalid character
int Encrypter::ctoi(char c)
{
  for(int i = 0; i < m_alphabet_size; i++)
  {
    if(c == m_alphabet[i]) return i;
  }
  return 0;
}

char Encrypter::itoc(int i)
{
  if(i < 0 || i > 95) return '\n';

  return m_alphabet[i];
}
