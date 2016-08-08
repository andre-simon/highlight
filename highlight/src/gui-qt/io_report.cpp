/***************************************************************************
                               io_report.cpp
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

#include "io_report.h"
#include "ui_io_report.h"

io_report::io_report(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::io_report)
{
    m_ui->setupUi(this);
}

io_report::~io_report()
{
    delete m_ui;
}

void io_report::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void io_report::addInputErrors(QStringList & list)
{
    m_ui->listInputErrors->clear();
    m_ui->listInputErrors->addItems(list);
}

void io_report::addOutputErrors(QStringList & list)
{
    m_ui->listOutputErrors->clear();
    m_ui->listOutputErrors->addItems(list);
}
void io_report::addReformatErrors(QStringList & list)
{
    m_ui->listReformatErrors->clear();
    m_ui->listReformatErrors->addItems(list);
}

bool io_report::removeInputErrorFiles()
{
    return m_ui->cbRemoveFiles->isChecked();
}
