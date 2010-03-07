#pragma once

#include <QtCore/QString>
#include <QtXml/QDomElement>

#include "../../kernel/ids.h"

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

			void parse(QStringList const &files);
		private:
			Id mImportedPortMappingDiagramId;
			Id mImportedStructureDiagramId;

			qrRepo::RepoApi &mApi;
			EditorManager const &mEditorManager;

			Id initDiagram(QString const &diagramName, QString const &diagramType);
			Id addElement(Id const &parent, Id const &elementType, QString const &name);
			void preprocessFile(QString const &fileName);
			void parseFile(QString const &fileName);
			void parseProcess(QDomElement const &element);
			void parsePorts(QDomNodeList const &ports, QString const &direction
				, Id const &parentOnAPortMap, Id const &parentOnAStructure);
			void initClassifierFields(Id const &classifier);

			void doLayout(Id const &diagram, unsigned cellWidth, unsigned cellHeight);
			void doPortMappingLayout();
			void doStructureLayout();
			void doPlugsLayout(Id const &parent);
			void doPortsLayout(Id const &parent);
			void doLayoutForPortsType(Id const &parent, unsigned margin, QString const &direction, unsigned count);
		};
	}

}
