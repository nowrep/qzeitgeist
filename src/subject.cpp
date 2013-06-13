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

#include "subject.h"
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QDebug>

namespace QZeitgeist
{

class SubjectPrivate
{
public:
    explicit SubjectPrivate() { }
    bool compare(const SubjectPrivate &other) const;
    void copy(const SubjectPrivate &other);

    QUrl url;
    QUrl currentUrl;
    QUrl origin;
    QUrl currentOrigin;
    QUrl interpretation;
    QUrl manifestation;
    QString text;
    QString storage;
    QString mimeType;
};

bool SubjectPrivate::compare(const SubjectPrivate &other) const
{
    return url == other.url
           && currentUrl == other.currentUrl
           && origin == other.origin
           && currentOrigin == other.currentOrigin
           && interpretation == other.interpretation
           && manifestation == other.manifestation
           && text == other.text
           && storage == other.storage
           && mimeType == other.mimeType;
}

void SubjectPrivate::copy(const SubjectPrivate &other)
{
    if (this != &other) {
        url = other.url;
        currentUrl = other.currentUrl;
        origin = other.origin;
        currentOrigin = other.currentOrigin;
        interpretation = other.interpretation;
        manifestation = other.manifestation;
        text = other.text;
        storage = other.storage;
        mimeType = other.mimeType;
    }
}

// class Subject
Subject::Subject()
    : d(new SubjectPrivate)
{
}

Subject::Subject(const Subject &other)
    : d(new SubjectPrivate)
{
    d->copy(*other.d);
}

Subject::~Subject()
{
}

bool Subject::operator==(const Subject &other) const
{
    return d->compare(*other.d);
}

Subject &Subject::operator=(const Subject &other)
{
    d->copy(*other.d);
    return *this;
}

QUrl Subject::url() const
{
    return d->url;
}

void Subject::setUrl(const QUrl &url)
{
    d->url = url;
}

QUrl Subject::currentUrl() const
{
    return d->currentUrl;
}

void Subject::setCurrentUrl(const QUrl &url)
{
    d->currentUrl = url;
}


QUrl Subject::origin() const
{
    return d->origin;
}

void Subject::setOrigin(const QUrl &origin)
{
    d->origin = origin;
}

QUrl Subject::currentOrigin() const
{
    return d->currentOrigin;
}

void Subject::setCurrentOrigin(const QUrl &origin)
{
    d->currentOrigin = origin;
}

QUrl Subject::interpretation() const
{
    return d->interpretation;
}

void Subject::setInterpretation(const QUrl &interpretation)
{
    d->interpretation = interpretation;
}

QUrl Subject::manifestation() const
{
    return d->manifestation;
}

void Subject::setManifestation(const QUrl &manifestation)
{
    d->manifestation = manifestation;
}

QString Subject::text() const
{
    return d->text;
}

void Subject::setText(const QString &text)
{
    d->text = text;
}

QString Subject::storage() const
{
    return d->storage;
}

void Subject::setStorage(const QString &storage)
{
    d->storage = storage;
}

QString Subject::mimeType() const
{
    return d->mimeType;
}

void Subject::setMimeType(const QString &mimeType)
{
    d->mimeType = mimeType;
}

bool Subject::matchesTemplate(const Subject &subject) const
{
    ZeitgeistSubject *self = (::ZeitgeistSubject *)createHandle();
    ZeitgeistSubject *other = (::ZeitgeistSubject *)subject.createHandle();

    bool ret = zeitgeist_subject_matches_template(self, other);

    g_object_unref(self);
    g_object_unref(other);
    return ret;
}

static inline const gchar *passData(const QByteArray &data)
{
    return data.isEmpty() ? 0 : data.constData();
}

HANDLE Subject::createHandle() const
{
    QByteArray urlData = d->url.toString().toUtf8();
    QByteArray currentUrlData = d->currentUrl.toString().toUtf8();
    QByteArray originData = d->origin.toString().toUtf8();
    QByteArray currentOriginData = d->currentOrigin.toString().toUtf8();
    QByteArray interpretationData = d->interpretation.toString().toUtf8();
    QByteArray manifestationData = d->manifestation.toString().toUtf8();
    QByteArray textData = d->text.toUtf8();
    QByteArray storageData = d->storage.toUtf8();
    QByteArray mimeData = d->mimeType.toUtf8();

    // The only difference between new_full and new_move_event is that
    // new_move_event is able to initialize all data at once ;-)
    return zeitgeist_subject_new_move_event(passData(urlData),
                                            passData(originData),
                                            passData(currentUrlData),
                                            passData(currentOriginData),
                                            passData(interpretationData),
                                            passData(manifestationData),
                                            passData(mimeData),
                                            passData(textData),
                                            passData(storageData));
}

// static
Subject Subject::fromHandle(HANDLE handle)
{
    ZeitgeistSubject *subject = (ZeitgeistSubject *)handle;
    Subject sub;

    sub.d->url = QUrl(zeitgeist_subject_get_uri(subject));
    sub.d->currentUrl = QUrl(zeitgeist_subject_get_current_uri(subject));
    sub.d->origin = QUrl(zeitgeist_subject_get_origin(subject));
    sub.d->currentOrigin = QUrl(zeitgeist_subject_get_current_origin(subject));
    sub.d->interpretation = QUrl(zeitgeist_subject_get_interpretation(subject));
    sub.d->manifestation = QUrl(zeitgeist_subject_get_manifestation(subject));
    sub.d->text = zeitgeist_subject_get_text(subject);
    sub.d->storage = zeitgeist_subject_get_storage(subject);
    sub.d->mimeType = zeitgeist_subject_get_mimetype(subject);

    return sub;
}

static const int streamVersion = 1;

QDataStream &operator<<(QDataStream &stream, const Subject &subject)
{
    stream << streamVersion;
    stream << subject.d->url;
    stream << subject.d->currentUrl;
    stream << subject.d->origin;
    stream << subject.d->currentOrigin;
    stream << subject.d->interpretation;
    stream << subject.d->manifestation;
    stream << subject.d->text;
    stream << subject.d->storage;
    stream << subject.d->mimeType;

    return stream;
}

QDataStream &operator>>(QDataStream &stream, Subject &subject)
{
    int version = -1;
    stream >> version;

    if (version != streamVersion) {
        qWarning() << "Subject: Invalid stream version!";
        return stream;
    }

    stream >> subject.d->url;
    stream >> subject.d->currentUrl;
    stream >> subject.d->origin;
    stream >> subject.d->currentOrigin;
    stream >> subject.d->interpretation;
    stream >> subject.d->manifestation;
    stream >> subject.d->text;
    stream >> subject.d->storage;
    stream >> subject.d->mimeType;

    return stream;
}

}; // namespace QZeitgeist

