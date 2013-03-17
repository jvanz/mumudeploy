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

	Rectangle {
		id: rectMain
		color: "gray"
		width: 400
		height: 400

		Button {
			id: send
			width: 300
			height: 30
			y: 100
			buttonColor: "lightgray"
			label: "Escolha um IP para enviar um arquivo"
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
			width: 150
			height: 30
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
			width: 400
			height: 100
			color: "lightgreen"

			Text {
				text: "Estou escondido"
				anchors.centerIn: parent
			}
		}

		Rectangle {
			id: rectIp
			y: -100
			width: 400
			height: 100
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
				width: 200
				height: 20
				anchors.top: ipLabel.top
				anchors.left: ipLabel.right
				anchors.leftMargin: 10

				TextInput {
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
				width: 200
				height: 20
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
				width: 110
				height: 50
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
