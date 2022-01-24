#include <iostream>
#include <boost/share_ptr.hpp>
#include <boost/filesystem.hpp>

int main(int argc, char **argv) {
	std::cout << "Hello third-party-lib!" << std::endl;

	boost::share_ptr<int> isp(new int(4));
	boost::filesystem::path path = "/usr/share/cmake/modules"

	if (path.is_relative()) {
		std::cout << "path is relative!" << std::endl;
	} else {
		std::cout << "path is not relative!" << std::endl;
	}
	return 0;
}
