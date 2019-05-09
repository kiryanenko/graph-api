#include <utility>

//
// Created by kiryanenko on 09.05.19.
//

#ifndef GRAPH_API_EXCEPTIONS_H
#define GRAPH_API_EXCEPTIONS_H

#include <string>
#include <exception>

using namespace std;

class NotFound: public exception
{
    string _error;
public:
    explicit NotFound(string error = "404 Not Found") : _error(std::move(error)) {}
    const char* what() const noexcept override { return _error.c_str(); }
};


class MethodNotAllowed: public exception
{
    string _error;
public:
    explicit MethodNotAllowed(string error = "405 Method Not Allowed") : _error(std::move(error)) {}
    const char* what() const noexcept override { return _error.c_str(); }
};


class Conflict: public exception
{
    string _error;
public:
    explicit Conflict(string error = "409 Conflict") : _error(std::move(error)) {}
    const char* what() const noexcept override { return _error.c_str(); }
};


class NotImplemented: public exception
{
    string _error;
public:
    explicit NotImplemented(string error = "501 Not Implemented") : _error(std::move(error)) {}
    const char* what() const noexcept override { return _error.c_str(); }
};

#endif //GRAPH_API_EXCEPTIONS_H
