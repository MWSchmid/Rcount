/********************************************************************************
** Form generated from reading UI file 'analysisprogressview.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANALYSISPROGRESSVIEW_H
#define UI_ANALYSISPROGRESSVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_analysisProgressView
{
public:
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QProgressBar *progressBar;
    QPushButton *pushButton;

    void setupUi(QWidget *analysisProgressView)
    {
        if (analysisProgressView->objectName().isEmpty())
            analysisProgressView->setObjectName(QStringLiteral("analysisProgressView"));
        analysisProgressView->resize(442, 73);
        gridLayout_2 = new QGridLayout(analysisProgressView);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        groupBox = new QGroupBox(analysisProgressView);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        progressBar = new QProgressBar(groupBox);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(0);

        horizontalLayout->addWidget(progressBar);

        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        gridLayout_2->addWidget(groupBox, 0, 0, 1, 1);


        retranslateUi(analysisProgressView);

        QMetaObject::connectSlotsByName(analysisProgressView);
    } // setupUi

    void retranslateUi(QWidget *analysisProgressView)
    {
        analysisProgressView->setWindowTitle(QApplication::translate("analysisProgressView", "analysisProgressView", 0));
        groupBox->setTitle(QApplication::translate("analysisProgressView", "Data analysis is running", 0));
        label->setText(QApplication::translate("analysisProgressView", "CurrentStatusDisplayedHerePlaceHolder", 0));
        pushButton->setText(QApplication::translate("analysisProgressView", "cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class analysisProgressView: public Ui_analysisProgressView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANALYSISPROGRESSVIEW_H
