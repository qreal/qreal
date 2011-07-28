#include <iostream>
#include "nxtOSEKRobotGenerator.h"

using namespace std;

int main() {
	qReal::generators::NxtOSEKRobotGenerator gen("/home/me/tmp/delme/git/models/Robots-examples/multipleObstacleTouches");
	gen.generate();
	return 0;
}
