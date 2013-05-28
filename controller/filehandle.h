#ifndef __FILE_HANDLE_H__
#define __FILE_HANDLE_H__

#include <QFile>
#include <QDir>

/**
 * FileHandle - The class who verifies for files and dirs
 */
class FileHandle : public QObject
{
	Q_OBJECT

public:
	FileHandle() : QObject(0) {} /// default constructor
	static QString getUserHome(); /// returns the user home dir path
	static QDir getDirUserHome(); /// returns the user home dir
	static QDir getPublicUserHome(); /// returns the public user home dir
	
public slots:
	Q_INVOKABLE static bool verifyPath(QString path); /// verify if a path exists (if the path points to a file or a dir)
	Q_INVOKABLE static bool verifyFile(QString fileName); /// verify if the fileName points to a valid file
	Q_INVOKABLE static bool verifyDir(QString dirName); /// verify if the dirName points to a a valid dir
};
#endif //__FILE_HANDLE_H__
