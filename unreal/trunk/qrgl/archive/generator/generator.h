#include <QString>
#include <QList>

class Generator {
	public:
		bool compile(const QString& templateHFilename, const QString& templateCPPFilename, 
				const QString& templateMethodHFilename, const QString& templateMethodCPPFilename,
				const QString& classContentFilename,
				const QString& resultHFilename, const QString& resultCPPFilename);

	private:
		bool loadTemplateFromFile(const QString& filename, QString& loadedTemplate);
		
		class Method {
			public:
				Method(const QString& resultType, const QString& name): resultType(resultType), name(name) {}
				QString resultType;
				QString name;
		};

		class ClassContent {
			public:
				QString className;
				QList<Method> methodsList;
		};

		bool loadClassContent(const QString& filename, ClassContent& classContent);

		bool insertStrToTemplate(const QString& newStr, const QString& placeIndentificator, QString& templateString);
		bool insertMethodToTemplate(const Method&  method, const QString& methodTemplate, QString& templateString);
};
