#include <iostream>
#include "nxtOSEKRobotGenerator.h"

using namespace std;

int main() {
	qReal::generators::NxtOSEKRobotGenerator gen("/home/me/programming/gitRepos/qreal");
	gen.generate();
	return 0;
}
