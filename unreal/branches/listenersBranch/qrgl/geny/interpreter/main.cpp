#include "interpreter.h"
#include <QDebug>
#include "../../../qrrepo/repoApi.h"

using namespace Geny;

int main() {
	Interpreter interpreter("../../..", "../classRep_2.geny", qReal::Id());

	//qrRepo::RepoApi rApi("../../..");
	//Interpreter interpreter("../../..", "../classRep_2.geny", rApi.elementsByType("Class").at(0));

	//qrRepo::RepoApi rApi("../../..");
	//Interpreter interpreter("../../..", "../fieldRep.geny", rApi.elementsByType("Field").at(0));

	qDebug() << interpreter.interpret(); 
	return 0;
}
