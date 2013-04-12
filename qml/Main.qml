import QtQuick 1.0
import "."
import mumu 1.0

Item {
	id: mainScreen
	width: 500
	height: 400

	FileHandle {
		id: fileControler
	}

	DatabaseManager {
		id: dbManager
	}

	MumuFriend {
		id: friend
	}

	Rectangle {
		id: rectMain
		color: "gray"
		width: parent.width
		height: parent.height

		Button {
			id: btOptionSend
			width: parent.width - 100
			height: parent.height - 370
			y: parent.height - 320
			buttonColor: "lightgray"
			label: qsTr("Escolha um IP para enviar um arquivo")
			anchors.horizontalCenter: parent.horizontalCenter

			MouseArea {
				anchors.fill: parent
				onClicked: {
					if (rectFile.color == "#ffffff") { // white
						rectFile.color = "white";
						textFile.text = ""
					}
					rectMain.state = (rectMain.state == "showSendTo") ? "showNothing" : "showSendTo"
				}
			}
		}

		Button {
			id: btOptionConfig
			width: parent.width - 280
			height: parent.height - 370
			y: parent.height - 220
			buttonColor: "lightgray"
			label: qsTr("Configurações")
			x: parent.width / 2 - btOptionConfig.width / 2

			MouseArea {
				anchors.fill: parent
				onClicked: {
					if (rectFieldDir.color == "#ffffff") { // white
						rectFieldDir.color = "white";
						editFieldDir.text = dbManager.getDestinationDir()
					}
					editFieldServer.text = dbManager.getServerAddress()
					rectMain.state = (rectMain.state == "showConfig") ? "showNothing" : "showConfig"
				}
			}
		}

		Button {
			id: btOptionTranfers
			width: parent.width - 250
			height: parent.height - 370
			y: parent.height - 100
			buttonColor: "lightgray"
			label: qsTr("Em andamento")
			anchors.horizontalCenter: parent.horizontalCenter
			smooth: true

			MouseArea {
				anchors.fill: parent
				onClicked: {
					rectMain.state = (rectMain.state == "showRunning") ? "showNothing" : "showRunning"
				}
			}
		}

		Rectangle {
			id: panelConfig
			width: parent.width
			height: parent.height - 300
			color: "palegoldenrod"
			x: parent.width + 10
			y: parent.height - 230

			Text {
				id: txtDestDir
				text: qsTr("Pasta destino: ")
				anchors.left: parent.letf
				anchors.leftMargin: 150
				anchors.top: parent.top
				anchors.topMargin: 15
			}

			Rectangle {
				id: rectFieldDir
				width: parent.width - 180
				height: parent.height - 70
				anchors.left: txtDestDir.right
				anchors.leftMargin: 5
				anchors.top: parent.top
				anchors.topMargin: 11

				TextEdit {
					id: editFieldDir
					width: parent.width
					height: parent.height - 10
					anchors.left: parent.left
					anchors.leftMargin: 2
					anchors.verticalCenter: parent.verticalCenter
				}

				Button {
					id: btSaveDir
					width: parent.width - 250
					height: parent.height - 5
					anchors.left: editFieldDir.right
					anchors.leftMargin: 2
					anchors.verticalCenter: parent.verticalCenter
					buttonColor: "lightcyan"
					label: qsTr("Salvar")

					MouseArea {
						anchors.fill: parent
						onClicked: {
							if (fileControler.verifyDir(editFieldDir.text)) {
								dbManager.updateDestDir(editFieldDir.text)
								rectMain.state = "showNothing"
								rectFieldDir.color = "white"
							} else {
								rectFieldDir.color = "red"
							}
						}
					}
				}
			}

			Text {
				id: txtServerAddr
				text: qsTr("Endereço Server: ")
				anchors.left: parent.letf
				anchors.leftMargin: 150
				anchors.top: txtDestDir.top
				anchors.topMargin: 42
			}

			Rectangle {
				id: rectServerAddres
				width: parent.width - 190
				height: parent.height - 70
				anchors.left: txtServerAddr.right
				anchors.leftMargin: 5
				anchors.top: rectFieldDir.top
				anchors.topMargin: 40

				TextEdit {
					id: editFieldServer
					width: parent.width
					height: parent.height - 10
					anchors.left: parent.left
					anchors.leftMargin: 2
					anchors.verticalCenter: parent.verticalCenter
				}

				Button {
					id: btSaveServer
					width: parent.width - 250
					height: parent.height - 5
					anchors.left: editFieldServer.right
					anchors.leftMargin: 2
					anchors.verticalCenter: parent.verticalCenter
					buttonColor: "lightcyan"
					label: qsTr("Salvar")

					MouseArea {
						anchors.fill: parent
						onClicked: {
								dbManager.updateServerAddress(editFieldServer.text)
								rectMain.state = "showNothing"
							}
					}
				}
			}
		}

		Rectangle {
			id: panelTranfers
			y: 500
			width: parent.width
			height: parent.height - 300
			color: "lightgreen"

			Timer {
				repeat: true
				running: true
				interval: 1000

				onTriggered: { grid.model.refresh() }
			}

			GridView {
				id: grid
				anchors.fill: parent
				model: dbManager.retornaPendetesModel()
				flow: GridView.TopToBottom
				cacheBuffer: 0
				delegate: FilesToSend {
						id: toSend
						width: panelTranfers.width
					}
				keyNavigationWraps: true
				cellWidth: 250
				cellHeight: 20
			}
		}

		Rectangle {
			id: panelSend
			y: -100
			width: parent.width
			height: parent.height - 300
			color: "lightblue"

			Text {
				id: ipLabel
				text: "IP: "
				anchors.top: parent.top
				anchors.topMargin: 30
				anchors.right: fileLabel.right
			}

			Rectangle {
				id: rectIP
				width: parent.width / 2
				height: parent.height - 80
				anchors.top: ipLabel.top
				anchors.left: ipLabel.right
				anchors.leftMargin: 10

				TextInput {
					id: textIP
					anchors.fill: parent
					anchors.left : parent.left
					anchors.leftMargin: 2
					inputMask: "999.999.999.999"
					text: "127.000.000.001"
				}
			}

			Text {
				id: fileLabel
				text: qsTr("Arquivo: ")
				anchors.top: ipLabel.bottom
				anchors.topMargin: 5
				anchors.left: parent.left
				anchors.leftMargin: 6
			}

			Rectangle {
				id: rectFile
				width: parent.width / 2
				height: parent.height - 80
				anchors.top: fileLabel.top
				anchors.left: rectIP.left

				TextInput {
					id: textFile
					anchors.fill: parent
					anchors.left: parent.left
					anchors.leftMargin: 2
				}
			}

			Button {
				id: btSend
				width: parent.width - 350
				height: parent.height - 50
				anchors.left: rectFile.right
				anchors.leftMargin: 10
				anchors.verticalCenter: parent.verticalCenter
				buttonColor: "lightcyan"
				label: qsTr("Enviar arquivo")

				MouseArea {
					anchors.fill: parent
					onClicked: {
						if (fileControler.verifyFile(textFile.text)) {
							rectFile.color = "lightgreen"
							if (dbManager.insertNewProcess(textIP.text, textFile.text)) {
								friend.sendFile(textFile.text)
								rectMain.state = "showRunning"
								rectFile.color = "white"
							} else {
								rectFile.color = "red"
							}
						} else {
							rectFile.color = "red"
						}
					}
				}
			}
		}

		states: [
			State {
				name: "showRunning"
				PropertyChanges {
					target: btOptionTranfers
					y: 250
				}

				PropertyChanges {
					target: panelTranfers
					y: 300
				}
			},
			State {
				name: "showSendTo"
				PropertyChanges {
					target: btOptionSend
					y: 120
				}
				PropertyChanges {
					target: panelSend
					y: 0
				}
			},
			State {
				name: "showConfig"
				PropertyChanges {
					target: btOptionConfig
					x: 3
					y: 130
				}

				PropertyChanges {
					target: panelConfig
					x: 0
				}
			}
		]

		transitions: Transition {
			NumberAnimation {
				target: rectMain
				properties: "height"
				easing.type: Easing.OutExpo
				duration: 1000
			}
			NumberAnimation {
				target: btOptionTranfers
				properties: "y"
				easing.type: Easing.OutExpo
				duration: 1000
			}
			NumberAnimation {
				target: panelTranfers
				properties: "y"
				easing.type: Easing.OutExpo
				duration: 1000
			}
			NumberAnimation {
				target: panelSend
				properties: "y"
				easing.type: Easing.OutExpo
				duration: 1000
			}
			NumberAnimation {
				target: btOptionSend
				properties: "y"
				easing.type: Easing.OutExpo
				duration: 1000
			}
			NumberAnimation {
				target: btOptionConfig
				properties: "x"
				easing.type: Easing.OutExpo
				duration: 1000
			}
			NumberAnimation {
				target: btOptionConfig
				properties: "y"
				easing.type: Easing.OutExpo
				duration: 1000
			}
			NumberAnimation {
				target: panelConfig
				properties: "x"
				easing.type: Easing.OutExpo
				duration: 1000
			}
			NumberAnimation {
				target: panelConfig
				properties: "y"
				easing.type: Easing.OutExpo
				duration: 1000
			}
		}
	}
}
