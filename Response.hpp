#include <iostream>
#include <string>
#include <unordered_map>
#include <map>


union Body
{
    std::string body_string;
    std::iostream body_stream;
    /* data */
};

class Response {
    private:
        int code;
        std::string message;
        std::map<std::string, std::string> headers;
        std::string body;
        int fd;
        int flag_fg;

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
        const std::map<std::string, std::string>& getHeaders() const;
        const std::string& getBody() const;
        std::string toString(const Response& response) const;

// 
// get 
// post 
};
