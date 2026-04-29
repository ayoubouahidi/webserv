#include <iostream>
#include <string>
#include <unordered_map>

class Response {
    private:
        int code;
        std::string message;
        std::unordered_map<std::string, std::string> headers;
        std::string body;
    public:
        Response();
        ~Response();
        Response(const Response& other);
        Response& operator=(const Response& other);
        Response(int code, const std::string& message);
        void addHeader(const std::string& key, const std::string& value);
        void setBody(const std::string& body);
        void send() const;
        int getCode() const;
        const std::string& getMessage() const;
        const std::unordered_map<std::string, std::string>& getHeaders() const;
        const std::string& getBody() const;
};
