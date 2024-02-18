#ifndef _FORMAT_H_
#define _FORMAT_H_

#include <string>

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
};

#endif