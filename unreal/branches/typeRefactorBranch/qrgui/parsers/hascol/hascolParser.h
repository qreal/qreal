#pragma once

#include <QtCore/QString>
#include <QtXml/QDomElement>

#include "../../kernel/ids.h"
#include "../../mainwindow/errorReporter.h"

namespace qrRepo {
	class RepoApi;
}

namespace qReal {

	class EditorManager;

	namespace parsers {

		class HascolParser
		{
		public:
			explicit HascolParser(qrRepo::RepoApi &api, EditorManager const &editorManager);

			gui::ErrorReporter &parse(QStringList const &files);
		private:
                        NewType mImportedPortMappingDiagramId;
                        NewType mImportedStructureDiagramId;

			qrRepo::RepoApi &mApi;
			EditorManager const &mEditorManager;
			gui::ErrorReporter mErrorReporter;

                        NewType initDiagram(QString const &diagramName, QString const &diagramType);
                        NewType addElement(NewType const &parent, NewType const &elementType, QString const &name);
			void preprocessFile(QString const &fileName);
			void parseFile(QString const &fileName);
			void parseProcess(QDomElement const &element);
			void parsePorts(QDomNodeList const &ports, QString const &direction
                                , NewType const &parentOnAPortMap, NewType const &parentOnAStructure);
                        void initClassifierFields(NewType const &classifier);

                        void doLayout(NewType const &diagram, unsigned cellWidth, unsigned cellHeight);
			void doPortMappingLayout();
			void doStructureLayout();
                        void doPlugsLayout(NewType const &parent);
                        void doPortsLayout(NewType const &parent);
                        void doLayoutForPortsType(NewType const &parent, unsigned margin, QString const &direction, unsigned count);
		};

	}

}
