#include <QWidget>
#include <QLayout>

#include "clearlayout.h"

void clearLayout(QLayout* layout) {
    while (QLayoutItem* item = layout->takeAt(0)) {

        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        }

        if (QLayout* childLayout = item->layout()) {
            clearLayout(childLayout);
        }

        delete item;
    }
}
