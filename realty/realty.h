#pragma once

#include <dlib/matrix.h>
#include <dlib/clustering.h>
#include <dlib/svm_threaded.h>

#include <sstream>
#include <type_traits>
#include <numeric>
#include <iostream>

// долгота;широта;комнат;цена;метраж;кухня;этаж;этажность\n
//86.116781;55.335492;2;4326901.00;54.00;7.00;5;5\n
//const size_t gc_realty_object_string_min_len = 42;

const double gc_precision = 0.00000001;
using realty_object_t = dlib::matrix<double, 7, 1>;
using rbf_kernel_t = dlib::radial_basis_kernel<realty_object_t>;
using ovo_trainer_t = dlib::one_vs_one_trainer< dlib::any_trainer<realty_object_t> >;
using descision_func_t = dlib::one_vs_one_decision_function<ovo_trainer_t, dlib::decision_function<rbf_kernel_t>>;


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


inline realty_object_t parse_reality_object(std::string str) {
	realty_object_t t;
	std::stringstream ss(std::move(str));
	for (int i = 0; i < 7; i++) {
		t(i) = extract_val<double>(ss);
	}
	int floor = extract_val<int>(ss);
	int floors_total = extract_val<int>(ss);
	t(6) = static_cast<double>((floor == floors_total || floor == 1) ? 0 : 1);
	return t;
}


inline std::ostream& operator << (std::ostream& os, const realty_object_t& obj) {
	
  	for (int i = 0; i < obj.size(); i++){
		os << std::setprecision(10) << obj(i) << ";";
	}
	return os;
}