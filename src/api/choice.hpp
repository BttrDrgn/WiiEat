#ifndef SELECTION
#define SELECTION

#include <string>
#include <vector>

class option
{
    public:
        std::string name;
        std::string id;

        option(const std::string& name, const std::string& id)
        {
            this->name = name;
            this->id = id;
        }
};

class choice
{
    public:
        std::string name;
        std::string id;
        std::vector<option> options;
        bool required;
        int max_options;

        choice(const std::string& name, const std::string& id, int max_options, bool required = false)
        {
            this->name = name;
            this->id = id;
            this->max_options = max_options;
            this->required = required;
        }

        void add_option(const std::string& name, const std::string& id)
        {
            //options.emplace_back(new option(name, id));
        }
};

#endif /* CHOICE */
