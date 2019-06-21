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

class HP45{
	public:
		HP45(std::string portname, int baudrate);
		~HP45();

		int openPort();
		int writePort(std::string message);}
		std::string readPort();

		int closePort();

		void update();

	private:
		int fd;
		std::string port;
		int baudrate;
		int start_event = 0;

};



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
	if (read(fd, %s, 255) > 0){
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
	std::this_thread::sleep_for(0.05);
	std::string buff = "";

	while (start_event){
		buff = readPort();

	} 

}

#endif // SERIALHP45_HPP_INCLUDE
