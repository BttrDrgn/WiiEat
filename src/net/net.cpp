#include "net.hpp"
#include <main.hpp>
#include <menus/console_menu/console_menu.hpp>

bool net::offline = true;

void net::initialize()
{
    curl_global_init(CURL_GLOBAL_ALL);
            
    //Open socket
    int socket_init_success = -1;
    for (int attempts = 0; attempts < 3; attempts++)
    {
        socket_init_success = wiisocket_init();
        if (socket_init_success == 0) 
        {
            break;
        }
    }

    if (socket_init_success != 0)
    {
        return;
    }

    //Get ip address
    u32 ip = 0;
    for (int attempts = 0; attempts < 3; attempts++)
    {
        ip = gethostid();
        if (ip)
        {
            break;
        }
    }
    
    if (!ip)
    {
        return;
    }

    offline = false;
}

net::response net::http_request(std::string url, char* query, const char* method, std::vector<net::header> headers, char* err)
{
    CURL* curl = curl_easy_init();
    net::response response;

    if (!curl)
    {
        err = "curl easy init failed";
        return response;
    }

    if(!strcmp(method, "GET"))
    {
        if(strcmp(query, ""))
        {
            url = format::va("%s?%s", url, query);
        }
    }
    else if(!strcmp(method, "POST"))
    {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, query);

        headers.emplace_back("Content-Type", "application/json");
    }

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
    curl_easy_setopt(curl, CURLOPT_URL, url.data());
    curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, err);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, net::write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &(response.body));

    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, net::header_callback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &(response.headers));

    struct curl_slist *headerlist = NULL;
    for (const auto &header : headers)
    {
        std::string header_string = format::va("%s: %s", header.key, header.value);
        headerlist = curl_slist_append(headerlist, header_string.data());
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

    CURLcode res = curl_easy_perform(curl);

    if (res == CURLE_OK)
    {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &(response.status_code));
    }

    curl_slist_free_all(headerlist);
    curl_easy_cleanup(curl);

    console_menu::write_line(format::va("[%s] %i : %s", method, response.status_code, url.data()));
    return response;
}

size_t net::write_callback(void* contents, size_t size, size_t nmemb, std::string* body)
{
    size_t totalSize = size * nmemb;
    body->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

size_t net::header_callback(void* contents, size_t size, size_t nmemb, std::string* headers)
{
    size_t totalSize = size * nmemb;
    headers->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

char* net::generate_query_string(const std::unordered_map<char*, char*>& queryable)
{
    const char separator = '&';
    const char assignment = '=';

    // Calculate the total length needed for the query string
    size_t totalLength = 1; // For null terminator
    for (const auto& pair : queryable)
    {
        totalLength += strlen(pair.first) + strlen(pair.second) + 2; // +2 for '=' and '&'
    }

    if (totalLength == 1)
    {
        return nullptr; // No elements in the map
    }

    // Allocate memory for the query string
    char* queryString = new char[totalLength];
    char* current = queryString;

    // Construct the query string
    for (const auto& pair : queryable) 
    {
        // Copy the key
        const size_t keyLength = strlen(pair.first);
        memcpy(current, pair.first, keyLength);
        current += keyLength;
        *current++ = assignment;

        // Copy the value
        const size_t valueLength = strlen(pair.second);
        memcpy(current, pair.second, valueLength);
        current += valueLength;
        *current++ = separator;
    }

    // Replace the last '&' with null terminator
    *(current - 1) = '\0';

    return queryString;
}