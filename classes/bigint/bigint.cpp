#include "bigint.h"

BigInt::BigInt(int tmp)
{
  int digit = 0;
  do
  {
    m_int[digit] = tmp % 10;
    tmp /= 10;
    digit++;
  }while(tmp > 0);

  return;
}

BigInt::BigInt(string tmp)
{
  int digit = 0;
  for(int i = tmp.size() - 1; i >= 0; i--)
  {
    if(isdigit(tmp[i]))
    {
      m_int[digit] = tmp[i] - '0';
      digit++;
    }
    else
    {
      cout << "Trying to initiate a BigInt with a non-int string" << endl;
      return;
    }
  }

  return;
}

BigInt::BigInt(BigInt* tmp)
{
  for(int i = 0; i < BIGINT_MAXDIGITS; i++)
  {
    m_int[i] = tmp->get_digit(i);
  }

  return;
}

void BigInt::set(int tmp)
{
  if(tmp < 0)
  {
    cout << "Trying to set BigInt to a negative int." << endl;
    return;
  }

  int digit = 0;
  do
  {
    m_int[digit] = tmp % 10;
    tmp /= 10;
    digit++;
  }while(tmp > 0);

  while(digit < BIGINT_MAXDIGITS)
  {
    m_int[digit] = 0;
    digit++;
  }

  return;
}

void BigInt::set(BigInt* tmp)
{
  for(int i = 0; i < BIGINT_MAXDIGITS; i++)
  {
    m_int[i] = tmp->get_digit(i);
  }

  return;
}

//print the larger digits first
void BigInt::print()
{
  bool started = false;
  for(int i = BIGINT_MAXDIGITS - 1; i >= 0; i--)
  {
    if(!started)
    {
      if(m_int[i] != 0)
      {
        started = true;
        cout << m_int[i];
      }
    }
    else cout << m_int[i];
  }

  if(!started) cout << "0";

  return;
}

//print the larger digits first
string BigInt::str_value()
{
  string tmp = "";
  bool started = false;
  for(int i = BIGINT_MAXDIGITS - 1; i >= 0; i--)
  {
    if(started)
      tmp += m_int[i];
    else
    {
      if(m_int[i] != 0)
      {
        started = true;
        tmp += to_string(m_int[i]);
      }
    }
  }
  return tmp;
}

//returns -1 for 0 value
int BigInt::magnitude()
{
  int answer = -1;
  for(int i = 0; i < BIGINT_MAXDIGITS; i++)
  {
    if(m_int[i] != 0)
      answer = i;
  }
  return answer;
}

int BigInt::get_digit(int i)
{
  if(i >= BIGINT_MAXDIGITS)
  {
    cout << "Request for digit greater than BIGINT_MAXDIGIT" << endl;
    return -1;
  }
  if(i < 0)
  {
    cout << "Request for negative digit in a BIGINT" << endl;
    return -1;
  }

  return m_int[i];
}

void BigInt::add(int addition)
{
  int digit = 0;
  int carry = 0;
  do
  {
    m_int[digit] = m_int[digit] + (addition % 10) + carry;

    if(m_int[digit] >= 10)
    {
      m_int[digit] -= 10;
      carry = 1;
    }
    else carry = 0;
    
    digit++;
    addition /= 10;

  }while(addition > 0 || carry == 1);

  return;
}

void BigInt::add(BigInt* addition)
{
  int carry = 0;
  for(int i = 0; i < BIGINT_MAXDIGITS; i++)
  {
    m_int[i] = m_int[i] + addition->get_digit(i) + carry;
    carry = m_int[i] / 10;
    m_int[i] = m_int[i] % 10;
  }

  return;
}

void BigInt::multiply(int multiplier)
{
  if(multiplier < 0)
  {
    cout << "BigInt does not support multiplying by negative numbers" << endl;
    return;
  }
  if(multiplier == 0)
  {
    for(int i = 0; i < BIGINT_MAXDIGITS; i++) m_int[i] = 0;
    return;
  }

  std::array<int, BIGINT_MAXDIGITS> answer = {0};
  int carry;
  int multiplied_digits = 0;
  while(multiplier > 0)
  {
    int multiplying_digit = multiplier % 10;
    carry = 0;

    if(multiplying_digit != 0)
    {
      for(int i = 0; i < BIGINT_MAXDIGITS; i++) //m_int[0] is ones digit, m_int[1] is 10s digit, m_int[2] is 100s digit, etc...
      {
        int single_digit_answer = (m_int[i] * multiplying_digit) + carry;

        answer[i + multiplied_digits] += single_digit_answer % 10;
        carry                          = single_digit_answer / 10;
        if(answer[i + multiplied_digits] >= 10)
        {
          answer[i + multiplied_digits] -= 10;
          carry++;
        }
      }
    }

    multiplied_digits++;
    multiplier /= 10;
  }

  m_int = answer;

  return;
}

/*
void BigInt::multiply(BigInt* multiplier)
{
  int multiplying_digits = multiplier->magnitude();

  std::array<int, BIGINT_MAXDIGITS> answer = {0};
  int carry = 0;
  int multiplied_digits = 0;
  tmp = multiplier;
  while(multiplied_digits < multiplying_digits)
  {
    int multiplying_digit = tmp % 10;

    if(multiplying_digit != 0)
    {
      for(int i = 0; i < BIGINT_MAXDIGITS; i++) //m_int[0] is ones digit, m_int[1] is 10s digit, m_int[2] is 100s digit, etc...
      {
        int single_digit_answer = (m_int[i] * multiplying_digit) + carry;

        answer[i + multiplied_digits] += single_digit_answer % 10;
        carry = single_digit_answer / 10;
        if(answer[i + multiplied_digits] >= 10)
        {
          answer[i + multiplied_digits] -= 10;
          carry++;
        }
      }
    }

    carry = 0;
    tmp /= 10;
    multiplied_digits++;
  }

  m_int = answer;

  return;
}
*/

bool BigInt::equal_to(int operand)
{
  //A BigInt cannot be negative
  //Will always be NOT EQUAL to any negative number
  if(operand < 0) return false;

  int magnitude = this->magnitude();
  for(int i = 0; i < magnitude; i++)
  {
    if(m_int[i] != operand % 10) return false;
  }

  //Made it throough the whole number without missing
  return true;
}

bool BigInt::equal_to(BigInt* operand)
{
  int magnitude = this->magnitude();
  if(magnitude != operand->magnitude()) return false;

  for(int i = 0; i < magnitude; i++)
  {
    if(m_int[i] != operand->get_digit(i)) return false;
  }

  //Made it throough the whole number without missing
  return true;
}

bool BigInt::greater_than(int operand)
{
  //A BigInt cannot be negative
  //Will always be greater than any negative number
  if(operand < 0) return true;

  bool answer;
  bool dominance_set = false;
  int magnitude = this->magnitude();

  for(int i = 0; i < magnitude; i++)
  {
    int operand_digit = operand % 10;
    if(m_int[i] > operand_digit)
    {
      dominance_set = true;
      answer = true;
    }
    if(m_int[i] < operand_digit)
    {
      dominance_set = true;
      answer = false;
    }
    /*
    if(m_int[i] == operand_digit)
    {
      nothing changes
    }
    */

    operand = operand / 10;
    if(operand == 0) return true;
  }
  if(operand >= 10) return false;

  //they are the same magnitude

  if(dominance_set) return answer;
  else /* the numbers are equal */ return false;
}

bool BigInt::greater_than(BigInt* operand)
{
  for(int i = BIGINT_MAXDIGITS - 1; i >= 0; i--)
  {
    if(m_int[i] == operand->get_digit(i)) continue;
    if(m_int[i] >  operand->get_digit(i)) return true;
    if(m_int[i] <  operand->get_digit(i)) return false;
  }

  //Got through the whole thing without returning -> numbers are equal
  return false;
}

bool BigInt::less_than(int operand)
{
  //A BigInt cannot be negative
  //Will always be greater than any negative number
  if(operand < 0) return false;

  bool answer;
  bool dominance_set = false;
  int magnitude = this->magnitude();

  for(int i = 0; i < magnitude; i++)
  {
    int operand_digit = operand % 10;
    if(m_int[i] > operand_digit)
    {
      dominance_set = true;
      answer = false;
    }
    if(m_int[i] < operand_digit)
    {
      dominance_set = true;
      answer = true;
    }
    /*
    if(m_int[i] == tmp_digit)
    {
      nothing changes
    }
    */

    operand = operand / 10;
    if(operand == 0) return true;
  }
  if(operand >= 10) return false;

  //they are the same magnitude

  if(dominance_set) return answer;
  else /* the numbers are equal */ return false;
}

bool BigInt::less_than(BigInt* operand)
{
  for(int i = BIGINT_MAXDIGITS - 1; i >= 0; i--)
  {
    if(m_int[i] == operand->get_digit(i)) continue;
    if(m_int[i] >  operand->get_digit(i)) return false;
    if(m_int[i] <  operand->get_digit(i)) return true;
  }

  //Got through the whole thing without returning -> numbers are equal
  return false;
}
