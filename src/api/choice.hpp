#ifndef SELECTION
#define SELECTION

#include <string>
#include <vector>

class option
{
    public:
        std::string name;
        std::string id;
        double price;

        option(const std::string& name, const std::string& id, double price)
        {
            this->name = name;
            this->id = id;
            this->price = price;
        }
};

class choice
{
    public:
        std::string name;
        std::string id;
        std::vector<option*> options;
        bool required;
        int max_options;
        int min_options;

        choice(const std::string& name, const std::string& id, int max_options, int min_options)
        {
            this->name = name;
            this->id = id;
            this->max_options = max_options;
            this->min_options = min_options;
        }

        void add_option(const std::string& name, const std::string& id, double price)
        {
            options.emplace_back(new option(name, id, price));
        }
};

#endif /* CHOICE */
