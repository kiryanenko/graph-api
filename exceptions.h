#include <utility>

//
// Created by kiryanenko on 09.05.19.
//

#ifndef GRAPH_API_EXCEPTIONS_H
#define GRAPH_API_EXCEPTIONS_H

#include <string>
#include <exception>

using namespace std;


class BadRequest: public exception
{
    string _error;
public:
    explicit BadRequest(const string& error = "Bad Request") : _error("400 " + error) {}
    const char* what() const noexcept override { return _error.c_str(); }
};


class NotFound: public exception
{
    string _error;
public:
    explicit NotFound(const string& error = "Not Found") : _error("404 " + error) {}
    const char* what() const noexcept override { return _error.c_str(); }
};


class Conflict: public exception
{
    string _error;
public:
    explicit Conflict(const string& error = "Conflict") : _error("409 " + error) {}
    const char* what() const noexcept override { return _error.c_str(); }
};


class QueueError: public exception
{
    string _error;
public:
    explicit QueueError(const string& error = "Queue error") : _error("500 " + error) {}
    const char* what() const noexcept override { return _error.c_str(); }
};


class CommandOverflowError: public exception
{
    string _error;
public:
    explicit CommandOverflowError(const string& error = "500 Command overflow error") : _error("500 " + error) {}
    const char* what() const noexcept override { return _error.c_str(); }
};


class InsufficientStorage: public exception
{
    string _error;
public:
    explicit InsufficientStorage(const string& error = "Insufficient Storage") : _error("507 " + error) {}
    const char* what() const noexcept override { return _error.c_str(); }
};


#endif //GRAPH_API_EXCEPTIONS_H
