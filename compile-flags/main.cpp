#include <iostream>

int main(int argc, char **argv) {
	std::cout << "Hello Compile Flags!" << std::endl;

#ifdef EX2
	std::cout << "Hello Compile Flags EX2!" << std::endl;
#endif

#ifdef EX3
	std::cout << "Hello Compile Flags EX3!" << std::endl;
#endif

	return 0;
}
