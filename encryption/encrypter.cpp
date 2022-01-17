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

string Encrypter::single_key_encrypt(string message)
{
  string encrypted_message = "";
  int single_key_i;
  char next_char;
  for(unsigned int i = 0; i < message.length(); i++)
  {
    single_key_i = i % m_single_key.length();
    next_char = itoc((ctoi(message[i]) + ctoi(m_single_key[single_key_i])) % m_alphabet_size);

    cout << message[i] << " encrypted with " << m_single_key[single_key_i] << " is " << next_char << endl;

    encrypted_message += next_char;
  }
  return encrypted_message;
}

string Encrypter::single_key_decrypt(string encrypted_msg)
{
  string message = "";
  int single_key_i;
  char next_char;
  for(unsigned int i = 0; i < encrypted_msg.length(); i++)
  {
    single_key_i = i % m_single_key.length();
    next_char = itoc((ctoi(encrypted_msg[i]) - ctoi(m_single_key[single_key_i]) + m_alphabet_size) % m_alphabet_size);
    //Need to add alphabet_size in case the key char value was higher than the msg char value

    cout << encrypted_msg[i] << " decrypted with " << m_single_key[single_key_i] << " is " << next_char << endl;

    message += next_char;
  }
  return message;
}

string Encrypter::encrypt_with_public_key(string message)
{
  return message;
}

string Encrypter::generate_public_key_from_private_keys()
{
  return m_private_key1 + m_private_key2;
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
