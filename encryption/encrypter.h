#ifndef ENCRYPTER_H
#define ENCRYPTER_H

#include <iostream>
#include <string>

using namespace std;

class Encrypter
{
  public:
    //The alphabet includes eveything on the keyboard EXCEPT F-keys, arrow keys, and ESC
    bool is_valid_char (char tmp);
    bool is_valid_string (string tmp);

    //SINGLE-KEY FUNCTIONS
    char encrypt_char(char message_char, char key_char);
    char decrypt_char(char message_char, char key_char);
    string single_key_encrypt(string message, string key, bool show);
    string single_key_decrypt(string encrypted_message, string key, bool show);

    //RSA FUNCTIONS
    string RSA_generate_public_key(string private_key1, string private_key2);
    string RSA_encrypt(string message, string public_key);
    
  private:
    int ctoi(char c);
    char itoc(int i);

    int  m_alphabet_size = 97; //0-96
    char m_alphabet[97] {' ',  // 0
                         '!',  // 1
                         '\"', // 2
                         '#',  // 3
                         '$',  // 4
                         '%',  // 5
                         '&',  // 6
                         '\'', // 7
                         '(',  // 8
                         ')',  // 9
                         '*',  // 10
                         '+',  // 11
                         ',',  // 12
                         '-',  // 13
                         '.',  // 14
                         '/',  // 15
                         '0',  // 16
                         '1',  // 17
                         '2',  // 18
                         '3',  // 19
                         '4',  // 20
                         '5',  // 21
                         '6',  // 22
                         '7',  // 23
                         '8',  // 24
                         '9',  // 25
                         ':',  // 26
                         ';',  // 27
                         '>',  // 28
                         '=',  // 29
                         '<',  // 30
                         '?',  // 31
                         '@',  // 32
                         'A',  // 33
                         'B',  // 34
                         'C',  // 35
                         'D',  // 36
                         'E',  // 37
                         'F',  // 38
                         'G',  // 39
                         'H',  // 40
                         'I',  // 41
                         'J',  // 42
                         'K',  // 43
                         'L',  // 44
                         'M',  // 45
                         'N',  // 46
                         'O',  // 47
                         'P',  // 48
                         'Q',  // 49
                         'R',  // 50
                         'S',  // 51
                         'T',  // 52
                         'U',  // 53
                         'V',  // 54
                         'W',  // 55
                         'X',  // 56
                         'Y',  // 57
                         'Z',  // 58
                         '[',  // 59
                         '\\', // 60
                         ']',  // 61
                         '^',  // 62
                         '_',  // 63
                         '`',  // 64
                         'a',  // 65
                         'b',  // 66
                         'c',  // 67
                         'd',  // 68
                         'e',  // 69
                         'f',  // 70
                         'g',  // 71
                         'h',  // 72
                         'i',  // 73
                         'j',  // 74
                         'k',  // 75
                         'l',  // 76
                         'm',  // 77
                         'n',  // 78
                         'o',  // 79
                         'p',  // 80
                         'q',  // 81
                         'r',  // 82
                         's',  // 83
                         't',  // 84
                         'u',  // 85
                         'v',  // 86
                         'w',  // 87
                         'x',  // 88
                         'y',  // 89
                         'z',  // 90
                         '{',  // 91
                         '|',  // 92
                         '}',  // 93
                         '~',  // 94
                         '\t', // 95
                         '\n'};// 96
};
#endif
