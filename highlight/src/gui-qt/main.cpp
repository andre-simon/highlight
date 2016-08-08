/***************************************************************************
                               main.cpp
                             -------------------
    begin                : Mo 16.03.2009
    copyright            : (C) 2009 by Andre Simon
    email                : andre.simon1@gmx.de
 ***************************************************************************/


/*
This file is part of Highlight.

Highlight is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Highlight is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Highlight.  If not, see <http://www.gnu.org/licenses/>.
*/


//?? https://codereview.qt-project.org/#change,51048

#include <QtGlobal>

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
#include <QtGui/QApplication>
#else
#include <QtWidgets/QApplication>
#endif
#include <QTranslator>
#include <QLocale>
#include <QDir>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTranslator translator;
#ifdef DATA_DIR
    translator.load(QString("%1/gui_files/l10n/highlight_%2").arg(DATA_DIR).arg(QLocale::system().name()));
#else
    translator.load(QString("%1/gui_files/l10n/highlight_%2").arg(QDir::currentPath()).arg(QLocale::system().name()));
#endif
    app.installTranslator(&translator);

    if (QCoreApplication::arguments().contains("--portable")) {
        QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, QDir::currentPath());
    }

    MainWindow w;
    /*
        QStringList args=QCoreApplication::arguments();
        if (args.count()>1){
            args.removeFirst(); // drop highlight-gui.exe path
            w.addInputFiles(args);
        }
    */
    w.show();
    return app.exec();
}
