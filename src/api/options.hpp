#ifndef OPTIONS
#define OPTIONS

#include <string>
#include <vector>

class options
{
    public:
        std::vector<options> child_options;
        int id;
        int quantity;
        std::vector<int> sub_option_ids;

        options(const std::vector<options>& child_options, int id, int quantity, const std::vector<int>& sub_option_ids) :
            child_options(child_options),
            id(id),
            quantity(quantity),
            sub_option_ids(sub_option_ids)
        {}
};

#endif /* OPTIONS */
