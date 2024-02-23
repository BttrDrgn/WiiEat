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

        static std::string replace(const char* original, const char* find, const char* replace)
        {
            std::string original_str(original);
            std::string find_str(find);

            // Find the first occurrence of the substring to replace
            size_t pos = original_str.find(find_str);

            // While there are occurrences of the substring to replace
            while (pos != std::string::npos)
            {
                // Replace the substring with the replacement string
                original_str.replace(pos, find_str.length(), replace);
                
                // Find the next occurrence of the substring to replace
                pos = original_str.find(find_str, pos + strlen(replace));
            }

            // Return the modified string
            return original_str;
        }
};

#endif