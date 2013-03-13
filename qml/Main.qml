import QtQuick 1.0
import "."

Item {
	id: mainScreen
	width: 400
	height: 400

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

			TextEdit {
				width: 100
				height: 30
				text: "Mokiado Rulez"
				anchors.centerIn : parent
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
