#ifndef __QMLINTERFACE_H__
#define __QMLINTERFACE_H__

#include <QAbstractListModel>
#include <QSqlQuery>
#include <QVector>
#include <QObject>
#include <QStringList>

class QMLInterface : public QAbstractListModel
{
	Q_OBJECT
public:
	QMLInterface(QObject *parent = 0);
	Q_INVOKABLE void exec(QSqlQuery query);
	Q_INVOKABLE void refresh();
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role) const;

private:
	int m_columnCount;
	QSqlQuery m_query;
	QVector<QVector<QVariant>> m_dataList;

	QVector<QVariant> loadListFromCurrentRecord();
	void loadFieldMetaData();
};

#endif //__QMLINTERFACE_H__
