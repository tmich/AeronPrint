#pragma once
#include <string>
#include <exception>

class ConnectionException : std::exception {};

class DatabaseException : std::exception
{
	std::string message;
public:
	DatabaseException(const std::string& message);
	virtual const char * what() const throw() { return message.c_str(); }
};