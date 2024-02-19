#ifndef _NET_H_
#define _NET_H_

#include <main.hpp>
#include <utils/format.hpp>
#include <vector>
#include <string>

class net
{
    public:
        struct header
        {
            const char* key;
            const char* value;

            header(const char* k, const char* v)
            {
                key = k;
                value = v;
            }
        };

        struct memory
        {
            char* response;
            size_t size;
        };

        static bool offline;

        static size_t cb(void *data, size_t size, size_t nmemb, void *clientp)
        {
            size_t realsize = size * nmemb;
            struct memory *mem = (struct memory *)clientp;

            char* ptr = (char*)realloc(mem->response, mem->size + realsize + 1);
            if(ptr == NULL)
            {
                return 0;
            }

            mem->response = ptr;
            memcpy(&(mem->response[mem->size]), data, realsize);
            mem->size += realsize;
            mem->response[mem->size] = 0;

            return realsize;
        }

        static char* http_request(char* url, char* method, const std::vector<header> &headers, char* err)
        {
            if(offline) return "OFFLINE";

            CURL* curl = curl_easy_init();
            if (!curl)
            {
                err = "curl easy init failed";
                return NULL;
            }
            
            struct memory chunk = {0};
            chunk.response = (char*)malloc(1);
            chunk.size = 0;
            
            format::to_lower(method);

            if (!strcmp(method, "post"))
            {
                curl_easy_setopt(curl, CURLOPT_POST, 1L);
            }
            else if (!strcmp(method, "options")) 
            {
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "OPTIONS");
            }
            //else default to GET

            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_PROTOCOLS_STR, "HTTP");
            curl_easy_setopt(curl, CURLOPT_PROTOCOLS_STR, "HTTPS");
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
            curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, err);
            curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, cb);
            curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void*)&chunk);

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
                return chunk.response;
            }
            else
            {
                free(chunk.response);
                return NULL;
            }
        }

        static void initialize()
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

            //example
            /*
                char err[CURL_ERROR_SIZE + 1] = {0};
                char* resp = http_request("https://www.google.com", "GET", (char*)&err);
                if (!resp) 
                {
                    console_menu::write_line(format::va("FAIL: %s\n", err));
                }
                else 
                {
                    // print just the first line
                    char* end = strchr(resp, '\r');
                    if (end) resp[end - resp] = '\0';
                    console_menu::write_line(format::va("Response: %s\n", resp));
                    free(resp);
                }
            */
        }
};

#endif _NET_H_
