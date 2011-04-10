#pragma once

#include <QPair>
#include <QString>
#include <QFile>
#include <QTextStream>
//#include "../../../qrrepo/repoApi.h" // когда в транке лежит
#include "../../../../unreal/trunk/qrrepo/repoApi.h" // когда лежит в tools

namespace Geny {
	class Gemake;

	class Interpreter {
		public:
			Interpreter(const QString& repoPath, const QString& taskFilename,
					qReal::Id curObject, Gemake* gemaker);
			~Interpreter();

			QString interpret();

		private:
			//Can move cursor position in stream!
			QString interpret(QTextStream& stream);

			//Control string starts with #!
			bool isControlString(const QString&);

			enum ControlStringType {
				commentType, foreachType,
			        forType,
				leftBraceType, rightBraceType,
				toFileType, saveObjType,
				switchType, caseType,
				notControlType
			};
			ControlStringType controlStringType(const QString&);
			
			//Can move cursor position in stream!
			QString notControlStringParse(const QString&);
			//Can move cursor position in stream!
			QString controlStringParse(const QString&, QTextStream& stream);

			//Returns a pair of the type name of searching objects
			//and in that list they are searched
			QPair<QString, QString> foreachStringParse(const QString&);

			//Returns a method name in 'for' string
			QString forStringParse(const QString& str);

			//Returns filename to write 
			QString toFileStringFilename(const QString&);

			//Returns a property name for the switch
			QString switchStringParse(const QString&);

			//Returns a case value
			QString caseStringParse(const QString&);

			QString saveObjLabel(const QString&);
			void addLabel(const QString&);

			//Control expression between @@ @@
			QString controlExpressionParse(const QString& expression);
			QString getBraceBlock(QTextStream&);

			QPair<QString, QString> getNextCaseBlock(QTextStream&);

			QString getObjProperty(const qReal::Id& objectId, const QString& propertyName);
			QString getCurObjProperty(const QString& propertyName);
			
			qReal::Id getCurObjectMethodResult(const QString& methodName);
			qReal::IdList getCurObjectMethodResultList(const QString&);

			//нужно, так как возможно использование списка Id вместо одного
			qReal::Id getCurObjId();
	
			QFile taskFile;
			QTextStream* inStream;

			qrRepo::RepoApi rApi;
			QString repoPath;

			Gemake* geMaker;

			qReal::Id curObjectId;
			QMap<QString, qReal::Id> objectsByLabels;
	};
}
