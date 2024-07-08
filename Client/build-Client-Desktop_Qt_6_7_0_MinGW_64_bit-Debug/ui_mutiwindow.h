/********************************************************************************
** Form generated from reading UI file 'mutiwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MUTIWINDOW_H
#define UI_MUTIWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MutiWindow
{
public:

    void setupUi(QWidget *MutiWindow)
    {
        if (MutiWindow->objectName().isEmpty())
            MutiWindow->setObjectName("MutiWindow");
        MutiWindow->resize(653, 462);

        retranslateUi(MutiWindow);

        QMetaObject::connectSlotsByName(MutiWindow);
    } // setupUi

    void retranslateUi(QWidget *MutiWindow)
    {
        MutiWindow->setWindowTitle(QCoreApplication::translate("MutiWindow", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MutiWindow: public Ui_MutiWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MUTIWINDOW_H
