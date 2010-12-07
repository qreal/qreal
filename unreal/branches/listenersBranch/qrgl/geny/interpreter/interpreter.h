#include <QPair>
#include <QString>
#include <QFile>
#include <QTextStream>

#include "../../../qrrepo/repoApi.h"

namespace Geny {
	class Interpreter {
		public:
			//Interpreter(const QString& taskFilename);
			Interpreter(const QString& repoDirectory, const QString& taskFileName, qReal::Id curObject);
			~Interpreter();

			QString interpret();

		private:
			//Can move cursor position in stream!
			QString interpret(QTextStream& stream);

			//Control string starts with #!
			bool isControlString(const QString&);

			enum ControlStringType {
				commentType, foreachType, 
				leftBraceType, rightBraceType,
				notControlType
			};
			ControlStringType controlStringType(const QString&);
			
			//Can move cursor position in stream!
			QString nonControlStringParse(const QString&, QTextStream& stream);
			//Can move cursor position in stream!
			QString controlStringParse(const QString&, QTextStream& stream);

			QPair<QString, QString> foreachStringParse(const QString&);

			//Control expression between @@ @@
			QString controlExpressionParse(const QString& expression);

			QString getCurrentObjectProperty(const QString& propertyName);

			qReal::IdList getCurObjectMethodResultList(const QString&);

			//нужно, так как возможно использование списка Id вместо одного
			qReal::Id getCurObjectId();
	
			QFile taskFile;
			QTextStream* inStream;

			qrRepo::RepoApi rApi;
			qReal::Id curObjectId;
	};
}
