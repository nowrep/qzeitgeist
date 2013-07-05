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
#ifndef QZEITGEIST_DATASOURCE_H_
#define QZEITGEIST_DATASOURCE_H_

#include <QZeitgeist/QZeitgeist>
#include <QZeitgeist/Event>
#include <QtCore/QList>

namespace QZeitgeist
{

class QZEITGEIST_EXPORT DataSource
{
public:
    DataSource();
    DataSource(const DataSource &other);
    ~DataSource();

    DataSource &operator=(const DataSource &other);
    bool operator==(const DataSource &other) const;

    bool isEnabled() const;
    void setEnabled(bool enabled);

    bool isRunning() const;
    void setRunning(bool running);

    qint64 timestamp() const;
    void setTimestamp(qint64 timestamp);

    QString uniqueId() const;
    void setUniqueId(const QString &id);

    QString name() const;
    void setName(const QString &name);

    QString description() const;
    void setDescription(const QString &description);

    QList<Event> eventTemplates() const;
    void setEventTemplates(const QList<Event> &eventTemplates);

    HANDLE createHandle() const;
    static DataSource fromHandle(HANDLE handle);

    friend QZEITGEIST_EXPORT QDataStream &operator<<(QDataStream &stream, const DataSource &dataSource);
    friend QZEITGEIST_EXPORT QDataStream &operator>>(QDataStream &stream, DataSource &dataSource);

private:
    void detach();

    class DataSourcePrivate *d;
    friend class DataSourcePrivate;
};

} // namespace QZeitgeist

Q_DECLARE_METATYPE(QZeitgeist::DataSource)

#endif // QZEITGEIST_DATASOURCE_H_
