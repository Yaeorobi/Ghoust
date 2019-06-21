#include <iostream>
#include "cxxopts.hpp"
#include <sys/stat.h>


cxxopts::ParseResult
parse(int argc, char *argv[]){
	try{
	       	cxxopts::Options options("Ghoust", "Controller for Oasis system printer");

		options
		  .positional_help("[args]")
		  .show_positional_help();

		options
		  .add_options()
	  	  ("i,input", "Input PDF file", cxxopts::value<std::string>())
		  ("t,type", "Type file [pdf | bitmap]", cxxopts::value<std::string>())
		;

		auto result = options.parse(argc, argv);
		return result;

	}catch(const cxxopts::OptionException& e){
		 std::cout << "error parsing options: " << e.what() << std::endl;
		 exit(1);
	}
}

inline bool exist_file(const std::string& name){
	struct stat buffer;
	return(stat (name.c_str(), &buffer) == 0);
}

int
main(int argc, char* argv[]){
	auto result  = parse(argc, argv);
	auto args = result.arguments();
	std::string filename;
	bool pdf = true;

	for (int i=0; i < args.size(); i++){
		if (args[i].key() == "input"){
			if (!exist_file(args[i].value())){
				std::cout << "The file " << args[i].value() << " does not exist" << std::endl;
				exit(1);
			}
			filename = args[i].value();
		}
		if (args[i].key() == "type"){
			if (args[i].value() == "bitmap"){
				pdf = false;
			}
		}
	}

	return 0;
}
