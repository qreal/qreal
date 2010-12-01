#include "interpreter.h"
#include <QDebug>
#include "../../../qrrepo/repoApi.h"

using namespace Geny;

int main() {
	Interpreter interpreter("../../..", "../classRep_2.geny", qReal::Id());
	//Interpreter interpreter("../../..", "../classH_with_association.geny", qReal::Id());

	qDebug() << interpreter.interpret(); 
	return 0;
}
