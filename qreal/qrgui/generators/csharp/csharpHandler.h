#pragma once

#include <QtCore/QString>
#include <QSet>

#include "../../kernel/ids.h"

namespace qReal {

	namespace client {
		class RepoApi;
	}

	namespace generators {

            struct propertyClass{
                QString visibility;
                bool isAbstract;
                bool isLeaf;
            };

            class Variable{
                public:
                Id id;
                    QString type;
                    QString visibility;
                    QString name;
                    QString defaultVal;
                    bool isStatic;
                    bool isFinal;
                    Variable();
                    QString Print(void);


            };

            class Method{
                public:
                    bool isStatic;
                    bool isFinal;
                    QString type;
                    QString name;
                    QString visibility;
                QList<Variable > inputValue;
                Method();
                QString Print(void);

            };
            class Interfac{
                public:
                    QString name;//имя класса
                   QString visibility;//свойств класса: видимость, final, абстрактный
                    QList<Method> methods;//список методов
                    QList<Variable> variables;//список полей
                    int numVariables;
                    int numMethods;
                    QList<QString> interf;//список интерфейсов, от которых наследуем

                public:
                    Interfac();
                    QString Print(void);

            };

            class ClassSet{
                public:
                    QString name;//имя класса
                    propertyClass property;//свойств класса: видимость, final, абстрактный
                    QString cl;//класс от которого наследуем
                    QList<QString> interf;//список интерфейсов, от которых наследуем
                    QList<Method> methods;//список методов
                    QList<Variable> variables;//список полей
                    int numVariables;
                    int numMethods;

                public:
                    ClassSet();
                    QString Print(void);
                    void SetName(QString &Classname);
                    void SetProperty(QString visability, bool isAbstract, bool isLeaf );


                };

		class CsharpHandler
		{
		public:
			explicit CsharpHandler(client::RepoApi const &api);
bool isTypeSuitable(QString const &type) const;
			QString generateToCsharp(QString const &pathToFile);
		private:
                        QList<Variable> CsharpHandler::getObjectVariable(Id const id);
                        QString CsharpHandler::getParent(Id const &id);
                        bool CsharpHandler::isInterfaceExist(QString const &name);
                        bool CsharpHandler::isClassExist(QString const &name) const;
void CreateSet(Id const &id);
                        int number;
                        int numberInterface;
                        QList<ClassSet> classes;
                        QList<Interfac> interfaces;
                        QSet<QString> nameInterface;
                        QSet<QString> nameClass;
			QString serializeObject(Id const &id, Id const &parentId);
			QString serializeChildren(Id const &id);
			QString getVisibility(Id const &id);
                            bool getAbstract(Id const &id);
                            bool getStatic(Id const &id);
                            bool getFinal(Id const &id);
			QString getType(Id const &id);
			QString getDefaultValue(Id const &id);
			QString getOperationFactors(Id const &id);
			QString hasModifier(Id const &id, QString const &modifier);
                       QList<QString>  getParents(Id const &id);

			QString serializeMultiplicity(Id const &id, QString const &multiplicity) const;

			bool isVisibilitySuitable(QString const &type) const;

			void addError(QString const &errorText);

			client::RepoApi const &mApi;
			QString mErrorText;
		};

	}
}
