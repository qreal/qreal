#include "blockParser.h"

#include <QMessageBox>

using namespace qReal;

BlockParser::BlockParser(gui::ErrorReporter* errorReporter)
	: ExpressionsParser(errorReporter){
}

BlockParser::~BlockParser() {
}

void BlockParser::parseVarPart(QString stream, int& pos)
{
	skip(stream, pos);
	if (stream.mid(pos, 4).compare("var ") == 0) {
		pos += 4;
		skip(stream, pos);
		if (!isEndOfStream(stream, pos) &&
				stream.mid(pos, 4).compare("int ") != 0 && stream.mid(pos, 7).compare("double ") != 0)
		{
			error(unexpectedSymbol, QString::number(pos + 1), "int\' or \'double", stream.at(pos));
			return;
		}

		while (pos < stream.length() &&
			   (stream.mid(pos, 4).compare("int ") == 0 || stream.mid(pos, 7).compare("double ") == 0))
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
			while (pos < stream.length() && stream.at(pos).toAscii() != ';') {
				skip(stream, pos);
				QString variable = parseIdentifier(stream, pos);
				if (hasErrors()) {
					return;
				}
				skip(stream, pos);

				Number n;
				if (isEndOfStream(stream, pos)) {
					return;
				}
				switch (stream.at(pos).toAscii()) {
				case '=':
					pos++;
					skip(stream, pos);
					n = parseExpression(stream, pos);
					n.setProperty("Type", curType);
					mVariables[variable] = n;
					break;
				case ',':
					pos++;
					mVariables[variable] = n;
					skip(stream, pos);
					if (pos == stream.length()) {
						error(unexpectedEndOfStream, QString::number(pos+1));
						return;
					}
					if (stream.at(pos).toAscii() == ';') {
						error(unexpectedSymbol, QString::number(pos+1), "\'letter",
							  QString(stream.at(pos).toAscii()));
						return;
					}
					break;
				default:
					if (!checkForColon(stream, pos)) {
						return;
					}
					mVariables[variable] = n;
					break;
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

void BlockParser::checkForVariable(QString nameOfVariable, int &index)
{
	if (!mVariables.contains(nameOfVariable)) {
			error(unknownIdentifier, QString::number(index + 1), "", nameOfVariable);
		}
}
