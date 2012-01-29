#include <iostream>
#include "emboxRobotGenerator.h"

using namespace std;

int main() {
	qReal::generators::EmboxRobotGenerator gen("/home/me/programming/gitRepos/qreal");
	gen.generate();
	return 0;
}
