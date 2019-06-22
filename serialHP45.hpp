/*
MIT License

Copyright (c) 2019 Emmanuel Arias <eamanu@eamanu.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE

*/

#ifndef SERIALHP45_HPP_INCLUDE
#define SERIALHP45_HPP_INCLUDE

#include <termios.h>
#include <iostream>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <vector>


class HP45{
	public:
		HP45(std::string portname, int baudrate);
		~HP45();

		int openPort();
		int writePort(std::string message);}
		std::string readPort();

		int closePort();

		void update();
		void BufferNext();
		void SerialWriteBufferRaw(std::string input_string);

	private:
		int fd;
		std::string port;
		int baudrate;
		int start_event = 0;
		int ok = 0;
		int connected = 0;
		std::vector<std::string> code_buffer;
		int status_state = 0;
		int send_get_status = 0;
		std::string send_status_buffer = "";
		int inkjet_writeleft;


		void GetStatus();
		std::vector<std::string> split(std::string in, std::string delimiter);

};

std::vector<std::string>
split(std::string in, std::string delimiter){
	size_t pos = 0;
	std::string token;
	std::vector<std::string> split_text;

	while((pos = in.find(delimiter)) != std::string::npos ){
		token = in.substr(0, pos);
		split_text.push_back(token);
		in.erase(0, pos + delimiter.lengt())
	}
	return split_text;
}


void
HP45::GetStatus(){
	std::this_thread::sleep_for(std::chrono::seconds(5));
	while (start_event){
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); //  wait for 0.1 seconds
		if (status_state == 0)
			send_status_buffer = "GTP";
		if (status_state == 1)
			send_status_buffer = "GEP";
		if (status_state == 2)
			send_status_buffer = "BWL";

		send_get_status = 1;
		status_stat ++;
		if (status_state > 2)
			status_state = 0;
	}
}


HP45::HP45(std::string portname, int baud){
	fd = 0;
	port = portname;
	baudrate = baud;

}

HP45::~HP45(){
	/*Destroy*/
}

int HP45::openPort(){
	struct termios config;
	if (connected == 1){ // I am read connected
		return 0
	}
	fd = open(port.c_str(), O_RDWR, O_NOCTTY, O_NDELAY);
	if (fd < 0){
		std::cerr << "Error on open the serial port "<< std::endl;
		return -1;
	}

	if (!isatty(fd)){
		std::cerr << "This is not a TTY device"<< std::endl;
		return -1;
	}
	if (tcgetattr(fd, &config) < 0){
		std::cerr << "Error on getting the current configuration for serial interface"<< std::endl;
		return -1;
	}

	config.c_iflag &= ~(IGNBRK | BRKINT | ICRNL |
                            INLCR | PARMRK | INPCK | ISTRIP | IXON);
	config.c_oflag = 0;

	config.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);

	config.c_cflag &= ~(CSIZE | PARENB);
	config.c_cflag |= CS8;

	config.c_cc[VMIN]  = 1;
	config.c_cc[VTIME] = 0;

	cfsetospeed (&config, (speed_t) baudrate);
	cfsetospeed (&config, (speed_t) baudrate);

	cfmakeraw(&config);

	tcflush(fd, TCIFLUSH);

	if (tcsetattr(fd, TCSANOW, &config) < 0){
		std::cerr << "Error on set attribute " << std::endl;
		return -1;
	}

	std::cout << "Port openned" << std::endl;
	connected = 1;
	return 1;
}

int HP45::writePort(std::string message){
	if (write(fd, &message, std::strlen(message.c_str())) < 0){
		std::cerr << "Error on the write message" << std::endl;
		return -1;
	}
	std::cout << "Message sended" << std::endl;
	return 1;
}

std::string
readPort(){
	std::string s;
	if (read(fd, %s, 64) > 0){
		return s;
	}
	return "";
}


int
HP45::closePort(){
	close(fd);
	return 1;
}

void
HP45::update(){
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	std::string buff = "";
	std::string str_OK = "OK";
	std::string str_BWL = "BWL";

	std::string temp_decode;
	std::string read_buffer;
	std::string read_line;



	while(start_event){
		ok = 0;
		buff = readPort();
		/*Receive a OK word*/
		if (!strncmp(buff.c_str(), str_OK.c_str(), str_OK.size())){
			ok = 1;
		}

		if (ok == 1){
			/*Ready for print*/
			if (!strncmp(buff.c_str(), stR_BWL.c_str(), str_BWL.size())){

			}

		}


	}
}

void
HP45::BufferNext(){
	/*Print next element of the buffer*/
	if (code_buffer.size() > 0){
		writePort(code_buffer.front()); // print HP45
		code_buffer.erase(code_buffer.begin());
	}
}

void
HP45::SerialWriteBufferRaw(std::string input_string){
	if (connected){
		code_buffer.push_back(input_string + '\r');
	}
}

#endif // SERIALHP45_HPP_INCLUDE
