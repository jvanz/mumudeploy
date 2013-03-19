
#include <QtGui>
#include <QtNetwork>

#include <stdlib.h>
#include <iostream>
#include "server.h"

Server::Server(): tcpServer(0), networkSession(0)
{
	QNetworkConfigurationManager manager;
	if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
	        // Get saved network configuration
	        QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
	        settings.beginGroup(QLatin1String("QtNetwork"));
        	const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
	        settings.endGroup();
		std::cout<<"ID = "<<id.toStdString()<<std::endl;
		// If the saved network configuration is not currently discovered use the system default
		QNetworkConfiguration config = manager.configurationFromIdentifier(id);
		if ((config.state() & QNetworkConfiguration::Discovered) != QNetworkConfiguration::Discovered) {
			config = manager.defaultConfiguration();
		}

		networkSession = new QNetworkSession(config, this);
	        connect(networkSession, SIGNAL(opened()), this, SLOT(sessionOpened()));

	        networkSession->open();
	} else {
        	sessionOpened();
	}

        fortunes << tr("You've been leading a dog's life. Stay off the furniture.")
                 << tr("You've got to think about tomorrow.")
                 << tr("You will be surprised by a loud noise.")
                 << tr("You will feel hungry again in another hour.")
                 << tr("You might have mail.")
                 << tr("You cannot kill time without injuring eternity.")
                 << tr("Computers are not intelligent. They only think they are.");

        connect(tcpServer, SIGNAL(newConnection()), this, SLOT(sendFortune()));
	std::cout<<"Constructor finished!"<<std::endl;
}

void Server::sessionOpened()
{
	std::cout<<"Opening session"<<std::endl;
	// Save the used configuration
	if (networkSession) {
        	QNetworkConfiguration config = networkSession->configuration();
	        QString id;
        	if (config.type() == QNetworkConfiguration::UserChoice){
			id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
	        }else{
			id = config.identifier();
		}
	
	std::cout<<"ID = "<<id.toStdString()<<std::endl;
	QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
	}

	tcpServer = new QTcpServer(this);
	tcpServer->socketDescriptor().setPort(53097);
	if (!tcpServer->listen()) {
	        std::cout<<"Fortune Server: Unable to start the server! "<<std::endl;
	        return;
	}
	std::cout<<"TCP Server listening"<<std::endl;
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
	if (ipAddress.isEmpty()){
		ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
	}
	
	std::cout<<"Session Opened! IP = " <<ipAddress.toStdString()<<" Port = "<<tcpServer->serverPort()<<std::endl;
}

void Server::sendFortune()
{
	
	std::cout<<"Sending fortune!"<<std::endl;
	QByteArray block;
	QFile file("/home/vanz/server/file");
	if(file.open(QIODevice::ReadOnly)){
		std::cout<<"Arquivo aberto com sucesso"<<std::endl;
	}else{
		std::cout<<"Não foi possivel abrir o arquivo"<<std::endl;
	}
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_0);
	out << (quint16)0;
	out << fortunes.at(qrand() % fortunes.size());
	out.device()->seek(0);
	out << (quint16)(block.size() - sizeof(quint16));

	QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
	connect(clientConnection, SIGNAL(disconnected()),
        clientConnection, SLOT(deleteLater()));

	clientConnection->write(block);
	clientConnection->disconnectFromHost();
	
	std::cout<<"Fortune sent"<<std::endl;
}

void Server::run()
{
	std::cout<<"Server running!"<<std::endl;
	exec();
}
