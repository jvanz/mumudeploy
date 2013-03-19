
#include <QtGui>
#include <QtNetwork>
#include <iostream>

#include "client.h"

Client::Client() :  networkSession(0)
{

	// find out which IP to connect to
	QString ipAddress;
	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
	// use the first non-localhost IPv4 address
	for (int i = 0; i < ipAddressesList.size(); ++i) {
		if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
        		ipAddressesList.at(i).toIPv4Address()) {
	        	ipAddress = ipAddressesList.at(i).toString();
			break;
	        }
	}
    // if we did not find one, use IPv4 localhost
	if (ipAddress.isEmpty())
        	ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

	tcpSocket = new QTcpSocket(this);

	QNetworkConfigurationManager manager;
	if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        	// Get saved network configuration
	        QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
        	settings.beginGroup(QLatin1String("QtNetwork"));
	        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        	settings.endGroup();

	        // If the saved network configuration is not currently discovered use the system default
        	QNetworkConfiguration config = manager.configurationFromIdentifier(id);
	        if ((config.state() & QNetworkConfiguration::Discovered) != QNetworkConfiguration::Discovered) {
			config = manager.defaultConfiguration();
        	}

		networkSession = new QNetworkSession(config, this);
	        connect(networkSession, SIGNAL(opened()), this, SLOT(sessionOpened()));

        	networkSession->open();
	}
	
	QFile file("/home/vanz/client/file");
	if(file.open(QIODevice::WriteOnly)){
		std::cout<<"Client open file! :D"<<std::endl;
	}else{
		std::cout<<"Client cannot open de file :("<<std::endl;
	}
}

void Client::requestNewFortune()
{
	std::cout<<"Requesting new fortune"<<std::endl;
	blockSize = 0;
	tcpSocket->abort();
	tcpSocket->connectToHost("192.168.1.104",57390);
	std::cout<<"Fortune requested"<<std::endl;
}

void Client::readFortune()
{
	std::cout<<"Requesting new fortune"<<std::endl;
	QDataStream in(tcpSocket);
	in.setVersion(QDataStream::Qt_4_0);

	if (blockSize == 0) {
        	if (tcpSocket->bytesAvailable() < (int)sizeof(quint16)){
			return;
		}
		in >> blockSize;
	}

	if (tcpSocket->bytesAvailable() < blockSize){
	        return;
	}

	QString nextFortune;
	in >> nextFortune;

	if (nextFortune == currentFortune) {
        	QTimer::singleShot(0, this, SLOT(requestNewFortune()));
	        return;
	}

	currentFortune = nextFortune;
	std::cout<<"New fortune requested"<<std::endl;

}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
	switch (socketError) {
		case QAbstractSocket::RemoteHostClosedError:
			std::cout<<"RemoteHostClosedError"<<std::endl;
			break;
		case QAbstractSocket::HostNotFoundError:
			std::cout<<"HostNotFoundError"<<std::endl;
        		break;
		case QAbstractSocket::ConnectionRefusedError:
			std::cout<<"ConnectionRefusedError"<<std::endl;
		        break;
		default:
			std::cout<<"DefaultError"<<std::endl;
    }

}

void Client::sessionOpened()
{
	std::cout<<"Opening session"<<std::endl;
	// Save the used configuration
	QNetworkConfiguration config = networkSession->configuration();
	std::cout<<"apso configuration"<<std::endl;
	QString id;
	if (config.type() == QNetworkConfiguration::UserChoice){
		std::cout<<"if"<<std::endl;
        	id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
	}else{
		std::cout<<"else"<<std::endl;
	        id = config.identifier();
	}
	
	std::cout<<"Opa!"<<std::endl;
	QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
	settings.beginGroup(QLatin1String("QtNetwork"));
	settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
	settings.endGroup();
	std::cout<<"Session opened!"<<std::endl;
}

void Client::run()
{
	std::cout<<"Client running! :)"<<std::endl;
	int count = 0;
	while((count++) < 10){
		this->requestNewFortune();
		this->readFortune();
		std::cout<<currentFortune.toStdString()<<std::endl;
	}
	std::cout<<"Client finished."<<std::endl;
}
