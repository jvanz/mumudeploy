#include <QtDeclarative>
#include "filehandle.h"
#include "mumufriend.h"
#include "engine.h"

void Engine::start()
{
	qmlRegisterType<FileHandle>("mumu", 1, 0, "FileHandle");
	qmlRegisterType<MumuFriend>("mumu", 1, 0, "MumuFriend");
}
