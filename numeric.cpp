#include <algorithm>
#include <iostream>
#include <string>

//C++98 version ----------------------------
struct numerical_appender {
	numerical_appender(std::string &buf) : _buf(buf) { }
	void operator()(const char c) {
		if( c >= '0' && c <= '9' ) _buf.push_back(c);
	}
private:
	std::string &_buf;
};

void get_numeric(const std::string &input, std::string &output) 
{
	numerical_appender appender(output);
	std::for_each(input.begin(), input.end(), appender);
}

//C++14 version ----------------------------
std::string get_numeric(const std::string &input) 
{
	std::string output;
	std::for_each(begin(input), end(input), [&output](const char c) {
		if( c >= '0' && c <= '9' ) output.push_back(c);
	});
	return output;
}

int main() {
	
	//C++98 version
	std::string value = "12asew3d45ddf678ee9 0";
	std::string num;
	get_numeric(value, num);
	std::cout << value << " - " << num << std::endl;

	//C++14 version
	std::cout << value << " - " << get_numeric(value) << std::endl;
	
}