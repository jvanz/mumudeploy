#include <QSqlRecord>
#include <QHash>
#include <QSqlField>
#include "qmlinterface.h"

QMLInterface::QMLInterface(QObject *parent)
	: QAbstractListModel(parent)
	, m_columnCount(0)
	, m_query(NULL)
{
}

void QMLInterface::loadFieldMetaData()
{
	QHash<int, QByteArray> rolenames;
	QSqlRecord record = m_query.record();

	m_columnCount = record.count();

	for (int i = 0; i < m_columnCount; i++) {
		QString name = record.fieldName(i);
		rolenames.insert(Qt::UserRole + i + 1, name.toUtf8());
	}

	setRoleNames(rolenames);
}

QVariant QMLInterface::data(const QModelIndex &index, int role) const
{
        int columnIdx = role < Qt::UserRole ? index.column() : role - Qt::UserRole - 1;
        if ( role == Qt::DisplayRole || role > Qt::UserRole )
                return m_dataList[index.row()][columnIdx];
        return QVariant();
}

int QMLInterface::rowCount(const QModelIndex &parent) const 
{
        return parent.isValid() ? 0 : m_dataList.size();
}

QVector<QVariant> QMLInterface::loadListFromCurrentRecord()
{
	QVector<QVariant> result;
	for (int i = 0; i < m_columnCount; i++)
		result.append(m_query.value(i));
	return result;
}

void QMLInterface::exec(QSqlQuery query)
{
	m_query = query;

	if ( !m_query.isActive() )
		m_query.exec();

	loadFieldMetaData();

	while(m_query.next())
	{
		QVector<QVariant> cols = loadListFromCurrentRecord();
		m_dataList.push_back(cols);
	}
	reset();
}

void QMLInterface::refresh()
{
	int line = 0;

	if (!m_query.isActive())
		return;

	m_dataList.clear();
	m_query.exec();

	while (m_query.next())
	{
		// just to remove lines from view
		beginRemoveRows(QModelIndex(), line, line);
		endRemoveRows();

		beginInsertRows(QModelIndex(), line, line);
		auto row = loadListFromCurrentRecord();
		m_dataList.append(row);
		endInsertRows();

		line++;
	}
}
