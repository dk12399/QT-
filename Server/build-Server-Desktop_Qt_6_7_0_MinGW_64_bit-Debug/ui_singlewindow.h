/********************************************************************************
** Form generated from reading UI file 'singlewindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SINGLEWINDOW_H
#define UI_SINGLEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SingleWindow
{
public:

    void setupUi(QWidget *SingleWindow)
    {
        if (SingleWindow->objectName().isEmpty())
            SingleWindow->setObjectName("SingleWindow");
        SingleWindow->resize(400, 300);

        retranslateUi(SingleWindow);

        QMetaObject::connectSlotsByName(SingleWindow);
    } // setupUi

    void retranslateUi(QWidget *SingleWindow)
    {
        SingleWindow->setWindowTitle(QCoreApplication::translate("SingleWindow", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SingleWindow: public Ui_SingleWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SINGLEWINDOW_H
