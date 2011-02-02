#pragma once

#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtCore/QHash>

#include "../../kernel/ids.h"
#include "../../mainwindow/errorReporter.h"

namespace qrRepo {
	class RepoApi;
}

namespace qReal {

	namespace generators {

		class EditorGenerator
		{
		public:
			explicit EditorGenerator(qrRepo::RepoApi const &api);

                        QHash<NewType, QString> getMetamodelList();
                        gui::ErrorReporter& generateEditor(NewType const metamodelId, QString const &pathToFile);
		private:
                        void serializeObjects(QDomElement &parent, NewType const &idParent);
                        void createImport(QDomElement &parent, NewType const &type);
                        void createNode(QDomElement &parent, NewType const &type);
                        void createEdge(QDomElement &parent, NewType const &type);
                        void createEnum(QDomElement &parent, NewType const &type);
                        void setProperties(QDomElement &parent, NewType const &type);
                        void setContextMenuFields(QDomElement &parent, NewType const &type);
                        void setValues(QDomElement &parent, NewType const &type);
                        void setAssociations(QDomElement &parent, NewType const &type);
                        void setUsages(QDomElement &parent, NewType const &type);
                        void setConnections(QDomElement &parent, NewType const &type);
                        void setGeneralization(QDomElement &parent, NewType const &type);
                        void setPossibleEdges(QDomElement &parent, NewType const &type);
                        void setPin(QDomElement &parent, NewType const &type);
                        void setStatusElement(QDomElement &parent, NewType const &type, QString const &tagName, QString const &propertyName);
                        void setAction(QDomElement &parent, NewType const &type);
                        void createDiagrams(QDomElement &parent, NewType const &type);
                        void setContainer(QDomElement &parent, NewType const &type);
                        void setContainerProperties(QDomElement &parent, NewType const &type);
                        void newSetConnections(QDomElement &parent, const NewType &type,
					QString const &commonTagName, QString const &internalTagName, QString const &typeName);
                        void ensureCorrectness (NewType const &type, QDomElement element, QString const &tagName, QString const &value);
                        void setBoolValuesForContainer (QString const &propertyName, QDomElement &properties, NewType const &type);
                        void setSizesForContainer (QString const &propertyName, QDomElement &properties, NewType const &type);

			qrRepo::RepoApi const &mApi;
			QDomDocument mDocument;
			QString mErrorText;
                        TypeList mElements;
			QString mDiagramName;
			gui::ErrorReporter mErrorReporter;
		};
	}
}
