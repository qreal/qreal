#include "interpreter.h"
#include <QDebug>
#include "../../../qrrepo/repoApi.h"

using namespace Geny;

int main(int argc, char *argv[]) {
	if (argc < 2) {
		qDebug() << "Нет параметра-файла!";
		return 0;
	}

	Interpreter interpreter("../../..", argv[1], qReal::Id());
	//Interpreter interpreter("../../..", "../classH_with_association.geny", qReal::Id());

	qDebug() << interpreter.interpret(); 
	return 0;
}
