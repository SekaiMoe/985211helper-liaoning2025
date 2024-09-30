import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Window {
    width: 400
    height: 500
    visible: true
    title: qsTr("大学专业搜索")

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Label {
            text: "请输入大学名称:"
        }

        TextField {
            id: universityInput
            Layout.fillWidth: true
            onTextChanged: backend.updateProfessions(text)
        }

        Label {
            text: "请选择专业:"
        }

        ComboBox {
            id: professionCombo
            Layout.fillWidth: true
            model: []
        }

        Button {
            text: "搜索"
            Layout.fillWidth: true
            onClicked: backend.performSearch(universityInput.text, professionCombo.currentText)
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            TextArea {
                id: resultText
                readOnly: true
                wrapMode: TextEdit.Wrap
            }
        }
    }

    Connections {
        target: backend
        function onProfessionsUpdated(professions) {
            professionCombo.model = professions
        }
        function onSearchCompleted(result) {
            resultText.text = result
        }
    }
}
