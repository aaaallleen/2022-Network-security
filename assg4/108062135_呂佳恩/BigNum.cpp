#include "BigNum.hpp"
using namespace std;
// constructor
BigNum::BigNum() {
  BigNum(0);
}

BigNum::BigNum(long long input_number) {
  long long unsign_number;

  sgn = !(input_number < 0);

  unsign_number = (input_number < 0)? -input_number:input_number;

  while (unsign_number >= 16) {
    data.emplace_back(unsign_number & 15); // mod 16
    unsign_number = unsign_number >> 4; // div 16
  }
  data.emplace_back(unsign_number);
}

BigNum::BigNum(const string& input_string) {
  sgn = !(input_string.front() == '-');

  for (auto i = input_string.rbegin(), end = input_string.rend(); i != end; ++i) {
    if (*i >= '0' && *i <= '9') {
      data.emplace_back(*i - '0');
    } 
    else if (*i >= 'A' && *i <= 'F') {
      data.emplace_back(*i - 'A' + 10);
    } 
    else if (*i >= 'a' && *i <= 'f') {
      data.emplace_back(*i - 'a' + 10);
    }
  }
}

BigNum::BigNum(bool input_sgn, const vector<int8_t>& input_data) {
  sgn = input_sgn;
  data = input_data;
}

BigNum::BigNum(bool input_sgn, vector<int8_t>&& input_data) {
  sgn = input_sgn;
  data = move(input_data);
}


// logical operators
bool operator==(const BigNum& lhs, const BigNum& rhs) {
  return (lhs.sgn == rhs.sgn) && (BigNum::abs_compare(lhs, rhs) == EQUAL);
}

bool operator!=(const BigNum& lhs, const BigNum& rhs) {
  return !(lhs == rhs);
}

bool operator>(const BigNum& lhs, const BigNum& rhs) {
  int abs_cmp;

  if (lhs.sgn == rhs.sgn) {
    abs_cmp = BigNum::abs_compare(lhs, rhs);
    return ((lhs.sgn && abs_cmp == BIGGER) || (!lhs.sgn && abs_cmp == SMALLER));
  } else {
    return lhs.sgn;
  }
}

bool operator<(const BigNum& lhs, const BigNum& rhs) {
  return rhs > lhs;
}

bool operator>=(const BigNum& lhs, const BigNum& rhs) {
  return !(lhs < rhs);
}

bool operator<=(const BigNum& lhs, const BigNum& rhs) {
  return !(lhs > rhs);
}


// private method
int BigNum::abs_compare(const BigNum& lhs, const BigNum& rhs) {
  if (lhs.data.size() > rhs.data.size()) {
    return BIGGER;
  } else if (lhs.data.size() < rhs.data.size()) {
    return SMALLER;
  }

  // same size
  for (auto i = lhs.data.rbegin(), j = rhs.data.rbegin(), end = lhs.data.rend(); i != end; ++i, ++j) {
    if(*i > *j) {
      return BIGGER;
    } else if (*i < *j) {
      return SMALLER;
    }
  }

  return EQUAL;
}

void BigNum::discard_leading_zero(vector<int8_t>& input) {
  while (input.back() == 0 && input.size()!=1) {
    input.pop_back();
  }
}


// arithmetic operators
const BigNum operator+(const BigNum& lhs, const BigNum& rhs) {
  bool sgn;
  vector<int8_t> abs_result;
  unsigned long min_size;
  int8_t carry, sum;

  min_size = (lhs.data.size() < rhs.data.size())? lhs.data.size():rhs.data.size();

  // check is add or sub
  if (lhs.sgn == rhs.sgn) {
    // same sign
    sgn = lhs.sgn;
    carry = 0;

    // add all first
    for (unsigned long i = 0; i < min_size; i++) {
      sum = lhs.data[i] + rhs.data[i];
      abs_result.emplace_back(sum);
    }

    // insert remain digit
    if (lhs.data.size() > rhs.data.size()) {
      for (unsigned long i = min_size; i < lhs.data.size(); i++) {
        abs_result.emplace_back(lhs.data[i]);
      }
    } else {
      for (unsigned long i = min_size; i < rhs.data.size(); i++) {
        abs_result.emplace_back(rhs.data[i]);
      }
    }

    // handle carry
    carry = 0;
    for (unsigned long i = 0; i < abs_result.size(); i++) {
      sum = abs_result[i] + carry;
      abs_result[i] = sum & 15;
      carry = sum >> 4;
    }
    if (carry > 0) {
      abs_result.emplace_back(carry);
    }
  } else {
    if (!lhs.sgn) {
      return rhs - BigNum(!lhs.sgn, lhs.data);
    } else {
      return lhs - BigNum(!rhs.sgn, rhs.data);
    }
  }

  return BigNum(sgn,  move(abs_result));
}

const BigNum operator-(const BigNum& lhs, const BigNum& rhs) {
  bool sgn;
  vector<int8_t> abs_result;
  unsigned long min_size;

  // check is add or sub
  if (lhs.sgn != rhs.sgn) {
    // equal to do ADD operation
    return lhs + BigNum(!rhs.sgn, rhs.data);
  }
  else {
    if (BigNum::abs_compare(lhs, rhs) == EQUAL) {
      return BigNum(0);
    }
    else if (BigNum::abs_compare(lhs, rhs) == BIGGER) {
      sgn = lhs.sgn;
      // sub all first
      min_size = rhs.data.size();
      abs_result.resize(lhs.data.size());
      transform(lhs.data.begin(), lhs.data.begin() + min_size, rhs.data.begin(), abs_result.begin(),  minus<int8_t>());
      // insert remain digit
      copy(lhs.data.begin() + min_size, lhs.data.end(), abs_result.begin() + min_size);
    }
    else {
      sgn = !rhs.sgn;
      // sub all first
      min_size = lhs.data.size();
      abs_result.resize(rhs.data.size());
      transform(rhs.data.begin(), rhs.data.begin() + min_size, lhs.data.begin(), abs_result.begin(),  minus<int8_t>());
      // insert remain digit
      copy(rhs.data.begin() + min_size, rhs.data.end(), abs_result.begin() + min_size);
    }
    // handle borrow
    for (unsigned long i = 0; i < abs_result.size(); i++) {
      if (abs_result[i] < 0){
        abs_result[i] += 16;
        abs_result[i + 1]--;
      }
    }

    //discard redundant zero
    BigNum::discard_leading_zero(abs_result);
  }

  return BigNum(sgn,  move(abs_result));
}

const BigNum operator*(const BigNum& lhs, const BigNum& rhs) {
  bool sgn;
  vector<int8_t> abs_result;

  if(lhs == 0 || rhs == 0){
    return BigNum(0);
  }

  sgn = (lhs.sgn == rhs.sgn);

  abs_result.resize(lhs.data.size() + rhs.data.size(), 0);
  int16_t sum = 0;
  for(unsigned long i = 0; i < lhs.data.size(); i++){
    for(unsigned long j = 0; j < rhs.data.size(); j++){
      sum = abs_result[i + j] + lhs.data[i] * rhs.data[j];
      abs_result[i + j] = sum % 16; // sum
      abs_result[i + j + 1] += sum / 16; // carry
    }
  }
  //discard redundant zero
  BigNum::discard_leading_zero(abs_result);
  return BigNum(sgn, abs_result);
}

const BigNum operator/(const BigNum& lhs, const BigNum& rhs) {
  BigNum temp(0);
  BigNum remainder(true, lhs.data);
  BigNum divisor(true, rhs.data);
  BigNum quotient(0);
  quotient.sgn = (lhs.sgn == rhs.sgn);
  int8_t count = 0;
  while (remainder.data.size() != 0) {
    while(temp < divisor && remainder.data.size() != 0) {
      temp.data.insert(temp.data.begin(), remainder.data.back());
      remainder.data.pop_back();
      quotient.data.insert(quotient.data.begin(), 0);
    }
    BigNum::discard_leading_zero(temp.data);

    count = 0;
    while (temp >= divisor) {
      count ++;
      temp = temp - divisor;
    }
    quotient.data.front() = count;
  }
  BigNum::discard_leading_zero(quotient.data);
  // make -0 -> +0
  if (quotient.data.size()==1 && quotient.data.back()==0) {
    quotient.sgn = true;
  }
  return quotient;
}

const BigNum operator%(const BigNum& lhs, const BigNum& rhs) {
  BigNum temp(0);
  BigNum remainder(true, lhs.data);
  BigNum divisor(true, rhs.data);

  if (BigNum::abs_compare(lhs, rhs)==EQUAL) {
    return BigNum(0);
  }
  if (BigNum::abs_compare(lhs, rhs)==SMALLER) {
    return lhs;
  }
  while (remainder >= divisor) {
    temp.data.assign(remainder.data.end() - divisor.data.size(), remainder.data.end());
    remainder.data.erase(remainder.data.end() - divisor.data.size(), remainder.data.end());
    if (temp < divisor){
      temp.data.insert(temp.data.begin(), remainder.data.back());
      remainder.data.pop_back();
    }

    BigNum::discard_leading_zero(temp.data);

    while (temp >= divisor) {
      temp = temp - divisor;
    }
    remainder.data.insert(remainder.data.end(), temp.data.begin(), temp.data.end());
    temp.data.clear();
  }
  BigNum::discard_leading_zero(remainder.data);
  remainder.sgn = lhs.sgn;
  // make -0 -> +0
  if (remainder.data.size()==1 && remainder.data.back()==0) {
    remainder.sgn = true;
  }
  return remainder;
}


// output format  ostream& operator<<  ostream& os, const BigNum& rhs) {
std::ostream& operator<<(std::ostream& os, const BigNum& rhs) {
  if (!rhs.sgn) {
    os << "-";
  }

  for (auto i = rhs.data.rbegin(); i!= rhs.data.rend(); ++i ) {
    if (*i >= 10) {
      os << static_cast<char>(*i + 'A' - 10); // 10 -> 'a'
    } else {
      os << static_cast<char>(*i + '0'); // 1 -> '1'
    }
  }

  return os;
}
