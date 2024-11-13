#ifndef _FORMAT_H_
#define _FORMAT_H_

#include <string>
#include <vector>
#include <cstdarg>
#include <cstring>
#include <sstream>
#include <iomanip>

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

        static std::string to_upper(const std::string& input)
        {
            std::string result;
            result.reserve(input.size());  // Reserve space to optimize

            for (char c : input) {
                result += std::toupper(static_cast<unsigned char>(c));
            }

            return result;
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

        static std::string remove_non_ascii(const std::string& input)
        {
            std::string output = input;
            for (size_t i = 0; i < output.length(); ++i)
            {
                if (static_cast<unsigned char>(output[i]) > 127)
                {
                    output.erase(i, 1);
                    --i; // Adjust index after erasing
                }
            }
            return output;
        }

        static std::vector<std::string> split(const std::string& s, const std::string& seperator)
        {
            std::vector<std::string> output;

            std::string::size_type prev_pos = 0, pos = 0;

            while ((pos = s.find(seperator, pos)) != std::string::npos)
            {
                std::string substring(s.substr(prev_pos, pos - prev_pos));

                output.push_back(substring);

                prev_pos = ++pos;
            }

            output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word

            return output;
        }

        //https://stackoverflow.com/a/39813237
        static bool check_is_double(const std::string& in_str, double &result)
        {
            char* end;
            result = strtod(in_str.c_str(), &end);
            if (end == in_str.c_str() || *end != '\0') return false;
            return true;
        }

        static std::string cents_to_usd(int cents)
        {
            double dollars = static_cast<double>(cents) / 100.0;

            std::stringstream ss;
            ss << "$" << std::fixed << std::setprecision(2) << dollars;

            return ss.str();
        }
};

#endif