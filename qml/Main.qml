import QtQuick 1.0
import "."

Rectangle {
	id: mainScreen
	width: 600
	height: 400
	color: "gray"

	Column {
		spacing: 10
		anchors.centerIn: mainScreen

		Button {
			id: send
			buttonColor: "lightgray"
			label: "Escolha um IP para enviar um arquivo"
		}

		Button {
			id: openTransfers
			buttonColor: "lightgray"
			label: "Em andamento"
		}
	}
}
