QT -= gui
QT += core network sql
CONFIG += console
CONFIG -= app_bundle

SOURCES += \
	src/main.cpp \
	src/server.cpp \
	src/database.cpp \
	src/utils/utils.cpp

HEADERS += \
	src/stdafx.h \
	src/server.h \
	src/database.h \
	src/protocol.h \
	src/utils/utils.h

# 改用QODBC实现
# LIBS += -L$$PWD/libs/mysqlclient   # Linux/Mac 需要安装libmysqlclient，Windows需要链接库

