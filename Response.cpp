#include "Response.hpp"
#include <sstream>

Response::Response() : code(200), _statusMessage("OK") {
    std::cout << "Response created with default constructor" << std::endl;
}

Response::~Response() {
    std::cout << "Response destroyed" << std::endl;
}

Response::Response(const Response& other) : code(other.code), _statusMessage(other._statusMessage), headers(other.headers), body(other.body) {
    std::cout << "Response created with copy constructor" << std::endl;
}

Response& Response::operator=(const Response& other) {
    if (this != &other) {
        code = other.code;
        _statusMessage = other._statusMessage;
        headers = other.headers;
        body = other.body;
    }
    return *this;
}


int Response::getCode() const {
    return code;
}


std::string Response::getResponse(const Response& response) const {
    std::stringstream ss;
    ss << "HTTP/1.1 " << response.code << " " << response._statusMessage << "\r\n";
    for (const auto& header : response.headers) {
        ss << header.first << ": " << header.second << "\r\n";
    }
    ss << "\r\n" << response.body;
    return ss.str();
}

std::map<int, std::string> Response::_initStatusMap() {
    std::map<int, std::string> statusMap;
    statusMap[200] = "OK";
    statusMap[404] = "Not Found";
    statusMap[500] = "Internal Server Error";
    return statusMap;
}

std::map<std::string, std::string> Response::_initMimeMap() {
    std::map<std::string, std::string> mimeMap;
    mimeMap["html"] = "text/html";
    mimeMap["css"] = "text/css";
    mimeMap["js"] = "application/javascript";
    mimeMap["jpg"] = "image/jpeg";
    mimeMap["png"] = "image/png";
    return mimeMap;
}

std::map<int, std::string> Response::_statusMap = Response::_initStatusMap();
std::map<std::string, std::string> Response::_mimeMap = Response::_initMimeMap();


std::string Response::_buildHeader(size_t bodySize) const {
    std::stringstream ss;
    ss << "HTTP/1.1 " << code << " " << _statusMessage << "\r\n";
    for (const auto& header : headers) {
        ss << header.first << ": " << header.second << "\r\n";
    }
    ss << "Content-Length: " << bodySize << "\r\n";
    ss << "\r\n";
    return ss.str();
}

void Response::_buildError(int code) {
    this->code = code;
    _statusMessage = _statusMap[code];
    body = "<html><body><h1>" + std::to_string(code) + " " + _statusMessage + "</h1></body></html>";
    headers["Content-Type"] = "text/html";
}

std::string Response::_getMimeType(const std::string& path) const {
    size_t dotPos = path.find_last_of('.');
    if (dotPos != std::string::npos) {
        std::string ext = path.substr(dotPos + 1);
        if (_mimeMap.count(ext)) {
            return _mimeMap[ext];
        }
    }
    return "application/octet-stream";
}

std::string Response::_getDate() const {
    return "Wed, 21 Oct 2015 07:28:00 GMT";
}

std::string Response::_readFile(const std::string& path) const {
    return "File content of " + path;
}


void Response::_resetHeaders() {
    headers.clear();
}
