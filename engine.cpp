#include <QtDeclarative>
#include "filehandle.h"
#include "engine.h"

void Engine::start()
{
	qmlRegisterType<FileHandle>("mumu", 1, 0, "FileHandle");
}
