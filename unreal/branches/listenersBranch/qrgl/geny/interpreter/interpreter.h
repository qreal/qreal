#include <QString>
#include <QFile>
#include <QTextStream>

namespace Geny {
	class Interpreter {
		public:
			Interpreter(QString taskFilename);
			~Interpreter();

			QString interpret();

		private:
			//Является ли строка управляющей в языке Geny
			//т.е. начинается ли на #!
			bool isControlString(QString);
	
			QFile taskFile;
			QTextStream* inStream;
	};
}
