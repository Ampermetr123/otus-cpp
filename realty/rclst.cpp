#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <map>

#include "realty.h"

struct parse_arg_exception : public std::logic_error {
  parse_arg_exception() :
    std::logic_error("Invalid arguments.\nUsage: rclst <clusters_count> <modelfname>") {};
};

/** Parse command line  */
std::tuple<int, std::string> parse_args(int argc, const char* argv[]) {
	int number;
	if (argc < 3) {
		throw parse_arg_exception();
	}
	try {
		number = std::stoi(argv[1]);
	}
	catch (const std::exception&) {
		throw parse_arg_exception();
	}
	if (number <= 0) {
		throw parse_arg_exception();
	}
	return { number, std::string(argv[2]) };
}





int main(int argc, const char* argv[]) {
  try {
    const auto& [num_clasters, file_name] = parse_args(argc, argv);

	// Reading reality objects
    std::vector<realty_object_t> data;

    std::cout << "parsing... "; std::cout.flush();
    std::string str;
    while (std::getline(std::cin, str)) {
      data.push_back(parse_reality_object(std::move(str)));
    }
    std::cout << "ok ("<<data.size()<<" objects)"<<std::endl;
    // Clustering
    std::vector<realty_object_t> initial_centers;
    std::vector<double> lables;
    lables.reserve(num_clasters);
    
    dlib::kcentroid<rbf_kernel_t> kc(rbf_kernel_t(gc_precision), 0.001, num_clasters);
    dlib::kkmeans<rbf_kernel_t> kkmeans(kc);
    kkmeans.set_number_of_centers(num_clasters);
    pick_initial_centers(num_clasters, initial_centers, data, kkmeans.get_kernel());


    std::cout << "clasterisation... "; std::cout.flush();
    find_clusters_using_kmeans(data, initial_centers);
    kkmeans.train(data, initial_centers);
    std::cout << "ok"<<std::endl;
    
    std::map<double, int> map_count;
    for (const auto& obj : data) {
      lables.push_back(kkmeans(obj));
      map_count[lables.back()] += 1;
    }
    for (auto& [k, v] : map_count) {
      std::cout << v << " ";
    }
    std::cout << std::endl;

    // Train and serialize
    ovo_trainer_t  ovo_trainer;
    dlib::krr_trainer<rbf_kernel_t> rbf_trainer;
    rbf_trainer.set_kernel(rbf_kernel_t(gc_precision));
    ovo_trainer.set_trainer(rbf_trainer);
    std::cout << "training... "; std::cout.flush();
    descision_func_t df = ovo_trainer.train(data, lables);
    std::cout << "ok" << std::endl;

    std::cout << "serialisation... ";
    dlib::serialize(file_name) << data << lables << df;
    std::cout << "ok" << std::endl;

  }
  catch (const std::exception& ex) {
    std::cout << ex.what() << std::endl;
    return 1;
  }

  return 0;
}