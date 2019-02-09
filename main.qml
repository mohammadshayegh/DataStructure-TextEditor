import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.1

Window {
    id : root
    visible: true
    width: 440
    height: 430
    color : "#48899d"

    property string val
    Connections{
        target: connection
    }


    Item {
        id : inputs
        width : 400
        height: 50
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.topMargin: 60

        TextField{
            id : txtInput
            width : txtShow.width
            height : 50
        }

        TextArea{
            id : txtShow
            width : 400
            height : 250
            anchors.top: txtInput.bottom
            font.pixelSize: 20
            anchors.topMargin: 10
            text: connection.textSifter(txtInput.text)
        }
    }


//    ListView{
//        anchors.fill: parent
//        anchors.leftMargin: 3*parent.width / 4
//        height : 100
//        clip : true
//        model : 5
//        spacing: 1
//        delegate: stackElements
//    }

//    Component{
//        id:stackElements
//        Rectangle{
//            width : 60
//            height : 40
//            color : "white"
//            border.color: "black"
//            border.width: 4

//        }
//    }


}


