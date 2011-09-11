#include <QString>
#include "../../../qrkernel/ids.h"

namespace qReal {
namespace generators {
class SmartLine {
public:
	enum TabLevelChangeType {increase, decrease, withoutChange, increaseDecrease};
	//increases WITHOUT this line
	//decreases WITH this line
	//increaseDecrease - decreases only for this line

	SmartLine(QString text, qReal::Id elementId, TabLevelChangeType tabLevelChange = withoutChange);

	QString text() const;
	qReal::Id elementId() const;
	TabLevelChangeType tabLevelChange() const;

	void setText(QString const &);
	void setElementId(qReal::Id const &);
	void setTabLevelChange(TabLevelChangeType);

private:
	QString mText;
	qReal::Id mElementId;
	TabLevelChangeType mTabLevelChange;
};
}
}
