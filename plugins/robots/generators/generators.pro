TEMPLATE = subdirs

SUBDIRS = \
	robotsGeneratorBase \
	nxtGenerator \
	trikGenerator \
	russianCGenerator \

nxtGenerator.depends = robotsGeneratorBase
trikGenerator.depends = robotsGeneratorBase
russianCGenerator.depends = robotsGeneratorBase
