// © 2017 Loris Witschard

/////////////////////////////////////
//   STRING TO NUMBER
/////////////////////////////////////

#include <string>

template<typename T>
T ston(const std::string & str) = delete;

template<>
int ston(const std::string & str)
{
    return std::stoi(str);
}

template<>
unsigned int ston(const std::string & str)
{
    return std::stoul(str);
}

template<>
long ston(const std::string & str)
{
    return std::stol(str);
}

template<>
long long ston(const std::string & str)
{
    return std::stoll(str);
}

template<>
unsigned long ston(const std::string & str)
{
    return std::stoul(str);
}

template<>
unsigned long long ston(const std::string & str)
{
    return std::stoull(str);
}

template<>
float ston(const std::string & str)
{
    return std::stof(str);
}

template<>
double ston(const std::string & str)
{
    return std::stod(str);
}

template<>
long double ston(const std::string & str)
{
    return std::stold(str);
}
