/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menu_xml_version_1_0_encoding_UTF_8_ui_version_4_0_class_MainWindow_class_widget_class_QMainWindow_name_MainWindow_property_name_geometry_rect_x_0_x_y_0_y_width_800_width_height_600_height_rect_property_property_name_windowTitle_string_MainWindow_string_property_widget_class_QWidget_name_centralwidget_widget_class_QMenuBar_name_menubar_widget_class_QStatusBar_name_statusbar_widget_ui;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 24));
        menu_xml_version_1_0_encoding_UTF_8_ui_version_4_0_class_MainWindow_class_widget_class_QMainWindow_name_MainWindow_property_name_geometry_rect_x_0_x_y_0_y_width_800_width_height_600_height_rect_property_property_name_windowTitle_string_MainWindow_string_property_widget_class_QWidget_name_centralwidget_widget_class_QMenuBar_name_menubar_widget_class_QStatusBar_name_statusbar_widget_ui = new QMenu(menubar);
        menu_xml_version_1_0_encoding_UTF_8_ui_version_4_0_class_MainWindow_class_widget_class_QMainWindow_name_MainWindow_property_name_geometry_rect_x_0_x_y_0_y_width_800_width_height_600_height_rect_property_property_name_windowTitle_string_MainWindow_string_property_widget_class_QWidget_name_centralwidget_widget_class_QMenuBar_name_menubar_widget_class_QStatusBar_name_statusbar_widget_ui->setObjectName("menu_xml_version_1_0_encoding_UTF_8_ui_version_4_0_class_MainWindow_class_widget_class_QMainWindow_name_MainWindow_property_name_geometry_rect_x_0_x_y_0_y_width_800_width_height_600_height_rect_property_property_name_windowTitle_string_MainWindow_string_property_widget_class_QWidget_name_centralwidget_widget_class_QMenuBar_name_menubar_widget_class_QStatusBar_name_statusbar_widget_ui");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menu_xml_version_1_0_encoding_UTF_8_ui_version_4_0_class_MainWindow_class_widget_class_QMainWindow_name_MainWindow_property_name_geometry_rect_x_0_x_y_0_y_width_800_width_height_600_height_rect_property_property_name_windowTitle_string_MainWindow_string_property_widget_class_QWidget_name_centralwidget_widget_class_QMenuBar_name_menubar_widget_class_QStatusBar_name_statusbar_widget_ui->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        menu_xml_version_1_0_encoding_UTF_8_ui_version_4_0_class_MainWindow_class_widget_class_QMainWindow_name_MainWindow_property_name_geometry_rect_x_0_x_y_0_y_width_800_width_height_600_height_rect_property_property_name_windowTitle_string_MainWindow_string_property_widget_class_QWidget_name_centralwidget_widget_class_QMenuBar_name_menubar_widget_class_QStatusBar_name_statusbar_widget_ui->setTitle(QCoreApplication::translate("MainWindow", "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<ui version=\"4.0\">\n"
" <class>MainWindow</class>\n"
" <widget class=\"QMainWindow\" name=\"MainWindow\">\n"
"  <property name=\"geometry\">\n"
"   <rect>\n"
"    <x>0</x>\n"
"    <y>0</y>\n"
"    <width>800</width>\n"
"    <height>600</height>\n"
"   </rect>\n"
"  </property>\n"
"  <property name=\"windowTitle\">\n"
"   <string>MainWindow</string>\n"
"  </property>\n"
"  <widget class=\"QWidget\" name=\"centralwidget\"/>\n"
"  <widget class=\"QMenuBar\" name=\"menubar\"/>\n"
"  <widget class=\"QStatusBar\" name=\"statusbar\"/>\n"
" </widget>\n"
"</ui>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
