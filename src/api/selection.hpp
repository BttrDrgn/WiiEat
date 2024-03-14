#ifndef SELECTION
#define SELECTION

#include <string>

class selection
{
    public:
        std::string name;
        std::string id;

        selection(const std::string& name, const std::string& id)
        {
            this->name = name;
            this->id = id;
        }
};

#endif /* SELECTION */
