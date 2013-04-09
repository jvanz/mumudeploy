import QtQuick 1.0
import "."

Item {
	Rectangle {
		id: box
		width: parent.width / 2
		height: 20
		border.width: 1

		Text {
			text: FILE_PATH
			anchors.centerIn: parent
		}
	}
}
