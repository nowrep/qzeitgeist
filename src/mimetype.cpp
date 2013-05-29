/*
 * This file is part of QZeitgeist.
 *
 * Copyright (C) 2013 David Rosca <nowrep@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
extern "C" {
#include <zeitgeist.h>
}

#include "mimetype.h"
#include <QtCore/QString>
#include <QtCore/QUrl>

namespace QZeitgeist
{

void MimeType::registerMimeType(const QString &mimeType, const QUrl &url, StringType sType)
{
    QByteArray mimeData = mimeType.toUtf8();
    QByteArray urlData = url.toString().toUtf8();

    if (sType == PlainString) {
        zeitgeist_register_mimetype(mimeData.constData(), urlData.constData());
    }
    else if (sType == RegExpString) {
        zeitgeist_register_mimetype_regex(mimeData.constData(), urlData.constData());
    }
}

QUrl MimeType::getInterpretationUrl(const QString &mimeType)
{
    QByteArray mimeData = mimeType.toUtf8();

    const gchar *val = zeitgeist_interpretation_for_mimetype(mimeData.constData());
    const QString uri = val;

    g_free((void *)val);

    return QUrl(uri);
}

void MimeType::registerUrlScheme(const QString &urlScheme, const QString &manifestationType)
{
    QByteArray schemeData = urlScheme.toUtf8();
    QByteArray manifData = manifestationType.toUtf8();

    zeitgeist_register_uri_scheme(schemeData.constData(), manifData.constData());
}

QString MimeType::getManifestation(const QUrl &url)
{
    QByteArray urlData = url.toString().toUtf8();

    const gchar *val = zeitgeist_manifestation_for_uri(urlData.constData());
    const QString manifestation = val;

    g_free((void *)val);

    return manifestation;
}

}; // namespace QZeitgeist

