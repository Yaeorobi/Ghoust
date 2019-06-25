#include <iostream>
//#include <thread>
//#include <chrono>
//#include <vector>
#include "serialHP45.hpp" 
#include "cxxopts.hpp"


void print(std::vector <std::string> *buffer){
	int i = 0;
	while(1){
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		std::cout << i << std::endl;
		buffer->push_back("hola");
		buffer->erase(buffer->begin());

		if (i == 10){
			std::cout << "size: "<< buffer->size();
			break;
		}
		i++;
	}

}


int main(int argc, char** argv){
	/*
	std::vector<std::string> buffer;
	
	std::thread t1(print, &buffer);

	std::string a = "ola ke ace";
	std::string text = "Let;me;split;this;into;words";

	size_t pos = 0;
	std::string delimiter = ";";
	std::string token;

	std::vector<std::string> split_text;


	while((pos = text.find(delimiter)) != std::string::npos){
		token = text.substr(0, pos);
		split_text.push_back(token);
		text.erase(0, pos + delimiter.length());
	;}
	 
	for(int i = 0; i < split_text.size(); i++)
		std::cout  << split_text[i] << std::endl;
	// a += '\r';

	std::cout << a + 'd' << std::endl;

	t1.join();
	return 0;

	*/

	try{
		cxxopts::Options options("test", "Test communication");
		options
		  .positional_help("[args]")
		  .show_positional_help();
		options
		  .add_options()
		  ("p,port", "Port communication", cxxopts::value<std::string>())
		  ("b,baud", "Baudrate", cxxopts::value<int>());

		auto result = options.parse(argc, argv);

	}catch{
		std::cout <<"error parsing options: " << e.what() <<std::end;
		exit(1);
	}


	HP45 hp45 = HP45(result["port"].as<std::string>(), result["baud"].as<int>());

	hp45.openPort();

	std::cout << hp45.writePort("GTP");

	std::cout << hp45.readPort() << std::endl;
	
	std::cout << "test finished" << std::endl;

	return 0;


}
