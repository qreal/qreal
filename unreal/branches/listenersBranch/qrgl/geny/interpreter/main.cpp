#include "interpreter.h"
#include <QDebug>

using namespace Geny;

int main() {
	Interpreter interpreter("../../..", "../classRep_2.geny", qReal::Id());
	qDebug() << interpreter.interpret(); 
	return 0;
}
