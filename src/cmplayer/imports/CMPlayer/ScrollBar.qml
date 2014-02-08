import QtQuick 2.0

Item { id: item;
	property Flickable scrollable
	property alias color: handle.color
	property alias border: handle.border
	property alias radius: handle.radius
	property alias minimumLength: handle.hMin
	property real gap: 5
	opacity: 0

	Item {
		anchors { fill: parent; margins: gap }
		Rectangle {
			id: handle
			color: Qt.rgba(0.0, 0.0, 0.0, 0.5)
			readonly property real rh: item.scrollable.visibleArea.heightRatio
			readonly property real ry: item.scrollable.visibleArea.yPosition
			readonly property real target: (ry + rh*0.5)*parent.height
			property real hMin: 35
			readonly property real half: Math.max(rh*parent.height, hMin)*0.5
			x: 0; y: Math.min(Math.max(0, target - half), parent.height)
			width: parent.width; height: half + size()
			function size() {
				if (target < half)
					return target
				var dt = parent.height - target;
				return (dt < half) ? dt : half;
			}
		}
	}

	MouseArea {
		id: mouseArea
		anchors.fill: parent
		property bool moving: false
		function move(y) {
			scrollable.contentY = (scrollable.contentHeight - scrollable.height)*Math.max(0, Math.min(1, (y-gap)/(height-2*gap)))
		}
		onPressed: move(mouse.y)
		onPositionChanged: if (pressed) move(mouse.y)
		hoverEnabled: true
		Timer {
			id: timer
			running: false
			repeat: false
			interval: 500
			onTriggered: mouseArea.moving = false
		}
		preventStealing: true
		Connections {
			target: scrollable
			onContentYChanged: {
				mouseArea.moving = true
				timer.restart()
			}
			onMovingVerticallyChanged: {
				if (scrollable.movingVertically) {
					mouseArea.moving = true
					timer.restart()
				}
			}
		}
	}

	states: State {
		name: "visible"
		when: scrollable.movingVertically || mouseArea.moving || mouseArea.containsMouse
		PropertyChanges { target: item; opacity: 1.0 }
	}

	transitions: Transition {
		from: "visible"; to: ""
		NumberAnimation { properties: "opacity"; duration: 600 }
	}
}


