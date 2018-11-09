#ifndef GEEXCEPTION_H
#define GEEXCEPTION_H

#include <exception>
#include <string>

class geException : public std::exception
{
public:
    geException(const std::string &errorStr="") throw() : mErrorStr(errorStr) {}
    virtual ~geException() {}

    virtual const char* what() const throw(){ return mErrorStr.c_str(); }
    virtual const std::string toString() const throw(){ return mErrorStr; }
    geException extend(const std::string &message) const { return geException(message + " : " + mErrorStr); }
protected:
    const std::string mErrorStr;
};

#endif // GEEXCEPTION_H
