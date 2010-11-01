#include "generator.h"

int main() {
	Generator gen;
	gen.compile("template.h", "template.cpp",
			"templateMethod.h", "templateMethod.cpp",
			"classContent", "myClass.h", "myClass.cpp");
	return 0;
}
