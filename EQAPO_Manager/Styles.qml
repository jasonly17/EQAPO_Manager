pragma Singleton
import QtQuick 2.0
import QtQuick.Controls.Material 2.0

QtObject {
    property color primaryColor: Material.background
    property color secondaryColor: Material.accent
    property color tertiaryColor: Material.color(Material.Grey)
    property color primaryTextColor: Material.foreground
}
