#ifndef __QMLINTERFACE_H__
#define __QMLINTERFACE_H__

#include <QAbstractListModel>
#include <QSqlQuery>
#include <QVector>
#include <QObject>
#include <QStringList>

/**
 * QMLInterface - Bring the data of the database
 */
class QMLInterface : public QAbstractListModel
{
	Q_OBJECT
public:
	QMLInterface(QObject *parent = 0); ///< default contructor
	Q_INVOKABLE void exec(QSqlQuery query); ///< executes a SQL
	Q_INVOKABLE void refresh(); ///< refresh data bringing the data of database
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const; ///< number of rows fetched
	virtual QVariant data(const QModelIndex &index, int role) const; ///< type of data

private:
	int m_columnCount; ///< number of columns of the query
	QSqlQuery m_query; ///< query to execute
	QVector<QVector<QVariant>> m_dataList; /// data of the database

	QVector<QVariant> loadListFromCurrentRecord(); ///< bring data of the database to a record
	void loadFieldMetaData(); ///< load data in the field
};

#endif //__QMLINTERFACE_H__
