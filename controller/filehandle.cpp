#include <QDir>
#include "filehandle.h"

bool FileHandle::verifyPath(QString path)
{
	if (!verifyFile(path) && !verifyDir(path))
		return false;
	return true;
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
	if (!home.exists())
		home.mkdir(home.path());
	return home;
}
	
QDir FileHandle::getPublicUserHome()
{
	QDir home(QDir::toNativeSeparators(QDir::homePath() + "/mumudeploy/"));
	if (!home.exists())
		home.mkdir(home.path());
	
	return home;
}
