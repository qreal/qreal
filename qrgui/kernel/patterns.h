#pragma once

#include "ids.h"

#include "typeinfo"

#include <QString>

using namespace qReal;

class IdPart {
	public:
		virtual bool matches(const IdPart &part) const = 0;
		virtual bool equals(const IdPart &part) const {
			Q_UNUSED(part);
			return false;
		}
};
inline bool operator==(const IdPart &first, const IdPart &second) {
	return first.equals(second);
}

class Wildcard : public IdPart {
	public:
		Wildcard();
		bool matches(const IdPart &part) const;
		bool equals(const IdPart &part) const;
};
inline uint qHash(const Wildcard &any) {Q_UNUSED(any);return qHash("any");}

class TextPart : public IdPart {
	public:
		TextPart(const QString &text);
		bool matches(const IdPart &part) const;
		bool equals(const IdPart &part) const;
		const QString text;
};
inline uint qHash(const TextPart &text) {return qHash(text.text);}

//todo: нужны ли более мелкие паттерны для Node и Edge?
//todo: наверное, лучше всего такое сделать отдельно условиями

//todo: добавить EditorPattern и объединить эти классы в иерархию

class DiagramPattern {
	public:
		DiagramPattern(
			const IdPart &editor,
			const IdPart &diagram);
		DiagramPattern(const Id &id);
		DiagramPattern(const DiagramPattern &copy);

		bool matches(const DiagramPattern &pattern) const;

		const IdPart &editor;
		const IdPart &diagram;
};
inline uint qHash(const DiagramPattern &diagram) {
	return qHash(&diagram.editor) + qHash(&diagram.diagram);
}
inline bool operator==(const DiagramPattern &first, const DiagramPattern &second) {
	return  first.editor == second.editor &&
			first.diagram == second.diagram;
}

class ElementPattern {
	public:
		ElementPattern(
			const IdPart &editor,
			const IdPart &diagram,
			const IdPart &element);
		ElementPattern(const Id &id);
		ElementPattern(const ElementPattern &copy);

		bool matches(const ElementPattern &pattern) const;

		const IdPart &editor;
		const IdPart &diagram;
		const IdPart &element;
};
inline uint qHash(const ElementPattern &element) {
	return qHash(&element.editor) + qHash(&element.diagram) + qHash(&element.element);
}
inline bool operator==(const ElementPattern &first, const ElementPattern &second) {
	return  first.editor == second.editor &&
			first.diagram == second.diagram &&
			first.element == second.element;
}
