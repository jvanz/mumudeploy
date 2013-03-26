#include <QtDeclarative>
#include "filehandle.h"
#include "databasemanager.h"
#include "engine.h"

void Engine::start()
{
	qmlRegisterType<FileHandle>("mumu", 1, 0, "FileHandle");
	qmlRegisterType<DatabaseManager>("mumu", 1, 0, "DatabaseManager");
}
