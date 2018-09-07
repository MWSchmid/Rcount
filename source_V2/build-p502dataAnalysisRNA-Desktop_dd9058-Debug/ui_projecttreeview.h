/********************************************************************************
** Form generated from reading UI file 'projecttreeview.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROJECTTREEVIEW_H
#define UI_PROJECTTREEVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_projectTreeView
{
public:
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QTreeView *treeView;
    QHBoxLayout *horizontalLayout;
    QPushButton *newPushButton;
    QPushButton *openPushButton;
    QPushButton *closePushButton;
    QPushButton *runPushButton;
    QPushButton *runAllPushButton;

    void setupUi(QWidget *projectTreeView)
    {
        if (projectTreeView->objectName().isEmpty())
            projectTreeView->setObjectName(QStringLiteral("projectTreeView"));
        projectTreeView->resize(452, 347);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(projectTreeView->sizePolicy().hasHeightForWidth());
        projectTreeView->setSizePolicy(sizePolicy);
        gridLayout_2 = new QGridLayout(projectTreeView);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        groupBox = new QGroupBox(projectTreeView);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        treeView = new QTreeView(groupBox);
        treeView->setObjectName(QStringLiteral("treeView"));

        gridLayout->addWidget(treeView, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        newPushButton = new QPushButton(groupBox);
        newPushButton->setObjectName(QStringLiteral("newPushButton"));

        horizontalLayout->addWidget(newPushButton);

        openPushButton = new QPushButton(groupBox);
        openPushButton->setObjectName(QStringLiteral("openPushButton"));

        horizontalLayout->addWidget(openPushButton);

        closePushButton = new QPushButton(groupBox);
        closePushButton->setObjectName(QStringLiteral("closePushButton"));

        horizontalLayout->addWidget(closePushButton);

        runPushButton = new QPushButton(groupBox);
        runPushButton->setObjectName(QStringLiteral("runPushButton"));

        horizontalLayout->addWidget(runPushButton);

        runAllPushButton = new QPushButton(groupBox);
        runAllPushButton->setObjectName(QStringLiteral("runAllPushButton"));

        horizontalLayout->addWidget(runAllPushButton);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);


        gridLayout_2->addWidget(groupBox, 0, 0, 1, 1);


        retranslateUi(projectTreeView);

        QMetaObject::connectSlotsByName(projectTreeView);
    } // setupUi

    void retranslateUi(QWidget *projectTreeView)
    {
        projectTreeView->setWindowTitle(QApplication::translate("projectTreeView", "projectTreeView", 0));
        groupBox->setTitle(QApplication::translate("projectTreeView", "Projects", 0));
        newPushButton->setText(QApplication::translate("projectTreeView", "new", 0));
        openPushButton->setText(QApplication::translate("projectTreeView", "open", 0));
        closePushButton->setText(QApplication::translate("projectTreeView", "close", 0));
        runPushButton->setText(QApplication::translate("projectTreeView", "run", 0));
        runAllPushButton->setText(QApplication::translate("projectTreeView", "run all", 0));
    } // retranslateUi

};

namespace Ui {
    class projectTreeView: public Ui_projectTreeView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROJECTTREEVIEW_H
