#include <QString>
#include "../../../qrkernel/ids.h"

namespace qReal {
namespace generators {
class SmartLine {
public:
	enum IndentChangeType {increase, decrease, withoutChange, increaseDecrease};
	//increases WITHOUT this line
	//decreases WITH this line
	//increaseDecrease - decreases only for this line

	SmartLine(QString text, qReal::Id elementId, IndentChangeType tabLevelChange = withoutChange);

	QString text() const;
	qReal::Id elementId() const;
	IndentChangeType tabLevelChange() const;

	void setText(QString const &);
	void setElementId(qReal::Id const &);
	void setIndentChange(IndentChangeType);

private:
	QString mText;
	qReal::Id mElementId;
	IndentChangeType mIndentChange;
};
}
}
