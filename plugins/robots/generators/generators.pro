TEMPLATE = subdirs

SUBDIRS = \
	generatorBase \
	trikGeneratorBase \
	trikQtsGenerator \
	nxtGeneratorBase \
	nxtOsekCGenerator \
	nxtRussianCGenerator \

trikGeneratorBase.file = $$PWD/trik/trikGeneratorBase/trikGeneratorBase.pro
trikQtsGenerator.file = $$PWD/trik/trikQtsGenerator/trikQtsGenerator.pro
nxtGeneratorBase.file = $$PWD/nxt/nxtGeneratorBase/nxtGeneratorBase.pro
nxtOsekCGenerator.file = $$PWD/nxt/nxtOsekCGenerator/nxtOsekCGenerator.pro
nxtRussianCGenerator.file = $$PWD/nxt/nxtRussianCGenerator/nxtRussianCGenerator.pro

trikGeneratorBase.depends = generatorBase
trikQtsGenerator.depends = trikGeneratorBase
nxtGeneratorBase.depends = generatorBase
nxtOsekCGenerator.depends = nxtGeneratorBase
nxtRussianCGenerator.depends = nxtGeneratorBase
