#include "filehandle.h"

bool FileHandle::verifyFile(QString filename)
{
	QFile file(filename);

	if (!file.exists())
		return false;
	return true;
}

bool FileHandle::verifyDir(QString dirName)
{
	QDir dir(dirName);

	if (!dir.exists())
		return false;
	return true;
}
