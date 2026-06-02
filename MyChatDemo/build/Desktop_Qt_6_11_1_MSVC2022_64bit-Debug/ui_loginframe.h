/********************************************************************************
** Form generated from reading UI file 'loginframe.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINFRAME_H
#define UI_LOGINFRAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginFrame
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_2;
    QLabel *lab_title;
    QSpacerItem *verticalSpacer;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *combobox_id;
    QPushButton *btn_register;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *lineedit_password;
    QPushButton *btn_change_password;
    QWidget *widget_5;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_4;
    QCheckBox *cb_remember_password;
    QSpacerItem *horizontalSpacer_5;
    QCheckBox *cb_auto_login;
    QSpacerItem *horizontalSpacer_3;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btn_login;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QMainWindow *LoginFrame)
    {
        if (LoginFrame->objectName().isEmpty())
            LoginFrame->setObjectName("LoginFrame");
        LoginFrame->resize(404, 243);
        centralwidget = new QWidget(LoginFrame);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        widget_2 = new QWidget(centralwidget);
        widget_2->setObjectName("widget_2");
        verticalLayout = new QVBoxLayout(widget_2);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalSpacer_2 = new QSpacerItem(20, 25, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout->addItem(verticalSpacer_2);

        lab_title = new QLabel(widget_2);
        lab_title->setObjectName("lab_title");
        QFont font;
        font.setPointSize(24);
        font.setBold(false);
        font.setItalic(true);
        font.setUnderline(false);
        lab_title->setFont(font);
        lab_title->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(lab_title);

        verticalSpacer = new QSpacerItem(20, 25, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        widget_3 = new QWidget(widget_2);
        widget_3->setObjectName("widget_3");
        horizontalLayout_2 = new QHBoxLayout(widget_3);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(60, 0, 40, 0);
        combobox_id = new QComboBox(widget_3);
        combobox_id->setObjectName("combobox_id");
        combobox_id->setMinimumSize(QSize(242, 30));
        combobox_id->setMaximumSize(QSize(16777215, 30));
        combobox_id->setEditable(true);
        combobox_id->setInsertPolicy(QComboBox::InsertPolicy::InsertAtTop);

        horizontalLayout_2->addWidget(combobox_id);

        btn_register = new QPushButton(widget_3);
        btn_register->setObjectName("btn_register");
        QFont font1;
        font1.setUnderline(true);
        btn_register->setFont(font1);
        btn_register->setStyleSheet(QString::fromUtf8("	QPushButton {\n"
"        background-color: transparent;\n"
"        border: none;\n"
"		color: rgb(0, 0, 0);\n"
"		font-size: 14px;\n"
"    }\n"
"      QPushButton:hover {\n"
"        color:  rgb(0, 0, 255);\n"
"    }\n"
"    QPushButton:pressed {\n"
"        padding-left: 2px;\n"
"        padding-top: 2px;\n"
"    }"));

        horizontalLayout_2->addWidget(btn_register);


        verticalLayout->addWidget(widget_3);

        widget_4 = new QWidget(widget_2);
        widget_4->setObjectName("widget_4");
        horizontalLayout_3 = new QHBoxLayout(widget_4);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(60, 0, 40, 0);
        lineedit_password = new QLineEdit(widget_4);
        lineedit_password->setObjectName("lineedit_password");
        lineedit_password->setMinimumSize(QSize(0, 30));
        lineedit_password->setMaximumSize(QSize(16777215, 30));
        lineedit_password->setEchoMode(QLineEdit::EchoMode::Password);
        lineedit_password->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_3->addWidget(lineedit_password);

        btn_change_password = new QPushButton(widget_4);
        btn_change_password->setObjectName("btn_change_password");
        btn_change_password->setFont(font1);
        btn_change_password->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        btn_change_password->setContextMenuPolicy(Qt::ContextMenuPolicy::DefaultContextMenu);
        btn_change_password->setStyleSheet(QString::fromUtf8("	QPushButton {\n"
"        background-color: transparent;\n"
"        border: none;\n"
"		color: rgb(0, 0, 0);\n"
"		font-size: 14px;\n"
"    }\n"
"      QPushButton:hover {\n"
"        color:  rgb(0, 0, 255);\n"
"    }\n"
"    QPushButton:pressed {\n"
"        padding-left: 2px;\n"
"        padding-top: 2px;\n"
"    }"));
        btn_change_password->setFlat(false);

        horizontalLayout_3->addWidget(btn_change_password);


        verticalLayout->addWidget(widget_4);

        widget_5 = new QWidget(widget_2);
        widget_5->setObjectName("widget_5");
        horizontalLayout_4 = new QHBoxLayout(widget_5);
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalLayout_4->setContentsMargins(30, 0, 30, 0);
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);

        cb_remember_password = new QCheckBox(widget_5);
        cb_remember_password->setObjectName("cb_remember_password");

        horizontalLayout_4->addWidget(cb_remember_password);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);

        cb_auto_login = new QCheckBox(widget_5);
        cb_auto_login->setObjectName("cb_auto_login");

        horizontalLayout_4->addWidget(cb_auto_login);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);


        verticalLayout->addWidget(widget_5);

        widget = new QWidget(widget_2);
        widget->setObjectName("widget");
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(30, 0, 30, 0);
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        btn_login = new QPushButton(widget);
        btn_login->setObjectName("btn_login");
        btn_login->setMinimumSize(QSize(220, 0));
        btn_login->setMaximumSize(QSize(16777215, 30));
        btn_login->setStyleSheet(QString::fromUtf8("    QPushButton {\n"
"        background-color: #3498db;\n"
"        color: white;\n"
"        border: none;\n"
"        border-radius: 5px;\n"
"        padding: 8px 16px;\n"
"        font-size: 14px;\n"
"        font-weight: bold;\n"
"   }\n"
"    QPushButton:hover {\n"
"        background-color: #2980b9;\n"
"    }\n"
"    QPushButton:pressed {\n"
"        background-color: #1f618d;\n"
"        padding-left: 10px;\n"
"        padding-top: 10px;\n"
"    }"));

        horizontalLayout->addWidget(btn_login);

        horizontalSpacer = new QSpacerItem(69, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addWidget(widget);

        verticalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout->addItem(verticalSpacer_3);


        verticalLayout_2->addWidget(widget_2);

        LoginFrame->setCentralWidget(centralwidget);

        retranslateUi(LoginFrame);

        btn_change_password->setDefault(false);


        QMetaObject::connectSlotsByName(LoginFrame);
    } // setupUi

    void retranslateUi(QMainWindow *LoginFrame)
    {
        LoginFrame->setWindowTitle(QCoreApplication::translate("LoginFrame", "MainWindow", nullptr));
        lab_title->setText(QCoreApplication::translate("LoginFrame", "\347\231\273\345\275\225", nullptr));
        combobox_id->setPlaceholderText(QCoreApplication::translate("LoginFrame", "\350\276\223\345\205\245\350\264\246\345\217\267", nullptr));
        btn_register->setText(QCoreApplication::translate("LoginFrame", "\346\263\250\345\206\214\350\264\246\345\217\267", nullptr));
        lineedit_password->setPlaceholderText(QCoreApplication::translate("LoginFrame", "\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        btn_change_password->setText(QCoreApplication::translate("LoginFrame", "\346\211\276\345\233\236\345\257\206\347\240\201", nullptr));
        cb_remember_password->setText(QCoreApplication::translate("LoginFrame", "\350\256\260\344\275\217\345\257\206\347\240\201", nullptr));
        cb_auto_login->setText(QCoreApplication::translate("LoginFrame", "\350\207\252\345\212\250\347\231\273\345\275\225", nullptr));
        btn_login->setText(QCoreApplication::translate("LoginFrame", "\347\231\273\345\275\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginFrame: public Ui_LoginFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINFRAME_H
