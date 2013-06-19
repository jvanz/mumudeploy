#include <QDir>
#include "filehandle.h"

bool FileHandle::verifyPath(QString path)
{
	bool ret = true;
	QString curr = QDir::currentPath();

	QDir::setCurrent(getDirUserHome().path());

	if (!verifyFile(path) && !verifyDir(path))
		ret = false;

	if (!ret) {
		QDir::setCurrent(getPublicUserHome().path() + "/file");
		
		if (!verifyFile(path) && !verifyDir(path))
			ret = false;
	}

	QDir::setCurrent(curr);

	return ret;
}

bool FileHandle::verifyFile(QString filename)
{
	QFile file(filename);
	return file.exists();
}

bool FileHandle::verifyDir(QString dirName)
{
	QDir dir(dirName);
	return dir.exists();
}

QString FileHandle::getUserHome()
{
	QDir home(QDir::toNativeSeparators(QDir::homePath() + "/.mumudeploy/"));

	if (!home.exists())
		home.mkdir(home.path());
	
	return home.path();
}
	
QDir FileHandle::getDirUserHome()
{
	QDir home(QDir::toNativeSeparators(QDir::homePath() + "/.mumudeploy/"));
	return home;
}
	
QDir FileHandle::getPublicUserHome()
{
	QDir home(QDir::toNativeSeparators(QDir::homePath() + "/mumudeploy/"));
	if (!home.exists())
		home.mkdir(home.path());
	
	return home;
}
