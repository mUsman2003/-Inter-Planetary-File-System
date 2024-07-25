#include <string>
#include <iostream>

using namespace std;
class BigInt {
public:
    string value;

    // Helper function to remove leading zeros from a string
    static void removeLeadingZeros(string& str);

public:

    // Constructors
    BigInt(int val = 0);
    BigInt(const  string& text);
    BigInt(const BigInt& copy);

    // Binary Operators and Arithmetic Operators
    BigInt operator+(const BigInt& val) const;
    BigInt operator+(int val) const;

    BigInt operator-(const BigInt& val) const;
    BigInt operator-(int val) const;

    BigInt operator*(const BigInt& val) const;
    BigInt operator*(const int val) const;

    // Compound Assignment Operators
    BigInt& operator+=(const BigInt& rhs);
    BigInt& operator-=(const BigInt& rhs);
    BigInt& operator*=(const BigInt& rhs);

    // Logical Operators
    bool operator==(const BigInt& val) const;
    bool operator==(const int& val) const;
    bool operator!=(const BigInt& val) const;
    bool operator!=(const int& val) const;
    bool operator<(const BigInt& val) const;
    bool operator<(const int& val) const;
    bool operator<=(const BigInt& val) const;
    bool operator<=(const int& val) const;
    bool operator>(const BigInt& val) const;
    bool operator>(const int& val) const;
    bool operator>=(const BigInt& val) const;
    bool operator>=(const int& val) const;
    // Unary Operators
    BigInt& operator++();      // Pre-increment Operator
    BigInt operator++(int);    // Post-increment Operator
    BigInt& operator--();      // Pre-decrement Operator
    BigInt operator--(int);    // Post-decrement Operator

    BigInt power(int a);
    BigInt mod(BigInt val, const BigInt& val2);
    BigInt div(const BigInt& lhs, const BigInt& rhs) 
    {
        // Check for division by zero
        if (rhs.value == "0") {
            std::cerr << "Error: Division by zero." << std::endl;
            // You might want to handle this error differently based on your needs
            exit(EXIT_FAILURE);
        }

        // Remove leading zeros from the operands
        std::string dividend = lhs.value;
        std::string divisor = rhs.value;
        BigInt::removeLeadingZeros(dividend);
        BigInt::removeLeadingZeros(divisor);

        // Perform division logic here
        std::string quotient;
        BigInt currentDividend;

        for (char digit : dividend) 
        {
            currentDividend.value.push_back(digit);
            BigInt partialQuotient;
            int count = 0;

            while (currentDividend >= rhs) {
                currentDividend = currentDividend - rhs;
                count++;
            }

            partialQuotient.value = std::to_string(count);
            quotient += partialQuotient.value;
        }

        // Remove leading zeros from the quotient
        BigInt::removeLeadingZeros(quotient);

        return BigInt(quotient);
    }

    BigInt modwithstring(BigInt val, const string& val2);

    BigInt newmod(BigInt val, const BigInt& val2);

    BigInt operator/(const BigInt& val) const 
    {
        string number = value;
        string divisor = val.value;
        int idx = 0;
        string temp = number.substr(idx, divisor.length());

        while (stoi(temp) < stoi(divisor))
            temp += number[++idx];

        string quotient;

        while (number.size() > idx) {
            int i = 0;
            while (stoi(temp) >= stoi(divisor)) {
                temp = to_string(stoi(temp) - stoi(divisor));
                i++;
            }
            quotient += to_string(i);

            if (number.size() == idx)
                break;
            temp += number[++idx];
        }

        if (quotient.length() == 0)
            return 0;

        return BigInt(quotient);
    }

    // Conversion Operator
    operator string();    // Return value of the BigInt as string
    // Destructor
    ~BigInt();
};

// Non-member functions
ostream& operator<<(ostream& output, const BigInt& val);
istream& operator>>(istream& input, BigInt& val);

BigInt::BigInt(int val) : value(to_string(val)) {}
BigInt::BigInt(const  string& text) : value(text) {
    removeLeadingZeros(value);
}
BigInt::BigInt(const BigInt& copy) : value(copy.value) {}



void BigInt::removeLeadingZeros(string& str) {
    size_t pos = 0;
    while (pos < str.length() && str[pos] == '0') {
        pos++;
    }
    if (pos > 0) {
        str.erase(0, pos); //it will remove zeros from the string
    }
}




BigInt BigInt::operator+(const BigInt& val) const {
    string a = value;
    string b = val.value;
    if (a.length() < b.length())
        swap(a, b);
    int carry = 0;
    string result;
    int i = a.length() - 1;
    int j = b.length() - 1;
    while (i >= 0 || j >= 0 || carry > 0) {
        int sum = carry;
        if (i >= 0)
            sum += a[i] - '0';
        if (j >= 0)
            sum += b[j] - '0';
        carry = sum / 10;
        sum %= 10;
        result = char(sum + '0') + result;
        i--;
        j--;
    }
    return BigInt(result);
}

BigInt BigInt::power(int a)
{
    BigInt p = *this;
    BigInt q = *this;


    for (int i = 0; i < a - 1; i++)
        p *= q;


    return p;
}

BigInt BigInt::mod(BigInt val, const BigInt& val2)
{
    BigInt p = 0;
    while (val > val2)
    {
        val = val - val2;
        //cout << val << endl;
    }
    return val;
}
BigInt BigInt::newmod(BigInt val, const BigInt& val2)
{
    BigInt p = val / val2;
    p = p * val2;
    p = val - p;

    return p;
}
BigInt BigInt::modwithstring(BigInt val, const string& val2)
{
    BigInt p = 0;
    while (val > val2)
    {
        val = val - val2;
        //cout << val << endl;
    }
    return val;
}




BigInt BigInt::operator+(int val) const {
    return *this + BigInt(val);
}



BigInt BigInt::operator-(const BigInt& val) const {
    string a = value;
    string b = val.value;
    if (a == b)
        return BigInt(0);
    bool isNegative = false;
    if (a.length() < b.length() || (a.length() == b.length() && a < b)) {
        swap(a, b);
        isNegative = true;
    }
    int borrow = 0; //we will need borrow for greter value to subtract from 
    string result;
    int i = a.length() - 1;
    int j = b.length() - 1;
    while (i >= 0 || j >= 0 || borrow > 0) {
        int diff = borrow;
        if (i >= 0)
            diff += a[i] - '0';
        if (j >= 0)
            diff -= b[j] - '0';
        if (diff < 0) {
            borrow = -1;
            diff += 10;
        }
        else {
            borrow = 0;
        }
        result = char(diff + '0') + result;
        i--;
        j--;
    }
    BigInt res(result);
    if (isNegative)
        res.value = "-" + res.value;
    return res;
}
BigInt BigInt::operator-(int val) const {
    return *this - BigInt(val);
}



BigInt BigInt::operator*(const BigInt& val) const {
    string a = value;
    string b = val.value;
    int n = a.length();
    int m = b.length();
    string result(n + m, '0');
    for (int i = n - 1; i >= 0; i--) {
        int carry = 0;
        for (int j = m - 1; j >= 0; j--) {
            int prod = (a[i] - '0') * (b[j] - '0') + (result[i + j + 1] - '0') + carry;
            carry = prod / 10;
            result[i + j + 1] = char(prod % 10 + '0');
        }
        result[i] = char(carry + '0');
    }
    removeLeadingZeros(result);
    return BigInt(result);
}

BigInt BigInt::operator*(const int val) const {

    BigInt p = val;
    return (*this * p);

}


BigInt& BigInt::operator+=(const BigInt& rhs) {
    *this = *this + rhs;
    return *this;
}




BigInt& BigInt::operator-=(const BigInt& rhs) {
    string a = value;
    string b = rhs.value;
    bool isNegative = false;
    if (a.length() < b.length() || (a.length() == b.length() && a < b)) {
        swap(a, b);
        isNegative = true;
    }
    int borrow = 0;
    string result;
    int i = a.length() - 1;
    int j = b.length() - 1;
    while (i >= 0 || j >= 0 || borrow > 0) {
        int diff = borrow;
        if (i >= 0)
            diff += a[i] - '0';
        if (j >= 0)
            diff -= b[j] - '0';

        if (diff < 0) {
            borrow = -1;
            diff += 10;
        }
        else {
            borrow = 0;
        }

        result = char(diff + '0') + result;

        i--;
        j--;
    }

    BigInt res(result);
    if (isNegative)
        res.value = "-" + res.value;

    return res;
}




BigInt& BigInt::operator*=(const BigInt& rhs) {
    *this = *this * rhs;
    return *this;
}

bool BigInt::operator==(const BigInt& val) const {
    return value == val.value;
}


bool BigInt::operator==(const int& val) const {
    BigInt p = val;
    return *this == p;
}

//////////////////////////////////////////////////////////////////////////////////////

bool BigInt::operator!=(const BigInt& val) const {
    return !(*this == val);
}

bool BigInt::operator!=(const int& val) const {
    return !(*this == val);
}



/////////////////////////////////////////////////////////////////////////////////////

bool BigInt::operator<(const BigInt& val) const {
    if (value.length() < val.value.length())
        return true;
    else if (value.length() > val.value.length())
        return false;
    else
        return value < val.value;
}

bool BigInt::operator<(const int& val) const
{
    BigInt p = val;
    return (*this < p);
}


//////////////////////////////////////////////////////////////////////////////////////
bool BigInt::operator<=(const BigInt& val) const {
    return *this < val || *this == val;
}

bool BigInt::operator<=(const int& val) const {

    return *this < val || *this == val;
}



/////////////////////////////////////////////////////////////////////////////////////

bool BigInt::operator>(const BigInt& val) const {
    return !(*this <= val);
}

bool BigInt::operator>(const int& val) const {
    return !(*this <= val);
}


///////////////////////////////////////////////////////////////////////////////////

bool BigInt::operator>=(const BigInt& val) const {
    return !(*this < val);
}

bool BigInt::operator>=(const int& val) const {
    return !(*this < val);
}





///////////////////////////////////////////////////////////////////////////////////
BigInt& BigInt::operator++() {
    *this += 1;
    return *this;
}
BigInt BigInt::operator++(int) {
    BigInt temp = *this;
    ++(*this);
    return temp;
}
BigInt& BigInt::operator--() {
    *this -= 1;
    return *this;
}
BigInt BigInt::operator--(int) {
    BigInt temp = *this;
    --(*this);
    return temp;
}



BigInt::operator string() {
    return value;
}


BigInt::~BigInt() {}
ostream& operator<<(ostream& output, const BigInt& val) {
    output << val.value;
    return output;
}
istream& operator>>(istream& input, BigInt& val) {
    string str;
    input >> str;
    val = BigInt(str);
    return input;
}