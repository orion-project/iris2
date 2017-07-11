INCLUDEPATH += $$PWD

DEFINES += NO_QWT

QT += xml

SOURCES += \
    $$PWD/qwt-mml-dev/qwt_mml_document.cpp \
    $$PWD/qwt-mml-dev/qwt_mml_entity_table.cpp \
    $$PWD/qwt-mml-dev/formulaview.cpp

HEADERS  += \
    $$PWD/qwt-mml-dev/qwt_mml_document.h \
    $$PWD/qwt-mml-dev/qwt_mml_entity_table.h \
    $$PWD/qwt-mml-dev/formulaview.h
