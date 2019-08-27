TEMPLATE = app
TARGET = GSample

QT += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += gstreamer-1.0 gstreamer-video-1.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    GstPipeElement.cpp \
    PipelineFactory.cpp \
    VidPipeline.cpp \
    GstElementComponent.cpp \
    Transform3D.cpp

HEADERS += \
    mainwindow.h \
    Vertex.h \
    GstPipeElement.h \
    PipelineFactory.h \
    interfaces.h \
    VidPipeline.h \
    GstElementComponent.h \
    Transform3D.h

target.path = /home/root
xml.path = /run/media/mmcblk2p6/xml/
xml.files = xml/*
INSTALLS += target
INSTALLS += xml

DISTFILES += \
    xml/* \

