#include "patterns.h"

#include "typeinfo"

/* Partial patterns */

bool Wildcard::matches(const IdPart &part) const {
	Q_UNUSED(part);
	return true;
}
bool Wildcard::equals(const IdPart &part) const {
	try {
		const Wildcard &any = dynamic_cast<const Wildcard&>(part);
		Q_UNUSED(any);
		return true;
	} catch (int e) {
		return false;
	}
}

TextPart::TextPart(const QString &text) :
	text(text)
{}

bool TextPart::equals(const IdPart &part) const {
	try {
		const TextPart &text = dynamic_cast<const TextPart&>(part);
		return this->text == text.text;
	} catch (int e) {
		return false;
	}
}
bool TextPart::matches(const IdPart &part) const {
	return equals(part);
}

/* Id patterns*/

DiagramPattern::DiagramPattern(const IdPart &editor, const IdPart &diagram) :
	editor(editor),
	diagram(diagram)
{}

DiagramPattern::DiagramPattern(const DiagramPattern &copy) :
	editor(copy.editor),
	diagram(copy.diagram)
{}

DiagramPattern::DiagramPattern(const Id &id) :
	editor(TextPart(id.editor())),
	diagram(TextPart(id.diagram()))
{}

bool DiagramPattern::matches(const DiagramPattern &pattern) const {
	return  this->editor.matches(pattern.editor) &&
			this->diagram.matches(pattern.diagram);
}

ElementPattern::ElementPattern(const IdPart &editor, const IdPart &diagram, const IdPart &element) :
	editor(editor),
	diagram(diagram),
	element(element)
{}

ElementPattern::ElementPattern(const ElementPattern &copy) :
	editor(copy.editor),
	diagram(copy.diagram),
	element(copy.element)
{}

ElementPattern::ElementPattern(const Id &id) :
	editor(TextPart(id.editor())),
	diagram(TextPart(id.editor())),
	element(TextPart(id.editor()))
{}

bool ElementPattern::matches(const ElementPattern &pattern) const {
	return  this->editor.matches(pattern.editor) &&
			this->diagram.matches(pattern.diagram) &&
			this->element.matches(pattern.element);
}
