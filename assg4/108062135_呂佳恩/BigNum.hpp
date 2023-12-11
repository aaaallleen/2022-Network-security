#ifndef BigNumber_hpp
#define BigNumber_hpp

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

#define BIGGER 1
#define SMALLER -1
#define EQUAL 0

// Big number class definition
class BigNum{
private:
  // member
  bool sgn;
  std::vector<int8_t> data;
  // private handle method
  static int abs_compare(const BigNum&, const BigNum&);
  static void discard_leading_zero(std::vector<int8_t>&);

public:
  // constructors
  BigNum();
  BigNum(long long); // directly convert from an int
  BigNum(const std::string&);
  BigNum(bool, const std::vector<int8_t>&);
  BigNum(bool, std::vector<int8_t>&&);

  // overloaded logical operators as member functions
  friend bool operator==(const BigNum&, const BigNum&);
  friend bool operator!=(const BigNum&, const BigNum&);
  friend bool operator>(const BigNum&, const BigNum&);
  friend bool operator<(const BigNum&, const BigNum&);
  friend bool operator>=(const BigNum&, const BigNum&);
  friend bool operator<=(const BigNum&, const BigNum&);

  // overloaded arithmetic operators as member functions
  friend const BigNum operator+(const BigNum&, const BigNum&);
  friend const BigNum operator-(const BigNum&, const BigNum&);
  friend const BigNum operator*(const BigNum&, const BigNum&);
  friend const BigNum operator/(const BigNum&, const BigNum&);
  friend const BigNum operator%(const BigNum&, const BigNum&);

  // ouput format for BigNum
  friend std::ostream& operator<<(std::ostream&, const BigNum&);
};


#endif /* BigNumber_hpp */
