/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtCore/QString>
#include <QtXml/QDomElement>

#include <qrkernel/ids.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <qrrepo/logicalRepoApi.h>

namespace hascol {

/// Parser of Hascol sources, able to build structure and port mapping diagrams.
class HascolParser
{
public:
	/// Constructor
	/// @param api Mutable logical repo API to add hascol nodes to
	/// @param errorReporter Where to put various error and warning messages
	HascolParser(qrRepo::LogicalRepoApi &api, qReal::ErrorReporterInterface &errorReporter);

	/// Parse hascol sources
	/// @param files A list of files to parse
	void parse(QStringList const &files);

private:
	/// Creates a new structure diagram or removes all elements from existing
	/// diagram with a specified name.
	/// @param diagramName Name of the diagram to be prepared for adding elements
	/// @param diagramType Type of the diagram (structure or port mapping)
	qReal::Id initDiagram(QString const &diagramName, QString const &diagramType);

	/// Creates and adds element with filled required properties to a parent
	/// @param parent Parent of the new element
	/// @param elementType Type of an element to be created
	/// @param name Name of an element to be created
	qReal::Id addElement(qReal::Id const &parent, qReal::Id const &elementType, QString const &name);

	/// Runs hascol preprocessor from CoolKit that extracts structure information to an xml file
	/// @param fileName Name of a file to preprocess
	void preprocessFile(QString const &fileName);

	/// Parses one .xml file prepared by preprocessor
	/// @param fileName Name of .xml file to be parsed
	void parseFile(QString const &fileName);

	/// Parses process or functor XML node and creates related structures on structure and port map diagrams
	/// @param element XML node containing info about process or functor
	void parseProcess(QDomElement const &element);

	/// Parses process/functor ports and fills structure and port mapping diagrams
	/// @param ports XML nodes with information about ports
	/// @param direction Direction of these ports (in or out)
	/// @param parentOnAPortMap Node on a port map diagram that shall be parent of these ports
	/// @param parentOnAStructure Node on a structure diagram that shall be parent of these ports
	void parsePorts(QDomNodeList const &ports, QString const &direction
			, qReal::Id const &parentOnAPortMap, qReal::Id const &parentOnAStructure);

	/// Initializes required properties of a classifier in repo
	/// @param classifier Id of a classifier to be filled
	void initClassifierFields(qReal::Id const &classifier);

	/// Applies kind of "grid" layout with given width and height for a grid cell
	/// to top-level structures like processes
	/// @param diagram Diagram to apply layout to
	/// @param cellWidth Width of layout grid cell
	/// @param cellHeight Height of layout grid cell
	void doLayout(qReal::Id const &diagram, unsigned cellWidth, unsigned cellHeight);

	/// Does layout specifically on port mapping diagram
	void doPortMappingLayout();

	/// Does layout specifically on structure diagram
	void doStructureLayout();

	/// Lays out plugs on a structure diagram
	/// @param parent Id of a parent node
	void doPlugsLayout(qReal::Id const &parent);

	/// Lays out ports on a port map diagram
	/// @param parent Id of a parent node
	void doPortsLayout(qReal::Id const &parent);

	/// Lays out input and output ports on a port map diagram on a correct side of a process figure
	/// @param parent Id of a parent node
	/// @param margin X Coordinate of a vertical line on which ports shall be places
	/// @param direction Direction --- in or out --- of ports we currently placing
	/// @param count Overall count of ports we need to place
	void doLayoutForPortsType(qReal::Id const &parent, unsigned margin, QString const &direction, unsigned count);

	/// Port mapping diagram being built from Hascol sources
	qReal::Id mImportedPortMappingDiagramId;

	/// Structure diagram being built from Hascol sources
	qReal::Id mImportedStructureDiagramId;

	/// Logical repo API that will be filled with hascol entities from sources
	qrRepo::LogicalRepoApi &mApi;

	/// ErrorReporter object to which error and warning messages are put
	qReal::ErrorReporterInterface &mErrorReporter;
};

}
