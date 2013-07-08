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
#ifndef QZEITGEIST_SUBJECT_H_
#define QZEITGEIST_SUBJECT_H_

#include <QZeitgeist/QZeitgeist>
#include <QtCore/QMetaType>

class QUrl;
class QString;

namespace QZeitgeist
{

class QZEITGEIST_EXPORT Subject
{
public:
    Subject();
    Subject(const Subject &other);
    ~Subject();

    Subject &operator=(const Subject &other);
    bool operator==(const Subject &other) const;

    QUrl url() const;
    void setUrl(const QUrl &url);

    QUrl currentUrl() const;
    void setCurrentUrl(const QUrl &url);

    QUrl origin() const;
    void setOrigin(const QUrl &origin);

    QUrl currentOrigin() const;
    void setCurrentOrigin(const QUrl &origin);

    QUrl interpretation() const;
    void setInterpretation(const QUrl &interpretation);

    QUrl manifestation() const;
    void setManifestation(const QUrl &manifestation);

    QString text() const;
    void setText(const QString &text);

    QString storage() const;
    void setStorage(const QString &storage);

    QString mimeType() const;
    void setMimeType(const QString &mimeType);

    bool matchesTemplate(const Subject &subject) const;

    friend QZEITGEIST_EXPORT QDataStream &operator<<(QDataStream &stream, const Subject &subject);
    friend QZEITGEIST_EXPORT QDataStream &operator>>(QDataStream &stream, Subject &subject);

private:
    HANDLE createHandle() const;
    static Subject fromHandle(HANDLE handle);

    void detach();

    class SubjectPrivate *d;

    friend class Tools;
};

} // namespace QZeitgeist

Q_DECLARE_METATYPE(QZeitgeist::Subject)

#endif // QZEITGEIST_SUBJECT_H_
