#ifndef FS
#define FS

#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <sys/time.h>
#include <chrono>
#include <format/format.hpp>
#include <stdio.h>
#define MAXPATH 256

class io
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

        static bool read_file(const std::string& file, uint8_t*& data, size_t& size)
        {
            data = nullptr;
            size = 0;

            std::ifstream stream(file, std::ios::binary | std::ios::ate);
            if (!stream.is_open()) return false;

            // Get the file size
            size = stream.tellg();
            stream.seekg(0, std::ios::beg);

            if (size > 0)
            {
                // Allocate memory for the buffer
                data = new uint8_t[size];
                // Read the file into the buffer
                stream.read(reinterpret_cast<char*>(data), size);
                stream.close();
                return true;
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

        static bool write_file(const std::string& file, const uint8_t* buffer, size_t size, const bool append = false)
        {
            const auto pos = file.find_last_of("/\\");
            if (pos != std::string::npos) {
                // Create directory if it doesn't exist
                create_directory(file.substr(0, pos));
            }

            std::ofstream stream(file, std::ios::binary | std::ofstream::out | (append ? std::ofstream::app : (std::ios_base::openmode)0));

            if (stream.is_open()) {
                // Write buffer to file
                stream.write(reinterpret_cast<const char*>(buffer), size);
                stream.close();
                return true;
            }

            return false;
        }

        static bool delete_file(const std::string& path)
        {
            return !std::remove(path.c_str());
        }

        static unsigned long long time_now()
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        }
};

#endif /* FS */
