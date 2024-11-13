#ifndef CREDIT_CARD
#define CREDIT_CARD

#include <string>

class credit_card
{
    public:
        std::string id;
        std::string diner_id;
        std::string type;
        std::string last_4;

        credit_card(const std::string& id, const std::string& diner_id, const std::string& type, const std::string& last_4)
        {
            this->id = id;
            this->type = type;
            this->last_4 = last_4;
        }
};

#endif /* CREDIT_CARD */
