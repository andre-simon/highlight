/***************************************************************************
                             syntax_chooser.cpp
                             -------------------
    begin                : Mo 17.03.2017
    copyright            : (C) 2017 by Andre Simon
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

#include "syntax_chooser.h"
#include "ui_syntax_chooser.h"

syntax_chooser::syntax_chooser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::syntax_chooser)
{
    ui->setupUi(this);
}

void syntax_chooser::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

syntax_chooser::~syntax_chooser()
{
    delete ui;
}

void syntax_chooser::addSyntaxName(const QString& name){
    ui->cbSyntaxNames->addItem(name);
}

void syntax_chooser::setUnclearExtension(const QString& ext){
    ui->lblDescription->setText(tr("The file extension %1 is assigned to multiple syntax definitions.\nSelect the correct one:").arg(ext));
}

QString syntax_chooser::getSelection(){
    return ui->cbSyntaxNames->currentText();
}

bool syntax_chooser::getRememberFlag(){
    return ui->cbRemember->isChecked();
}
