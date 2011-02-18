#pragma once

#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtCore/QHash>

#include "../../kernel/ids.h"
#include "../../mainwindow/errorReporter.h"
#include "../../../qrrepo/logicalRepoApi.h"

namespace qReal {

	namespace generators {

		class EditorGenerator
		{
		public:
			explicit EditorGenerator(qrRepo::LogicalRepoApi const &api);

			QHash<Id, QString> getMetamodelList();
			gui::ErrorReporter& generateEditor(Id const metamodelId, QString const &pathToFile);
		private:
			void serializeObjects(QDomElement &parent, Id const &idParent);
                        void createImport(QDomElement &parent, Id const &type);
                        void createNode(QDomElement &parent, Id const &type);
                        void createEdge(QDomElement &parent, Id const &type);
                        void createEnum(QDomElement &parent, Id const &type);
                        void setProperties(QDomElement &parent, Id const &type);
                        void setContextMenuFields(QDomElement &parent, Id const &type);
                        void setValues(QDomElement &parent, Id const &type);
                        void setAssociations(QDomElement &parent, Id const &type);
                        void setUsages(QDomElement &parent, Id const &type);
                        void setConnections(QDomElement &parent, Id const &type);
                        void setGeneralization(QDomElement &parent, Id const &type);
                        void setPossibleEdges(QDomElement &parent, Id const &type);
                        void setPin(QDomElement &parent, Id const &type);
                        void setStatusElement(QDomElement &parent, Id const &type, QString const &tagName, QString const &propertyName);
                        void setAction(QDomElement &parent, Id const &type);
                        void createDiagrams(QDomElement &parent, Id const &type);
                        void setContainer(QDomElement &parent, Id const &type);
                        void setContainerProperties(QDomElement &parent, Id const &type);
                        void newSetConnections(QDomElement &parent, const Id &type,
					QString const &commonTagName, QString const &internalTagName, QString const &typeName);
                        void ensureCorrectness (Id const &type, QDomElement element, QString const &tagName, QString const &value);
                        void setBoolValuesForContainer (QString const &propertyName, QDomElement &properties, Id const &type);
                        void setSizesForContainer (QString const &propertyName, QDomElement &properties, Id const &type);
			static void copyImages(QString const &pathToFile);

			qrRepo::LogicalRepoApi const &mApi;
			QDomDocument mDocument;
			QString mErrorText;
			IdList mElements;
			QString mDiagramName;
			gui::ErrorReporter mErrorReporter;
		};
	}
}
