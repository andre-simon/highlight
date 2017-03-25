/***************************************************************************
                          charcodes.cpp  -  description
                             -------------------
    begin                : Wed Nov 24 2003
    copyright            : (C) 2003 by Andre Simon
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


#ifndef CHAR_CODES
#define CHAR_CODES

#ifdef _WIN32

#define AUML_LC 228
#define OUML_LC 246
#define UUML_LC 252

#define AUML_UC 196
#define OUML_UC 214
#define UUML_UC 220


#define AACUTE_LC 225
#define EACUTE_LC 233
#define OACUTE_LC 243
#define UACUTE_LC 250

#define AACUTE_UC 193
#define EACUTE_UC 201
#define OACUTE_UC 211
#define UACUTE_UC 218

#define AGRAVE_LC 224
#define EGRAVE_LC 232
#define OGRAVE_LC 242
#define UGRAVE_LC 249

#define AGRAVE_UC 192
#define EGRAVE_UC 200
#define OGRAVE_UC 210
#define UGRAVE_UC 217

#define SZLIG 223

#else

#define AUML_LC 164
#define OUML_LC 182
#define UUML_LC 188

#define AUML_UC 132
#define OUML_UC 150
#define UUML_UC 156


#define AACUTE_LC 161
#define EACUTE_LC 169
#define OACUTE_LC 179
#define UACUTE_LC 186

#define AACUTE_UC 129
#define EACUTE_UC 137
#define OACUTE_UC 147
#define UACUTE_UC 154

#define AGRAVE_LC 160
#define EGRAVE_LC 168
#define OGRAVE_LC 178
#define UGRAVE_LC 185

#define AGRAVE_UC 128
#define EGRAVE_UC 136
#define OGRAVE_UC 146
#define UGRAVE_UC 153

#define SZLIG 159

#endif

#endif
