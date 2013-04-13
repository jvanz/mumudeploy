import QtQuick 1.0
import "."

Item {
	Rectangle {
		id: path
		width: parent.width / 3 - 5
		height: 20
		anchors.left: parent.left

		Text {
			text: FILE_PATH
			anchors.centerIn: parent
		}
	}

	Rectangle {
		id: fragments 
		width: parent.width / 3 - 5
		height: 20
		anchors.left: path.right
		anchors.leftMargin: 5

		Text {
			text: SENT_PIECES + "/" + TOTAL_PIECES
			anchors.centerIn: parent
		}
	}

	Rectangle {
		id: direction
		width: parent.width / 3 - 5
		height: 20
		anchors.left: fragments.right
		anchors.leftMargin: 5

		Text {
			text: (DIRECTION == "R") ? qsTr("Recebendo") : qsTr("Enviando")
			anchors.centerIn: parent
		}
	}
}
