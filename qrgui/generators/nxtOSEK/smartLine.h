namespace qReal {
namespace generators {
class SmartLine {
public:
	enum TabLevelChangeType {increase, decrease, withoutChange};

	SmartLine(QString text, qReal::Id elementId, TabLevelChangeType tabLevelChange = withoutChange);

	QString text() const;
	qReal::Id elementId() const;
	TabLevelChangeType tabLevelChange() const;

	void setText(QString const &);
	void setElementId(qReal::Id const &);
	void setTabLevelChange(TabLevelChangeType):

private:
	QString mText;
	qReal::Id mElementId;
	TabLevelChangeType mTabLevelChange;
};
}
}
