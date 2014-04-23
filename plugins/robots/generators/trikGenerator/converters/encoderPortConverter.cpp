#include "encoderPortConverter.h"

using namespace trik::converters;

QString EncoderPortConverter::convert(QString const &data) const
{
	switch (data[0].toLatin1()) {
	case 'A':
		return "JB1";
	case 'B':
		return "JB2";
	case 'C':
		/// @todo this M1 disapoints me :(
		return "JB3";
	case 'J':
		// Got JM-form port
		return data.length() == 3 ? convert(data[2]) : data;
	case '1':
		return "JB1";
	case '2':
		return "JB2";
	case '3':
		return "JB3";
	case '4':
		return "JB4";
	default:
		return data;
	}
}
