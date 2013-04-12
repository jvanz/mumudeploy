#include <QtDeclarative>
#include "filehandle.h"
#include "databasemanager.h"
#include "mumufriend.h"
#include "engine.h"

void Engine::start()
{
	qmlRegisterType<FileHandle>("mumu", 1, 0, "FileHandle");
	qmlRegisterType<DatabaseManager>("mumu", 1, 0, "DatabaseManager");
	qmlRegisterType<MumuFriend>("mumu", 1, 0, "MumuFriend");
}
