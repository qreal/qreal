generate.commands = qcollectiongenerator qreal-robots.qhcp -o qreal-robots.qhc
generate.depends = qreal-robots.qhcp qreal-robots.qhp

QMAKE_EXTRA_TARGETS += generate

TARGET = generate
