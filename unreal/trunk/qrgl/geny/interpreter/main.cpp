#include "gemake.h"
#include <QDebug>
#include "../../../qrrepo/repoApi.h"

using namespace Geny;

int main(int argc, char *argv[]) {
	if (argc < 2) {
		qDebug() << "Нет параметра-файла!";
		return 0;
	}

	Gemake geMake(argv[1]);
	geMake.make();

	//qDebug() << interpreter.interpret(); 
	return 0;
}
