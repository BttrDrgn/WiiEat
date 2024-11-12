#include "net.hpp"
#include <main.hpp>
#include <menus/console_menu/console_menu.hpp>

bool net::offline = true;
std::string net::user_agent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/130.0.0.0 Safari/537.36";

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

//


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

net::response net::http_request(std::string url, const std::string& method,
            std::vector<net::header> headers, const std::string& post_data)
{
    CURL* curl = curl_easy_init();
    response response;
    if (!curl) return response;

    struct curl_slist* headerlist = nullptr;

    url = format::replace(url.c_str(), " ", "%20");
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &(response.body));

    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &(response.headers));

    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip");

    if(method == "POST" || method == "PUT")
    {
        headers.emplace_back("Content-Type", "application/json;charset=UTF-8");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
    }

    headers.emplace_back("Accept-Encoding", "gzip, deflate, br, zstd");
    headers.emplace_back("Accept-Language", "en-US,en;q=0.9");
    headers.emplace_back("Origin", "https://www.grubhub.com");
    headers.emplace_back("Priority", "u=1, i");
    headers.emplace_back("Referer", "https://www.grubhub.com/");
    headers.emplace_back("Sec-Fetch-Dest", "empty");
    headers.emplace_back("Sec-Fetch-Mode", "cors");
    headers.emplace_back("Sec-Fetch-Site", "same-site");
    headers.emplace_back("User-Agent", net::user_agent.c_str());

    if (method != "OPTIONS")
    {
        headers.emplace_back("Sec-Ch-Ua", "\"Chromium\";v=\"130\", \"Google Chrome\";v=\"130\", \"Not?A_Brand\";v=\"99\"");
        headers.emplace_back("Sec-Ch-Ua-Mobile", "?0");
        headers.emplace_back("Sec-Ch-Ua-Platform", "\"Windows\"");
    }

    if (method == "GET")
    {
        headers.emplace_back("If-Modified-Since", "0");
        headers.emplace_back("Cache-Control", "no-cache");
    }

    for (const auto& header : headers)
    {
        std::string header_string = format::va("%s: %s", header.key, header.value);
        headerlist = curl_slist_append(headerlist, header_string.c_str());
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

    CURLcode res = curl_easy_perform(curl);

    if (res == CURLE_OK)
    {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &(response.status_code));
    }
    else
    {
        console_menu::write_line(format::va("Net Err: %s", curl_easy_strerror(res)));
    }

    console_menu::write_line(format::va("[%s] %i %s", method.c_str(), response.status_code, url.c_str()));
    console_menu::write_line(response.body.c_str());

    curl_slist_free_all(headerlist);
    curl_easy_cleanup(curl);

    return response;
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