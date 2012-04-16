#pragma once

@@includeConstraintsDiagramFiles@@
namespace constraints {

class ConstraintsPlugin : public QObject, public qReal::ConstraintsPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ConstraintsPluginInterface)

public:
	ConstraintsPlugin();
	virtual ~ConstraintsPlugin();

	virtual qReal::CheckStatus check(qReal::IdList const &elements, qrRepo::LogicalRepoApi const &logicalApi);
	virtual QString metamodelName() const;

private:
@@privateFieldsOfConstraintsDigrams@@
};

}
