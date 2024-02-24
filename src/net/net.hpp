#ifndef _NET_H_
#define _NET_H_

#include <format/format.hpp>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>
#include <iomanip>

class net
{
    public:
        struct header
        {
            const char* key;
            const char* value;

            header(const char* k, const char* v) : key(k), value(v) {}
        };

        struct response
        {
            int status_code;
            std::string headers;
            std::string body;
        };

        static std::string user_agent;
        static bool offline;
        static void initialize();

        static size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* body);
        static size_t header_callback(void* contents, size_t size, size_t nmemb, std::string* headers);

        static net::response http_request(std::string url, const std::string& method,
            std::vector<net::header> headers, const std::string& post_data = "");
            
        static char* generate_query_string(const std::unordered_map<char*, char*>& queryable);

};

#endif _NET_H_