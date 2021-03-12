#include <iostream>
#include <string>
#include <filesystem>
#include <type_traits>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <tuple>

#include "realty.h"


namespace fs = std::filesystem;

int main(int argc, const char* argv[]) {
    try {
        if (argc < 2) {
            throw std::logic_error("Invalid arguments.\nUsage: rclss <modelfname>");
        }
        std::string file_name(argv[1]);

        if (!fs::exists(file_name) || !fs::is_regular_file(file_name)) {
            throw std::logic_error(file_name + " file not found");
        };

        std::vector<double> lables;
        std::vector<realty_object_t> data;
        descision_func_t df;
        dlib::deserialize(file_name) >> data >> lables >> df;

        using realty_object_map_t = std::map<double, std::vector<realty_object_t>>;
        realty_object_map_t obj_map;

        for (std::size_t i = 0; i < lables.size(); i++) {
            obj_map[lables.at(i)].emplace_back(data.at(i));
        }

        //std::cout << "Total " << obj_map.size() << " clasters" << std::endl;

        std::string str;
        while (std::getline(std::cin, str))
        {
            realty_object_t target = parse_reality_object(str);
            double target_lable = df(target);
            auto& obj_vect = obj_map[target_lable];

            std::sort(obj_vect.begin(), obj_vect.end(),
                [t = target](const realty_object_t& lhs, const realty_object_t& rhs) {
                    return std::hypot(t(0) - lhs(0), t(1) - lhs(1)) < std::hypot(t(0) - rhs(0), t(1) - rhs(1));
                }
            );

           // std::cout << "\nresult cluster for " << target << " (total " << obj_vect.size() << ") objects:" << std::endl;
            for (const auto& obj : obj_vect) {
                std::cout << obj << std::endl;
            }

        }
    }
    catch (const std::exception& ex) {
        std::cout << ex.what() << std::endl;
        return 1;
    }

    return 0;

}