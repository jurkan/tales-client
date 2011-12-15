/*
 * mapreader.cpp
 * Copyright 2008-2010, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
 * Copyright 2010, Jeff Bland <jksb@member.fsf.org>
 * Copyright 2010, Dennis Honeyman <arcticuno@gmail.com>
 *
 * This file is part of Tiled.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtDebug>
#include <QXmlStreamReader>

#ifndef XMLREADER_H
#define XMLREADER_H

class XmlReader : public QXmlStreamReader
{
public:
    XmlReader(QIODevice *device) : QXmlStreamReader(device) {}

    void readUnknownElement()
    {
        qDebug() << "Unknown element at line " << lineNumber()
                << " (fixme):" << name();
        skipCurrentElement();
    }

    QString attribute(QString name, QString def = "")
    {
        return attributes().hasAttribute(name)
                    ? attributes().value(name).toString()
                    : def;
    }

    int intAttribute(QString name, int def = 0)
    {
        bool ok;
        if (attributes().hasAttribute(name))
        {
            int ret = attributes().value(name).toString().toInt(&ok);
            if (ok)
                return ret;
        }

        return def;
    }
};

#endif // XMLREADER_H