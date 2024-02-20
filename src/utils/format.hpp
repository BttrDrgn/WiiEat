#ifndef _FORMAT_H_
#define _FORMAT_H_

#include <string>
#include <cstdarg>
#include <cstring>

class format
{
    public:
        static std::string va(const char* fmt, ...)
        {
            va_list va;
            va_start(va, fmt);
            char result[512]{};
            std::vsprintf(result, fmt, va);
            return std::string(result);
        }

        static void to_lower(char *str)
        {
            while (*str)
            {
                *str = tolower((unsigned char)*str);
                str++;
            }
        }

        static char* get_n_chars(const char* str, int n)
        {
            char* result = (char*)malloc((n + 1) * sizeof(char));
            if (result == NULL)
            {
                return NULL;
            }
            
            strncpy(result, str, n);
            
            result[n] = '\0';
            
            return result;
        }
};

#endif