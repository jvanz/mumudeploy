import QtQuick 1.0
import "."
import mumu 1.0

Item {
	id: mainScreen
	width: 400
	height: 400

	FileHandle {
		id: fileControler
	}

	DatabaseManager {
		id: dbManager
	}

	Rectangle {
		id: rectMain
		color: "gray"
		width: parent.width
		height: parent.height

		Button {
			id: send
			width: parent.width - 100
			height: parent.height - 370
			y: parent.height - 300
			buttonColor: "lightgray"
			label: qsTr("Escolha um IP para enviar um arquivo")
			anchors.horizontalCenter: parent.horizontalCenter

			MouseArea {
				anchors.fill: parent
				onClicked: {
					rectMain.state = (rectMain.state == "showSendTo") ? "showNothing" : "showSendTo"
				}
			}
		}

		Button {
			id: openTransfers
			width: parent.width - 250
			height: parent.height - 370
			y: 250
			buttonColor: "lightgray"
			label: "Em andamento"
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
			id: rectHidden
			y: 500
			width: parent.width
			height: parent.height - 300
			color: "lightgreen"

			Timer {
				// default is 1 second
				repeat: true
				running: true

				onTriggered: grid.model.refresh()
			}

			GridView {
				id: grid
				anchors.fill: parent
				model: dbManager.retornaPendetesModel()
				flow: GridView.TopToBottom
				cacheBuffer: 0
				delegate: FilesToSend { id: toSend }
				keyNavigationWraps: true
				cellWidth: 250
				cellHeight: 20
			}
		}

		Rectangle {
			id: rectIp
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
				width: parent.width - 200
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
				width: parent.width - 200
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
				width: parent.width - 290
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
							if (dbManager.insertNewProcess(textIP.text, textFile.text))
								rectMain.state = "showRunning"
							else
								rectFile.color = "red"
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
					target: openTransfers
					y: 200
				}

				PropertyChanges {
					target: rectHidden
					y: 300
				}
			},
			State {
				name: "showSendTo"
				PropertyChanges {
					target: send
					y: 150
				}
				PropertyChanges {
					target: rectIp
					y: 0
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
				target: openTransfers
				properties: "y"
				easing.type: Easing.OutExpo
				duration: 1000
			}
			NumberAnimation {
				target: rectHidden
				properties: "y"
				easing.type: Easing.OutExpo
				duration: 1000
			}
			NumberAnimation {
				target: rectIp
				properties: "y"
				easing.type: Easing.OutExpo
				duration: 1000
			}
			NumberAnimation {
				target: send
				properties: "y"
				easing.type: Easing.OutExpo
				duration: 1000
			}
		}
	}
}
