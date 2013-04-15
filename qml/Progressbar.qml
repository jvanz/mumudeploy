import QtQuick 1.0

Item {
	id: progressbar

	property int minimum: 0
	property int maximum: 100
	property int value: 0

	Rectangle {
		id: background
		anchors.fill: parent
	}

	Rectangle {
		id: color
		color: "orange"
		anchors { left: parent.left; top: parent.top; bottom: parent.bottom; margins: 3 }
		width: ((progressbar.width *  (value - minimum)) / (maximum - minimum) - 6)
		smooth: true
	}
}
