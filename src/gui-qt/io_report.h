/***************************************************************************
                               io_report.h
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

#ifndef IO_REPORT_H
#define IO_REPORT_H
#include <QtGlobal>

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
#include <QtGui/QDialog>
#else
#include <QtWidgets/QDialog>
#endif

namespace Ui {
    class io_report;
}

class io_report : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(io_report)
public:
    explicit io_report(QWidget *parent = 0);
    virtual ~io_report();

    void addInputErrors(QStringList & list);
    void addOutputErrors(QStringList & list);
    void addReformatErrors(QStringList & list);

    bool removeInputErrorFiles();


protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::io_report *m_ui;
};

#endif // IO_REPORT_H
