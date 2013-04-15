import QtQuick 1.0
import "."

Item {

	Rectangle {
		id: ip
		width: parent.width / 4
		height: 20
		anchors.left: parent.left

		Text {
			text: IP
			anchors.centerIn: parent
		}
	}

	Rectangle {
		id: path
		width: parent.width / 4
		height: 20
		anchors.left: ip.right
		anchors.leftMargin: 3

		Text {
			text: FILE_PATH
			anchors.centerIn: parent
		}
	}

	Rectangle {
		id: fragments 
		width: parent.width / 4
		height: 20
		anchors.left: path.right
		anchors.leftMargin: 3

		Progressbar {
			anchors.fill: parent
			value: SENT_PIECES
			maximum: TOTAL_PIECES
		}

		Text {
			text: SENT_PIECES + "/" + TOTAL_PIECES
			anchors.centerIn: parent
		}
	}

	Rectangle {
		id: direction
		width: parent.width / 4
		height: 20
		anchors.left: fragments.right
		anchors.leftMargin: 3

		Text {
			text: (DIRECTION == "R") ? qsTr("Recebendo") : qsTr("Enviando")
			anchors.centerIn: parent
		}
	}
}
