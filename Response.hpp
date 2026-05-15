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
        std::string _statusMessage;
        std::map<std::string, std::string> headers;
        std::string _response;
        std::string _path;
        std::string _contentType;
        std::string _location;
        std::string _allow;
        std::string body;

        static std::map<int, std::string>               _statusMap;
        static std::map<std::string, std::string>       _mimeMap;


        std::string _buildHeader(size_t bodySize) const;
        void        _buildError(int code);
        std::string _getMimeType(const std::string& path) const;
        std::string _getDate() const;
        std::string _readFile(const std::string& path) const;
        bool        _fileExists(const std::string& path) const;
        void        _resetHeaders();

       
        static std::map<int, std::string>             _initStatusMap();
        static std::map<std::string, std::string>     _initMimeMap();

        
    public:
        Response();
        ~Response();
        Response(const Response& other);
        Response& operator=(const Response& other);
       
        int getCode() const;

        std::string getResponse(const Response& response) const;

// 
// get 
// post 
};
