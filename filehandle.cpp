#include "filehandle.h"

bool FileHandle::verifyFile(QString filename)
{
	QFile file(filename);

	if (!file.exists())
		return false;
	return true;
}
