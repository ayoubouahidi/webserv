#include "Response.hpp"
#include <sstream>

Response::Response() : code(200), message("OK") {
    std::cout << "Response created with default constructor" << std::endl;
}

Response::~Response() {
    std::cout << "Response destroyed" << std::endl;
}

Response::Response(const Response& other) : code(other.code), message(other.message), headers(other.headers), body(other.body) {
    std::cout << "Response created with copy constructor" << std::endl;
}

Response& Response::operator=(const Response& other) {
    if (this != &other) {
        code = other.code;
        message = other.message;
        headers = other.headers;
        body = other.body;
    }
    return *this;
}

Response::Response(int code, const std::string& message) : code(code), message(message) {
    std::cout << "Response created with parameterized constructor" << std::endl;
}


int Response::getCode() const {
    return code;
}
const std::string& Response::getMessage() const {
    return message;
}
const std::map<std::string, std::string>& Response::getHeaders() const {
    return headers;
}
const std::string& Response::getBody() const {
    return body;
}

void Response::addHeader(const std::string& key, const std::string& value) {
    headers[key] = value;
}

void Response::setBody(const std::string& body) {
    this->body = body;
}

std::string Response::toString(const Response& response) const
{
    std::ostringstream oss;
    std::map<std::string, std::string>::const_iterator it = response.getHeaders().begin();
    while (it != response.getHeaders().end()) {
        oss << it->first << ": " << it->second << "\r\n";
        ++it;
    }
    oss << "\r\n" << response.getBody();
    return oss.str();
}

