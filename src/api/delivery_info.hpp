#ifndef DELIVERY_INFO
#define DELIVERY_INFO

#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <format/format.hpp>

class delivery_info
{
    public:
        std::vector<int> area_codes =    
        {
            205, 251, 256, 334, 938,       // Alabama
            907,                           // Alaska
            480, 520, 602, 623, 928,       // Arizona
            479, 501, 870,                 // Arkansas
            209, 213, 310, 323, 408, 415, 424, 442, 510, 530, 559, 562, 619, 626, 650, 657, 661, 707, 714, 760, 805, 818, 831, 858, 909, 916, 925, 949, 951, // California
            303, 719, 970,                 // Colorado
            203, 475, 860, 959,            // Connecticut
            202,                           // Washington, D.C.
            302,                           // Delaware
            239, 305, 321, 352, 386, 407, 561, 727, 754, 772, 786, 813, 850, 863, 904, 941, 954, // Florida
            229, 404, 470, 478, 678, 706, 762, 770, 912, // Georgia
            808,                           // Hawaii
            208, 986,                      // Idaho
            217, 224, 309, 312, 331, 447, 464, 618, 630, 708, 773, 779, 815, 847, 872, // Illinois
            219, 260, 317, 463, 574, 765, 812, 930, // Indiana
            319, 515, 563, 641, 712,       // Iowa
            316, 620, 785, 913,            // Kansas
            270, 364, 502, 606, 859, 270, 502, // Kentucky
            225, 318, 337, 504, 985,       // Louisiana
            207,                           // Maine
            240, 301, 410, 443, 667,       // Maryland
            339, 351, 413, 508, 617, 774, 781, 857, 978, // Massachusetts
            231, 248, 269, 313, 517, 586, 616, 734, 810, 906, 947, 989, // Michigan
            218, 320, 507, 612, 651, 763, 952, // Minnesota
            228, 601, 662, 769,            // Mississippi
            314, 417, 573, 636, 660, 816, // Missouri
            406,                           // Montana
            308, 402, 531,                 // Nebraska
            702, 725, 775,                 // Nevada
            603,                           // New Hampshire
            201, 551, 609, 732, 848, 862, 908, 973, // New Jersey
            505, 575,                      // New Mexico
            212, 315, 332, 347, 516, 518, 585, 607, 631, 646, 716, 718, 845, 914, 917, 929, // New York
            252, 336, 704, 743, 828, 910, 919, 980, 984, // North Carolina
            701,                           // North Dakota
            216, 234, 283, 330, 380, 419, 440, 513, 567, 614, 740, 937, // Ohio
            405, 539, 580, 918,            // Oklahoma
            458, 503, 541, 971,            // Oregon
            215, 223, 267, 272, 412, 484, 570, 610, 717, 724, 814, 878, // Pennsylvania
            401,                           // Rhode Island
            803, 839, 843, 854, 864,       // South Carolina
            605,                           // South Dakota
            423, 615, 629, 731, 865, 901, 931, // Tennessee
            210, 214, 254, 281, 325, 346, 361, 409, 430, 432, 469, 512, 682, 713, 737, 806, 817, 830, 832, 903, 915, 936, 940, 956, 972, 979, // Texas
            385, 435, 801,                 // Utah
            802,                           // Vermont
            276, 434, 540, 571, 703, 757, 804, 821, // Virginia
            206, 253, 360, 425, 509, 564, // Washington
            304, 681,                      // West Virginia
            262, 274, 414, 534, 608, 715, 920, // Wisconsin
            307                            // Wyoming
        };

        //This will generate a phone number that grubhub will accept for privacy reasons
        std::string generate_ph()
        {
            int three_digits = 100 + rand() % 900;
            int four_digits = 1000 + rand() % 9000;

            return format::va("(%i) %i-%i", area_codes[rand() % area_codes.size()], three_digits, four_digits);
        }

        std::string address_country;
        std::string address_locality;
        std::string address_region;
        std::string email;
        bool green_indicated;
        std::string latitude;
        std::string longitude;
        std::string name;
        std::string phone;
        std::string postal_code;
        std::string street_address1;

        // Constructor with default values
        delivery_info(const std::string& address_locality, const std::string& address_region,
        const std::string& email, double latitude, double longitude, const std::string& postal_code, const std::string& address) :
            address_country("US"),
            address_locality(address_locality),
            address_region(address_region),
            email(email),
            green_indicated(false),
            latitude(std::to_string(latitude)),
            longitude(std::to_string(longitude)),
            name("Wii Eat"),
            phone(generate_ph()),
            postal_code(postal_code),
            street_address1(address)
        {}

        delivery_info(const std::string& address_locality, const std::string& address_region,
        const std::string& email, const std::string& latitude, const std::string& longitude, const std::string& postal_code, const std::string& address) :
            address_country("US"),
            address_locality(address_locality),
            address_region(address_region),
            email(email),
            green_indicated(false),
            latitude(latitude),
            longitude(longitude),
            name("Wii Eat"),
            phone(generate_ph()),
            postal_code(postal_code),
            street_address1(address)
        {}

        // Serialize method
        json serialize() const
        {
            return {
                {"address_country", address_country},
                {"address_locality", address_locality},
                {"address_region", address_region},
                {"email", email},
                {"green_indicated", green_indicated},
                {"latitude", latitude},
                {"longitude", longitude},
                {"name", name},
                {"phone", phone},
                {"postal_code", postal_code},
                {"street_address1", street_address1},
            };
        }
};

#endif /* DELIVERY_INFO */
