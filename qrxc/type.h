#pragma once

#include <QDomElement>
#include <QMap>

class Property;
class Diagram;

namespace utils {
	class OutFile;
}

class Type
{
public:
	Type(bool isResolved, Diagram *diagram);
	virtual ~Type();
	virtual Type* clone() const = 0;
	virtual bool init(QDomElement const &element, QString const &context);
	virtual bool resolve() = 0;
	virtual bool isResolving() const;
	bool isResolved() const;

	QString name() const;
	QString path() const;
	QString qualifiedName() const;
	QString displayedName() const;

	QMap<QString, Property*> properties() const;

	void setName(QString const &name);
	void setDiagram(Diagram *diagram);
	void setContext(QString const &newContext);
	void setDisplayedName(QString const &displayedName);
	virtual void generateCode(utils::OutFile &out) = 0;
	virtual void generateNameMapping(utils::OutFile &out) = 0;
	virtual bool generateObjectRequestString(utils::OutFile &out, bool isNotFirst) = 0;
	virtual bool generateProperties(utils::OutFile &out, bool isNotFirst, bool isReference) = 0;
	virtual bool generateContainedTypes(utils::OutFile &out, bool isNotFirst) = 0;
	virtual bool generateConnections(utils::OutFile &out, bool isNotFirst) = 0;
	virtual bool generateUsages(utils::OutFile &out, bool isNotFirst) = 0;
	virtual bool generatePossibleEdges(utils::OutFile &out, bool isNotFirst) = 0;
	virtual bool generateEnumValues(utils::OutFile &out, bool isNotFirst) = 0;
	virtual void generatePropertyTypes(utils::OutFile &out) = 0;
	virtual void generatePropertyDefaults(utils::OutFile &out) = 0;
	virtual void generatePropertyDescriptionMapping(utils::OutFile &out) = 0;
	virtual void generateMouseGesturesMap(utils::OutFile &out) = 0;

protected:
	void copyFields(Type *type) const;
	QString nativeContext() const;

	QMap<QString, Property*> mProperties;
	bool mResolvingFinished;
	Diagram *mDiagram;

private:
	QString mName;  // Неквалифицированное имя метатипа
	QString mContext;  // Контекст квалификации. Например, для Kernel::Node: Node - имя, Kernel - контекст.
	QString mNativeContext;  // "Pодной" контекст квалификации, не меняется при импорте типа и используется для ресолва.
	QString mDisplayedName;
	QString mPath;
};
