
#    <BodePlot is a plotter for bode diagrams after entering a transfer function
#    and additionally calculates phase and amplitude margins.>
#    <BodePlot berechnet das Bode-Diagramm nach Eingabe einer Übertragungsfunktion
#    und berechnet zusätzlich Phasen- und Amplitudenreserven.>
#    Copyright (C) <2024>  <Carlo Kaulbarsch> <mail: ckaulbarsch@web.de>

#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.

#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.

#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.




QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BodePlot.cpp \
    main.cpp \
    qcustomplot.cpp

HEADERS += \
    BodePlot.h \
    qcustomplot.h

FORMS += \
    BodePlot.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
