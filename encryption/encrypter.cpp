#include "encrypter.h"

string Encrypter::single_key_encrypt(string message)
{
  string encrypted_message = "";
  int single_key_i;
  char next_char;
  for(unsigned int i = 0; i < message.length(); i++)
  {
    single_key_i = i % m_single_key.length();
    next_char = itoa((atoi(message[i]) + atoi(m_single_key[single_key_i])) % 95);
    //95 is just the number of valid characters in my atoi (0-94)
    encrypted_message += next_char;
  }
  return encrypted_message;
}

string Encrypter::single_key_decrypt(string encrypted_message)
{
  string message = "";
  int single_key_i;
  char next_char;
  for(unsigned int i = 0; i < encrypted_message.length(); i++)
  {
    single_key_i = i % m_single_key.length();
    next_char = itoa((atoi(encrypted_message[i]) - atoi(m_single_key[single_key_i]) + 95) % 95);
    //95 is just the number of valid characters in my atoi (0-94)
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

//this will return 0 if given an invalid character
int Encrypter::atoi(char c)
{
  if(c == ' ') return 0;
  if(c == '!') return 1;
  if(c == '\"') return 2;
  if(c == '#') return 3;
  if(c == '$') return 4;
  if(c == '%') return 5;
  if(c == '&') return 6;
  if(c == '\'') return 7;
  if(c == '(') return 8;
  if(c == ')') return 9;
  if(c == '*') return 10;
  if(c == '+') return 11;
  if(c == ',') return 12;
  if(c == '-') return 13;
  if(c == '.') return 14;
  if(c == '/') return 15;
  if(c == '0') return 16;
  if(c == '1') return 17;
  if(c == '2') return 18;
  if(c == '3') return 19;
  if(c == '4') return 20;
  if(c == '5') return 21;
  if(c == '6') return 22;
  if(c == '7') return 23;
  if(c == '8') return 24;
  if(c == '9') return 25;
  if(c == ':') return 26;
  if(c == ';') return 27;
  if(c == '>') return 28;
  if(c == '=') return 29;
  if(c == '<') return 30;
  if(c == '?') return 31;
  if(c == '@') return 32;
  if(c == 'A') return 33;
  if(c == 'B') return 34;
  if(c == 'C') return 35;
  if(c == 'D') return 36;
  if(c == 'E') return 37;
  if(c == 'F') return 38;
  if(c == 'G') return 39;
  if(c == 'H') return 40;
  if(c == 'I') return 41;
  if(c == 'J') return 42;
  if(c == 'K') return 43;
  if(c == 'L') return 44;
  if(c == 'M') return 45;
  if(c == 'N') return 46;
  if(c == 'O') return 47;
  if(c == 'P') return 48;
  if(c == 'Q') return 49;
  if(c == 'R') return 50;
  if(c == 'S') return 51;
  if(c == 'T') return 52;
  if(c == 'U') return 53;
  if(c == 'V') return 54;
  if(c == 'W') return 55;
  if(c == 'X') return 56;
  if(c == 'Y') return 57;
  if(c == 'Z') return 58;
  if(c == '[') return 59;
  if(c == '\\') return 60;
  if(c == ']') return 61;
  if(c == '^') return 62;
  if(c == '_') return 63;
  if(c == '`') return 64;
  if(c == 'a') return 65;
  if(c == 'b') return 66;
  if(c == 'c') return 67;
  if(c == 'd') return 68;
  if(c == 'e') return 69;
  if(c == 'f') return 70;
  if(c == 'g') return 71;
  if(c == 'h') return 72;
  if(c == 'i') return 73;
  if(c == 'j') return 74;
  if(c == 'k') return 75;
  if(c == 'l') return 76;
  if(c == 'm') return 77;
  if(c == 'n') return 78;
  if(c == 'o') return 79;
  if(c == 'p') return 80;
  if(c == 'q') return 81;
  if(c == 'r') return 82;
  if(c == 's') return 83;
  if(c == 't') return 84;
  if(c == 'u') return 85;
  if(c == 'v') return 86;
  if(c == 'w') return 87;
  if(c == 'x') return 88;
  if(c == 'y') return 89;
  if(c == 'z') return 90;
  if(c == '{') return 91;
  if(c == '|') return 92;
  if(c == '}') return 93;
  if(c == '~') return 94;
  return 0;
}

char Encrypter::itoa(int i)
{
  if(i ==  0) return ' '; 
  if(i ==  1) return '!'; 
  if(i ==  2) return '\"'; 
  if(i ==  3) return '#'; 
  if(i ==  4) return '$'; 
  if(i ==  5) return '%'; 
  if(i ==  6) return '&'; 
  if(i ==  7) return '\''; 
  if(i ==  8) return '('; 
  if(i ==  9) return ')'; 
  if(i ==  10) return '*'; 
  if(i ==  11) return '+'; 
  if(i ==  12) return ','; 
  if(i ==  13) return '-'; 
  if(i ==  14) return '.'; 
  if(i ==  15) return '/'; 
  if(i ==  16) return '0'; 
  if(i ==  17) return '1'; 
  if(i ==  18) return '2'; 
  if(i ==  19) return '3'; 
  if(i ==  20) return '4'; 
  if(i ==  21) return '5'; 
  if(i ==  22) return '6'; 
  if(i ==  23) return '7'; 
  if(i ==  24) return '8'; 
  if(i ==  25) return '9'; 
  if(i ==  26) return ':'; 
  if(i ==  27) return ';'; 
  if(i ==  28) return '>'; 
  if(i ==  29) return '='; 
  if(i ==  30) return '<'; 
  if(i ==  31) return '?'; 
  if(i ==  32) return '@'; 
  if(i ==  33) return 'A'; 
  if(i ==  34) return 'B'; 
  if(i ==  35) return 'C'; 
  if(i ==  36) return 'D'; 
  if(i ==  37) return 'E'; 
  if(i ==  38) return 'F'; 
  if(i ==  39) return 'G'; 
  if(i ==  40) return 'H'; 
  if(i ==  41) return 'I'; 
  if(i ==  42) return 'J'; 
  if(i ==  43) return 'K'; 
  if(i ==  44) return 'L'; 
  if(i ==  45) return 'M'; 
  if(i ==  46) return 'N'; 
  if(i ==  47) return 'O'; 
  if(i ==  48) return 'P'; 
  if(i ==  49) return 'Q'; 
  if(i ==  50) return 'R'; 
  if(i ==  51) return 'S'; 
  if(i ==  52) return 'T'; 
  if(i ==  53) return 'U'; 
  if(i ==  54) return 'V'; 
  if(i ==  55) return 'W'; 
  if(i ==  56) return 'X'; 
  if(i ==  57) return 'Y'; 
  if(i ==  58) return 'Z'; 
  if(i ==  59) return '['; 
  if(i ==  60) return '\\'; 
  if(i ==  61) return ']'; 
  if(i ==  62) return '^'; 
  if(i ==  63) return '_'; 
  if(i ==  64) return '`'; 
  if(i ==  65) return 'a'; 
  if(i ==  66) return 'b'; 
  if(i ==  67) return 'c'; 
  if(i ==  68) return 'd'; 
  if(i ==  69) return 'e'; 
  if(i ==  70) return 'f'; 
  if(i ==  71) return 'g'; 
  if(i ==  72) return 'h'; 
  if(i ==  73) return 'i'; 
  if(i ==  74) return 'j'; 
  if(i ==  75) return 'k'; 
  if(i ==  76) return 'l'; 
  if(i ==  77) return 'm'; 
  if(i ==  78) return 'n'; 
  if(i ==  79) return 'o'; 
  if(i ==  80) return 'p'; 
  if(i ==  81) return 'q'; 
  if(i ==  82) return 'r'; 
  if(i ==  83) return 's'; 
  if(i ==  84) return 't'; 
  if(i ==  85) return 'u'; 
  if(i ==  86) return 'v'; 
  if(i ==  87) return 'w'; 
  if(i ==  88) return 'x'; 
  if(i ==  89) return 'y'; 
  if(i ==  90) return 'z'; 
  if(i ==  91) return '{'; 
  if(i ==  92) return '|'; 
  if(i ==  93) return '}'; 
  if(i ==  94) return '~'; 
  return '\n';
}
