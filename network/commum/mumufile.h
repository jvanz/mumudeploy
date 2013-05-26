#include <QList>
#include <QFile>
#include <QByteArray>
#include <QObject>

#include "mumublock.h"
#include "filedescriptor.h"


/**
 * MumuFile - The struct of the file that will be used by server
 */

class MumuFile : QObject
{
	Q_OBJECT
	Q_PROPERTY(QByteArray md5 READ getMd5 WRITE setMd5)
	Q_PROPERTY(QList<MumuBlock> blocks READ getBlocks WRITE setBlocks)

private:
	QList<MumuBlock> blocks; ///< List of all blocks of MumuFile
	FileDescriptor descriptor;
	QFile * file; ///< File inside MumuFile

public:
	MumuFile(QString); ///< Default constructor
	MumuFile(QByteArray);
	MumuFile(FileDescriptor);
	void splitFile(int blocksCount = 3);  ///< Split file in chunks that will be send
	QByteArray getMd5(); ///< Get the MD5 of the file for comparisson
	void setMd5(QByteArray); ///< Set the MD5 of the file
	QList<MumuBlock> getBlocks(); ///< A list of the blocks that will be send
	void setBlocks(QList<MumuBlock>); ///< Populate the list of blocks tht will be send
	int getSize(); ///< Return the number os blocks
	MumuBlock getBlock(int); ///< Get the block at position
	FileDescriptor getFileDescriptor(); ///< Return the file descriptor of MumuFile
	QFile* getFile(); ///< Get the file inside of MumuFile
	QString fileName(); ///< Get the name of file
	bool exists(); ///< Verify if the file exists
	static QByteArray compress(const QByteArray& data); ///< Compress a packet that will be send
	static QByteArray uncompress(const QByteArray& data); ///< Uncompress a received packet
	static FileDescriptor* processFileDescriptorBlock(QByteArray);
};
