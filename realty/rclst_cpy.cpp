#include <iostream>
#include <string>
#include <type_traits>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <tuple>

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


template <typename T,  typename SFINAE = typename std::enable_if_t<std::numeric_limits<T>::is_integer || std::is_floating_point_v<T>, void> >
T extract_val(std::stringstream& inp_str) {
	std::string item;
	std::getline(inp_str, item, ';');
  	if (item.size() == 0) {
    	return T();
  	}
  	if constexpr (std::is_floating_point_v<T>) {
		return std::stod(item);
	}
  	else {
    	return static_cast<T>(std::stoll(item));
  	}
}


realty_object_t parse_reality_object(std::string str) {
	realty_object_t t;
	std::stringstream ss(std::move(str));
	for (int i = 0; i < 5; i++) {
		t(i) = extract_val<double>(ss);
	}
	int floor = extract_val<int>(ss);
	int floors_total = extract_val<int>(ss);
	t(5) = static_cast<double>((floor == floors_total || floor == 1) ? 0 : 1);
	return t;
}


int main(int argc, const char* argv[]) {
  try {
    const auto& [num_clasters, file_name] = parse_args(argc, argv);

	// Reading reality objects
    std::vector<realty_object_t> data;
  
    std::string str;
    while (std::getline(std::cin, str)) {
      data.push_back(parse_reality_object(std::move(str)));
    }

    // Clustering
    std::vector<realty_object_t> initial_centers;
    std::vector<double> lables;
    lables.reserve(num_clasters);
    
    dlib::kcentroid<rbf_kernel_t> kc(rbf_kernel_t(0.0001), 0.001, num_clasters);
    dlib::kkmeans<rbf_kernel_t> kkmeans(kc);
    kkmeans.set_number_of_centers(num_clasters);
    pick_initial_centers(num_clasters, initial_centers, data, kkmeans.get_kernel());
    find_clusters_using_kmeans(data, initial_centers);
    kkmeans.train(data, initial_centers);
    for (const auto& obj : data) {
      lables.push_back(kkmeans(obj));
    }

    // Train and serialize
 
    ovo_trainer_t  ovo_trainer;
    dlib::krr_trainer<rbf_kernel_t> rbf_trainer;
    rbf_trainer.set_kernel(rbf_kernel_t(0.0001));
    ovo_trainer.set_trainer(rbf_trainer);
    //dlib::one_vs_one_decision_function< ovo_trainer_t, dlib::decision_function<rbf_kernel_t> > df = ovo_trainer.train(data, lables);
    //dlib::serialize(file_name) << data << lables << df;

  }
  catch (const std::exception& ex) {
    std::cout << ex.what() << std::endl;
    return 1;
  }

  return 0;
}