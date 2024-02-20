#ifndef _FS_H_
#define _FS_H_

#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <utils/format.hpp>
#define MAXPATH 256

class fs
{
    public:
        static void init();

        static bool create_directory(const std::string& directory)
        {
            return !mkdir(directory.c_str(), 0777);
        }

        static bool file_exists(const std::string& file)
        {
            return std::ifstream(file).good();
        }

        static std::string read_file(const std::string& file)
        {
            std::string data;
            read_file(file, &data);
            return data;
        }

        static bool read_file(const std::string& file, std::string* data)
        {
            if (!data) return false;
            data->clear();

            if (file_exists(file))
            {
                std::ifstream stream(file, std::ios::binary);
                if (!stream.is_open()) return false;

                stream.seekg(0, std::ios::end);
                const std::streamsize size = stream.tellg();
                stream.seekg(0, std::ios::beg);

                if (size > -1)
                {
                    data->resize(static_cast<unsigned int>(size));
                    stream.read(data->data(), size);
                    stream.close();
                    return true;
                }
            }

            return false;
        }

        static bool write_file(const std::string& file, const std::string& data, const bool append = false)
        {
            const auto pos = file.find_last_of("/\\");
            if (pos != std::string::npos)
            {
                create_directory(file.substr(0, pos));
            }

            std::ofstream stream(file, std::ios::binary | std::ofstream::out | (append ? std::ofstream::app : (std::ios_base::openmode)0));

            if (stream.is_open())
            {
                stream.write(data.data(), static_cast<std::streamsize>(data.size()));
                stream.close();
                return true;
            }

            return false;
        }
};

#endif