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
	bool isResolved() const;
	QString name() const;
	QString qualifiedName() const;
	void setName(QString const &name);
	void setContext(QString const &newContext);
	QMap<QString, Property*> properties() const;
	virtual void generateCode(utils::OutFile &out) = 0;
	virtual void generateNameMapping(utils::OutFile &out) = 0;
	virtual bool generateObjectRequestString(utils::OutFile &out, bool notIsFirst) = 0;
	virtual bool generateProperties(utils::OutFile &out, bool notIsFirst) = 0;
	void setDiagram(Diagram *diagram);

protected:
	void copyFields(Type *type) const;
	QString nativeContext() const;

	QMap<QString, Property*> mProperties;
	bool mResolvingFinished;
	Diagram *mDiagram;

private:
	QString mName;  // Неквалифицированное имя метатипа
	QString mContext;  // Контекст квалификации. Например, для Kernel::Node: Node - имя, Kernel - контекст.
	QString mNativeContext;  // "Родной" контекст квалификации, не меняется при импорте типа и используется для ресолва.
};
