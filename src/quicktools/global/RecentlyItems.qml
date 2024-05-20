pragma Singleton

import QtQuick
import QuickTools.ui
import "../component"

QuickObject {
    property var pages: [
        ImageProcessItems,
    ]
    function getRecentlyAddedData() {
        var data = []
        if (pages) {
            for (var i = 0; i < pages.length; ++i) {
                var _page = pages[i]
                for (var j = 0; j < _page.children.length; ++j) {
                    var item = _page.children[j]
                    if (item instanceof ToolItem && item.extra && item.extra.recentlyAdded) {
                        data.push(item)
                    }
                }
            }
        }
        return data
    }
    function getRecentlyUpdatedData() {
        var data = []
        if (pages) {
            for (var i = 0; i < pages.length; ++i) {
                var _page = pages[i]
                for (var j = 0; j < _page.children.length; ++j) {
                    var item = _page.children[j]
                    if (item instanceof ToolItem && item.extra && item.extra.recentlyUpdated) {
                        data.push(item)
                    }
                }
            }
        }
        return data
    }
}
