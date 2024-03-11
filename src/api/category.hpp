#ifndef CATEGORY
#define CATEGORY

#include <string>

class category
{
    public:
        std::string name;
        std::string id;

        category(const std::string& name, const std::string& id)
        {
            this->name = name;
            this->id = id;
        }
};

#endif /* CATEGORY */
