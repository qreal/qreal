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

#include "blockParser.h"

using namespace qReal;
using namespace utils;

BlockParser::BlockParser(ErrorReporterInterface* errorReporter)
		: ExpressionsParser(errorReporter)
{
}

void BlockParser::parseVarPart(QString const &stream, int &pos)
{
	skip(stream, pos);
	if (stream.mid(pos, 4).compare("var ") == 0) {
		pos += 4;
		skip(stream, pos);
		if (!isEndOfStream(stream, pos)
				&& stream.mid(pos, 4).compare("int ") != 0 && stream.mid(pos, 7).compare("double ") != 0)
		{
			error(unexpectedSymbol, QString::number(pos + 1), tr("int\' or \'double"), stream.at(pos));
			return;
		}

		while (pos < stream.length()
				&& (stream.mid(pos, 4).compare("int ") == 0
				|| stream.mid(pos, 7).compare("double ") == 0))
		{
			Number::Type curType;
			if (stream.mid(pos, 4).compare("int ") == 0) {
				curType = Number::intType;
				pos += 4;
			} else {
				curType = Number::doubleType;
				pos += 7;
			}
			skip(stream, pos);
			while (pos < stream.length() && stream.at(pos).toLatin1() != ';') {
				skip(stream, pos);
				QString const variable = parseIdentifier(stream, pos);
				if (hasErrors()) {
					return;
				}
				skip(stream, pos);

				if (isEndOfStream(stream, pos)) {
					return;
				}

				switch (stream.at(pos).toLatin1()) {
				case '=':
					{
						pos++;
						skip(stream, pos);
						Number *temp = parseExpression(stream, pos);
						temp->setType(curType);
						mVariables[variable] = temp;
						break;
					}
				case ',':
					{
						pos++;
						mVariables[variable] = new Number();
						skip(stream, pos);
						if (pos == stream.length()) {
							error(unexpectedEndOfStream, QString::number(pos+1));
							return;
						}
						if (stream.at(pos).toLatin1() == ';') {
							error(unexpectedSymbol, QString::number(pos + 1),
									tr("\'letter"),
									QString(stream.at(pos).toLatin1())
							);
							return;
						}
						break;
					}
				default:
					{
						if (!checkForColon(stream, pos)) {
							return;
						}

						mVariables[variable] = new Number();
						break;
					}
				}
				skip(stream, pos);
			}

			if (hasErrors()) {
				return;
			}
			pos++;
			skip(stream, pos);
		}
	}
}

void BlockParser::checkForVariable(QString const &nameOfVariable, int &index)
{
	if (!mVariables.contains(nameOfVariable)) {
		error(unknownIdentifier, QString::number(index + 1), "", nameOfVariable);
	}
}
