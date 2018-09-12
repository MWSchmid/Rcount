/********************************************************************************
** Form generated from reading UI file 'createprojectwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATEPROJECTWIDGET_H
#define UI_CREATEPROJECTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_createProjectWidget
{
public:
    QGridLayout *gridLayout_12;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_13;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *filesTab;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_11;
    QGroupBox *inputFilesGroupBox;
    QGridLayout *gridLayout;
    QLabel *annotationInfileLabel;
    QLineEdit *annotationInfileLineEdit;
    QPushButton *findAnnotationInfilePushButton;
    QLabel *readsInfileLabel;
    QLineEdit *readsInfileLineEdit;
    QPushButton *findReadsInfilePushButton;
    QGroupBox *outputFilesGroupBox;
    QGridLayout *gridLayout_2;
    QLabel *annotationOutfileLabel;
    QLineEdit *annotationOutfileLineEdit;
    QPushButton *findAnnotationOutfilePushButton;
    QLabel *readsOutfileLabel;
    QLineEdit *readsOutfileLineEdit;
    QPushButton *findReadsOutfilePushButton;
    QLabel *countsOutfileLabel;
    QLineEdit *countsOutfileLineEdit;
    QPushButton *findCountsOutfilePushButton;
    QSpacerItem *verticalSpacer;
    QWidget *parametersTab;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout_10;
    QGroupBox *generalParametersGroupBox;
    QGridLayout *gridLayout_3;
    QLabel *multiLabel;
    QLabel *strandedLabel;
    QCheckBox *strandedCheckBox;
    QCheckBox *biasCheckBox;
    QSpinBox *minReadsSpinBox;
    QCheckBox *multiCheckBox;
    QLabel *biasLabel;
    QLabel *minReadsLabel;
    QLabel *antisenseLabel;
    QCheckBox *antisenseCheckBox;
    QGroupBox *biasParameterGroupBox;
    QGridLayout *gridLayout_4;
    QLabel *maxDistLabel;
    QSpinBox *maxDistSpinBox;
    QLabel *minBelowMaxDistLabel;
    QSpinBox *minBelowMaxDistSpinBox;
    QSpacerItem *verticalSpacer_2;
    QWidget *regionTab;
    QWidget *layoutWidget2;
    QGridLayout *gridLayout_9;
    QCheckBox *regionCheckBox;
    QGroupBox *subRegionGroupBox;
    QGridLayout *gridLayout_5;
    QLabel *regionStartNameLabel;
    QLineEdit *regionStartNameLineEdit;
    QLabel *regionStartLabel;
    QSpinBox *regionStartSpinBox;
    QLabel *regionEndNameLabel;
    QLineEdit *regionEndNameLineEdit;
    QLabel *regionEndLabel;
    QSpinBox *regionEndSpinBox;
    QSpacerItem *verticalSpacer_3;
    QWidget *settingsTab;
    QWidget *layoutWidget3;
    QGridLayout *gridLayout_8;
    QGroupBox *indexSettingsGroupBox;
    QGridLayout *gridLayout_7;
    QLabel *indexStepSizeLabel;
    QSpinBox *indexStepSizeSpinBox;
    QGroupBox *bufferSettingsGroupBox;
    QGridLayout *gridLayout_6;
    QLabel *bufferSizeBamLabel;
    QSpinBox *bufferSizeBamSpinBox;
    QLabel *bufferSizeMapLabel;
    QSpinBox *bufferSizeMapSpinBox;
    QLabel *bufferSizeOutLabel;
    QSpinBox *bufferSizeOutSpinBox;
    QSpacerItem *verticalSpacer_4;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QWidget *createProjectWidget)
    {
        if (createProjectWidget->objectName().isEmpty())
            createProjectWidget->setObjectName(QStringLiteral("createProjectWidget"));
        createProjectWidget->resize(424, 429);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(createProjectWidget->sizePolicy().hasHeightForWidth());
        createProjectWidget->setSizePolicy(sizePolicy);
        createProjectWidget->setMinimumSize(QSize(0, 0));
        createProjectWidget->setAutoFillBackground(false);
        gridLayout_12 = new QGridLayout(createProjectWidget);
        gridLayout_12->setSpacing(6);
        gridLayout_12->setContentsMargins(11, 11, 11, 11);
        gridLayout_12->setObjectName(QStringLiteral("gridLayout_12"));
        groupBox = new QGroupBox(createProjectWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout_13 = new QGridLayout(groupBox);
        gridLayout_13->setSpacing(6);
        gridLayout_13->setContentsMargins(11, 11, 11, 11);
        gridLayout_13->setObjectName(QStringLiteral("gridLayout_13"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tabWidget = new QTabWidget(groupBox);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy1);
        tabWidget->setMinimumSize(QSize(400, 350));
        filesTab = new QWidget();
        filesTab->setObjectName(QStringLiteral("filesTab"));
        sizePolicy1.setHeightForWidth(filesTab->sizePolicy().hasHeightForWidth());
        filesTab->setSizePolicy(sizePolicy1);
        layoutWidget = new QWidget(filesTab);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 2, 391, 311));
        gridLayout_11 = new QGridLayout(layoutWidget);
        gridLayout_11->setSpacing(6);
        gridLayout_11->setContentsMargins(11, 11, 11, 11);
        gridLayout_11->setObjectName(QStringLiteral("gridLayout_11"));
        gridLayout_11->setContentsMargins(0, 0, 0, 0);
        inputFilesGroupBox = new QGroupBox(layoutWidget);
        inputFilesGroupBox->setObjectName(QStringLiteral("inputFilesGroupBox"));
        sizePolicy1.setHeightForWidth(inputFilesGroupBox->sizePolicy().hasHeightForWidth());
        inputFilesGroupBox->setSizePolicy(sizePolicy1);
        gridLayout = new QGridLayout(inputFilesGroupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        annotationInfileLabel = new QLabel(inputFilesGroupBox);
        annotationInfileLabel->setObjectName(QStringLiteral("annotationInfileLabel"));

        gridLayout->addWidget(annotationInfileLabel, 0, 0, 1, 1);

        annotationInfileLineEdit = new QLineEdit(inputFilesGroupBox);
        annotationInfileLineEdit->setObjectName(QStringLiteral("annotationInfileLineEdit"));

        gridLayout->addWidget(annotationInfileLineEdit, 0, 1, 1, 1);

        findAnnotationInfilePushButton = new QPushButton(inputFilesGroupBox);
        findAnnotationInfilePushButton->setObjectName(QStringLiteral("findAnnotationInfilePushButton"));

        gridLayout->addWidget(findAnnotationInfilePushButton, 0, 2, 1, 1);

        readsInfileLabel = new QLabel(inputFilesGroupBox);
        readsInfileLabel->setObjectName(QStringLiteral("readsInfileLabel"));

        gridLayout->addWidget(readsInfileLabel, 1, 0, 1, 1);

        readsInfileLineEdit = new QLineEdit(inputFilesGroupBox);
        readsInfileLineEdit->setObjectName(QStringLiteral("readsInfileLineEdit"));

        gridLayout->addWidget(readsInfileLineEdit, 1, 1, 1, 1);

        findReadsInfilePushButton = new QPushButton(inputFilesGroupBox);
        findReadsInfilePushButton->setObjectName(QStringLiteral("findReadsInfilePushButton"));

        gridLayout->addWidget(findReadsInfilePushButton, 1, 2, 1, 1);


        gridLayout_11->addWidget(inputFilesGroupBox, 0, 0, 1, 1);

        outputFilesGroupBox = new QGroupBox(layoutWidget);
        outputFilesGroupBox->setObjectName(QStringLiteral("outputFilesGroupBox"));
        sizePolicy1.setHeightForWidth(outputFilesGroupBox->sizePolicy().hasHeightForWidth());
        outputFilesGroupBox->setSizePolicy(sizePolicy1);
        gridLayout_2 = new QGridLayout(outputFilesGroupBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        annotationOutfileLabel = new QLabel(outputFilesGroupBox);
        annotationOutfileLabel->setObjectName(QStringLiteral("annotationOutfileLabel"));

        gridLayout_2->addWidget(annotationOutfileLabel, 0, 0, 1, 1);

        annotationOutfileLineEdit = new QLineEdit(outputFilesGroupBox);
        annotationOutfileLineEdit->setObjectName(QStringLiteral("annotationOutfileLineEdit"));

        gridLayout_2->addWidget(annotationOutfileLineEdit, 0, 1, 1, 1);

        findAnnotationOutfilePushButton = new QPushButton(outputFilesGroupBox);
        findAnnotationOutfilePushButton->setObjectName(QStringLiteral("findAnnotationOutfilePushButton"));

        gridLayout_2->addWidget(findAnnotationOutfilePushButton, 0, 2, 1, 1);

        readsOutfileLabel = new QLabel(outputFilesGroupBox);
        readsOutfileLabel->setObjectName(QStringLiteral("readsOutfileLabel"));

        gridLayout_2->addWidget(readsOutfileLabel, 1, 0, 1, 1);

        readsOutfileLineEdit = new QLineEdit(outputFilesGroupBox);
        readsOutfileLineEdit->setObjectName(QStringLiteral("readsOutfileLineEdit"));

        gridLayout_2->addWidget(readsOutfileLineEdit, 1, 1, 1, 1);

        findReadsOutfilePushButton = new QPushButton(outputFilesGroupBox);
        findReadsOutfilePushButton->setObjectName(QStringLiteral("findReadsOutfilePushButton"));

        gridLayout_2->addWidget(findReadsOutfilePushButton, 1, 2, 1, 1);

        countsOutfileLabel = new QLabel(outputFilesGroupBox);
        countsOutfileLabel->setObjectName(QStringLiteral("countsOutfileLabel"));

        gridLayout_2->addWidget(countsOutfileLabel, 2, 0, 1, 1);

        countsOutfileLineEdit = new QLineEdit(outputFilesGroupBox);
        countsOutfileLineEdit->setObjectName(QStringLiteral("countsOutfileLineEdit"));

        gridLayout_2->addWidget(countsOutfileLineEdit, 2, 1, 1, 1);

        findCountsOutfilePushButton = new QPushButton(outputFilesGroupBox);
        findCountsOutfilePushButton->setObjectName(QStringLiteral("findCountsOutfilePushButton"));

        gridLayout_2->addWidget(findCountsOutfilePushButton, 2, 2, 1, 1);


        gridLayout_11->addWidget(outputFilesGroupBox, 1, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_11->addItem(verticalSpacer, 2, 0, 1, 1);

        tabWidget->addTab(filesTab, QString());
        parametersTab = new QWidget();
        parametersTab->setObjectName(QStringLiteral("parametersTab"));
        sizePolicy1.setHeightForWidth(parametersTab->sizePolicy().hasHeightForWidth());
        parametersTab->setSizePolicy(sizePolicy1);
        layoutWidget1 = new QWidget(parametersTab);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(0, 0, 391, 311));
        gridLayout_10 = new QGridLayout(layoutWidget1);
        gridLayout_10->setSpacing(6);
        gridLayout_10->setContentsMargins(11, 11, 11, 11);
        gridLayout_10->setObjectName(QStringLiteral("gridLayout_10"));
        gridLayout_10->setContentsMargins(0, 0, 0, 0);
        generalParametersGroupBox = new QGroupBox(layoutWidget1);
        generalParametersGroupBox->setObjectName(QStringLiteral("generalParametersGroupBox"));
        gridLayout_3 = new QGridLayout(generalParametersGroupBox);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        multiLabel = new QLabel(generalParametersGroupBox);
        multiLabel->setObjectName(QStringLiteral("multiLabel"));

        gridLayout_3->addWidget(multiLabel, 6, 0, 1, 1);

        strandedLabel = new QLabel(generalParametersGroupBox);
        strandedLabel->setObjectName(QStringLiteral("strandedLabel"));

        gridLayout_3->addWidget(strandedLabel, 2, 0, 1, 1);

        strandedCheckBox = new QCheckBox(generalParametersGroupBox);
        strandedCheckBox->setObjectName(QStringLiteral("strandedCheckBox"));

        gridLayout_3->addWidget(strandedCheckBox, 2, 1, 1, 1);

        biasCheckBox = new QCheckBox(generalParametersGroupBox);
        biasCheckBox->setObjectName(QStringLiteral("biasCheckBox"));

        gridLayout_3->addWidget(biasCheckBox, 7, 1, 1, 1);

        minReadsSpinBox = new QSpinBox(generalParametersGroupBox);
        minReadsSpinBox->setObjectName(QStringLiteral("minReadsSpinBox"));
        minReadsSpinBox->setMaximum(1000);
        minReadsSpinBox->setSingleStep(5);
        minReadsSpinBox->setValue(5);

        gridLayout_3->addWidget(minReadsSpinBox, 0, 1, 1, 1);

        multiCheckBox = new QCheckBox(generalParametersGroupBox);
        multiCheckBox->setObjectName(QStringLiteral("multiCheckBox"));

        gridLayout_3->addWidget(multiCheckBox, 6, 1, 1, 1);

        biasLabel = new QLabel(generalParametersGroupBox);
        biasLabel->setObjectName(QStringLiteral("biasLabel"));

        gridLayout_3->addWidget(biasLabel, 7, 0, 1, 1);

        minReadsLabel = new QLabel(generalParametersGroupBox);
        minReadsLabel->setObjectName(QStringLiteral("minReadsLabel"));

        gridLayout_3->addWidget(minReadsLabel, 0, 0, 1, 1);

        antisenseLabel = new QLabel(generalParametersGroupBox);
        antisenseLabel->setObjectName(QStringLiteral("antisenseLabel"));
        antisenseLabel->setEnabled(false);

        gridLayout_3->addWidget(antisenseLabel, 3, 0, 1, 1);

        antisenseCheckBox = new QCheckBox(generalParametersGroupBox);
        antisenseCheckBox->setObjectName(QStringLiteral("antisenseCheckBox"));
        antisenseCheckBox->setEnabled(false);

        gridLayout_3->addWidget(antisenseCheckBox, 3, 1, 1, 1);


        gridLayout_10->addWidget(generalParametersGroupBox, 0, 0, 1, 1);

        biasParameterGroupBox = new QGroupBox(layoutWidget1);
        biasParameterGroupBox->setObjectName(QStringLiteral("biasParameterGroupBox"));
        gridLayout_4 = new QGridLayout(biasParameterGroupBox);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        maxDistLabel = new QLabel(biasParameterGroupBox);
        maxDistLabel->setObjectName(QStringLiteral("maxDistLabel"));

        gridLayout_4->addWidget(maxDistLabel, 0, 0, 1, 1);

        maxDistSpinBox = new QSpinBox(biasParameterGroupBox);
        maxDistSpinBox->setObjectName(QStringLiteral("maxDistSpinBox"));
        maxDistSpinBox->setMaximum(9999);
        maxDistSpinBox->setSingleStep(50);
        maxDistSpinBox->setValue(250);

        gridLayout_4->addWidget(maxDistSpinBox, 0, 1, 1, 1);

        minBelowMaxDistLabel = new QLabel(biasParameterGroupBox);
        minBelowMaxDistLabel->setObjectName(QStringLiteral("minBelowMaxDistLabel"));

        gridLayout_4->addWidget(minBelowMaxDistLabel, 1, 0, 1, 1);

        minBelowMaxDistSpinBox = new QSpinBox(biasParameterGroupBox);
        minBelowMaxDistSpinBox->setObjectName(QStringLiteral("minBelowMaxDistSpinBox"));
        minBelowMaxDistSpinBox->setMaximum(1000);
        minBelowMaxDistSpinBox->setSingleStep(5);
        minBelowMaxDistSpinBox->setValue(0);

        gridLayout_4->addWidget(minBelowMaxDistSpinBox, 1, 1, 1, 1);


        gridLayout_10->addWidget(biasParameterGroupBox, 1, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_10->addItem(verticalSpacer_2, 2, 0, 1, 1);

        tabWidget->addTab(parametersTab, QString());
        regionTab = new QWidget();
        regionTab->setObjectName(QStringLiteral("regionTab"));
        sizePolicy1.setHeightForWidth(regionTab->sizePolicy().hasHeightForWidth());
        regionTab->setSizePolicy(sizePolicy1);
        layoutWidget2 = new QWidget(regionTab);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(0, 0, 391, 311));
        gridLayout_9 = new QGridLayout(layoutWidget2);
        gridLayout_9->setSpacing(6);
        gridLayout_9->setContentsMargins(11, 11, 11, 11);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        gridLayout_9->setContentsMargins(0, 0, 0, 0);
        regionCheckBox = new QCheckBox(layoutWidget2);
        regionCheckBox->setObjectName(QStringLiteral("regionCheckBox"));

        gridLayout_9->addWidget(regionCheckBox, 0, 0, 1, 1);

        subRegionGroupBox = new QGroupBox(layoutWidget2);
        subRegionGroupBox->setObjectName(QStringLiteral("subRegionGroupBox"));
        gridLayout_5 = new QGridLayout(subRegionGroupBox);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        regionStartNameLabel = new QLabel(subRegionGroupBox);
        regionStartNameLabel->setObjectName(QStringLiteral("regionStartNameLabel"));

        gridLayout_5->addWidget(regionStartNameLabel, 0, 0, 1, 1);

        regionStartNameLineEdit = new QLineEdit(subRegionGroupBox);
        regionStartNameLineEdit->setObjectName(QStringLiteral("regionStartNameLineEdit"));

        gridLayout_5->addWidget(regionStartNameLineEdit, 0, 1, 1, 1);

        regionStartLabel = new QLabel(subRegionGroupBox);
        regionStartLabel->setObjectName(QStringLiteral("regionStartLabel"));

        gridLayout_5->addWidget(regionStartLabel, 1, 0, 1, 1);

        regionStartSpinBox = new QSpinBox(subRegionGroupBox);
        regionStartSpinBox->setObjectName(QStringLiteral("regionStartSpinBox"));
        regionStartSpinBox->setMaximum(999999999);
        regionStartSpinBox->setSingleStep(10000);

        gridLayout_5->addWidget(regionStartSpinBox, 1, 1, 1, 1);

        regionEndNameLabel = new QLabel(subRegionGroupBox);
        regionEndNameLabel->setObjectName(QStringLiteral("regionEndNameLabel"));

        gridLayout_5->addWidget(regionEndNameLabel, 2, 0, 1, 1);

        regionEndNameLineEdit = new QLineEdit(subRegionGroupBox);
        regionEndNameLineEdit->setObjectName(QStringLiteral("regionEndNameLineEdit"));

        gridLayout_5->addWidget(regionEndNameLineEdit, 2, 1, 1, 1);

        regionEndLabel = new QLabel(subRegionGroupBox);
        regionEndLabel->setObjectName(QStringLiteral("regionEndLabel"));

        gridLayout_5->addWidget(regionEndLabel, 3, 0, 1, 1);

        regionEndSpinBox = new QSpinBox(subRegionGroupBox);
        regionEndSpinBox->setObjectName(QStringLiteral("regionEndSpinBox"));
        regionEndSpinBox->setMaximum(999999999);
        regionEndSpinBox->setSingleStep(10000);

        gridLayout_5->addWidget(regionEndSpinBox, 3, 1, 1, 1);


        gridLayout_9->addWidget(subRegionGroupBox, 1, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_9->addItem(verticalSpacer_3, 2, 0, 1, 1);

        tabWidget->addTab(regionTab, QString());
        settingsTab = new QWidget();
        settingsTab->setObjectName(QStringLiteral("settingsTab"));
        sizePolicy1.setHeightForWidth(settingsTab->sizePolicy().hasHeightForWidth());
        settingsTab->setSizePolicy(sizePolicy1);
        layoutWidget3 = new QWidget(settingsTab);
        layoutWidget3->setObjectName(QStringLiteral("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(0, 0, 391, 311));
        gridLayout_8 = new QGridLayout(layoutWidget3);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        gridLayout_8->setContentsMargins(0, 0, 0, 0);
        indexSettingsGroupBox = new QGroupBox(layoutWidget3);
        indexSettingsGroupBox->setObjectName(QStringLiteral("indexSettingsGroupBox"));
        gridLayout_7 = new QGridLayout(indexSettingsGroupBox);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        indexStepSizeLabel = new QLabel(indexSettingsGroupBox);
        indexStepSizeLabel->setObjectName(QStringLiteral("indexStepSizeLabel"));

        gridLayout_7->addWidget(indexStepSizeLabel, 0, 0, 1, 1);

        indexStepSizeSpinBox = new QSpinBox(indexSettingsGroupBox);
        indexStepSizeSpinBox->setObjectName(QStringLiteral("indexStepSizeSpinBox"));
        indexStepSizeSpinBox->setMinimum(1000);
        indexStepSizeSpinBox->setMaximum(1000000);
        indexStepSizeSpinBox->setSingleStep(1000);
        indexStepSizeSpinBox->setValue(10000);

        gridLayout_7->addWidget(indexStepSizeSpinBox, 0, 1, 1, 1);


        gridLayout_8->addWidget(indexSettingsGroupBox, 0, 0, 1, 1);

        bufferSettingsGroupBox = new QGroupBox(layoutWidget3);
        bufferSettingsGroupBox->setObjectName(QStringLiteral("bufferSettingsGroupBox"));
        gridLayout_6 = new QGridLayout(bufferSettingsGroupBox);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        bufferSizeBamLabel = new QLabel(bufferSettingsGroupBox);
        bufferSizeBamLabel->setObjectName(QStringLiteral("bufferSizeBamLabel"));

        gridLayout_6->addWidget(bufferSizeBamLabel, 0, 0, 1, 1);

        bufferSizeBamSpinBox = new QSpinBox(bufferSettingsGroupBox);
        bufferSizeBamSpinBox->setObjectName(QStringLiteral("bufferSizeBamSpinBox"));
        bufferSizeBamSpinBox->setMinimum(1000);
        bufferSizeBamSpinBox->setMaximum(1000000);
        bufferSizeBamSpinBox->setSingleStep(1000);
        bufferSizeBamSpinBox->setValue(200000);

        gridLayout_6->addWidget(bufferSizeBamSpinBox, 0, 1, 1, 1);

        bufferSizeMapLabel = new QLabel(bufferSettingsGroupBox);
        bufferSizeMapLabel->setObjectName(QStringLiteral("bufferSizeMapLabel"));

        gridLayout_6->addWidget(bufferSizeMapLabel, 1, 0, 1, 1);

        bufferSizeMapSpinBox = new QSpinBox(bufferSettingsGroupBox);
        bufferSizeMapSpinBox->setObjectName(QStringLiteral("bufferSizeMapSpinBox"));
        bufferSizeMapSpinBox->setMinimum(1000);
        bufferSizeMapSpinBox->setMaximum(1000000);
        bufferSizeMapSpinBox->setSingleStep(1000);
        bufferSizeMapSpinBox->setValue(200000);

        gridLayout_6->addWidget(bufferSizeMapSpinBox, 1, 1, 1, 1);

        bufferSizeOutLabel = new QLabel(bufferSettingsGroupBox);
        bufferSizeOutLabel->setObjectName(QStringLiteral("bufferSizeOutLabel"));

        gridLayout_6->addWidget(bufferSizeOutLabel, 2, 0, 1, 1);

        bufferSizeOutSpinBox = new QSpinBox(bufferSettingsGroupBox);
        bufferSizeOutSpinBox->setObjectName(QStringLiteral("bufferSizeOutSpinBox"));
        bufferSizeOutSpinBox->setMinimum(1000);
        bufferSizeOutSpinBox->setMaximum(1000000);
        bufferSizeOutSpinBox->setSingleStep(1000);
        bufferSizeOutSpinBox->setValue(200000);

        gridLayout_6->addWidget(bufferSizeOutSpinBox, 2, 1, 1, 1);


        gridLayout_8->addWidget(bufferSettingsGroupBox, 1, 0, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_8->addItem(verticalSpacer_4, 2, 0, 1, 1);

        tabWidget->addTab(settingsTab, QString());

        verticalLayout->addWidget(tabWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonBox = new QDialogButtonBox(groupBox);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        horizontalLayout->addWidget(buttonBox);


        verticalLayout->addLayout(horizontalLayout);


        gridLayout_13->addLayout(verticalLayout, 0, 0, 1, 1);


        gridLayout_12->addWidget(groupBox, 0, 0, 1, 1);

        QWidget::setTabOrder(tabWidget, annotationInfileLineEdit);
        QWidget::setTabOrder(annotationInfileLineEdit, findAnnotationInfilePushButton);
        QWidget::setTabOrder(findAnnotationInfilePushButton, readsInfileLineEdit);
        QWidget::setTabOrder(readsInfileLineEdit, findReadsInfilePushButton);
        QWidget::setTabOrder(findReadsInfilePushButton, annotationOutfileLineEdit);
        QWidget::setTabOrder(annotationOutfileLineEdit, findAnnotationOutfilePushButton);
        QWidget::setTabOrder(findAnnotationOutfilePushButton, readsOutfileLineEdit);
        QWidget::setTabOrder(readsOutfileLineEdit, findReadsOutfilePushButton);
        QWidget::setTabOrder(findReadsOutfilePushButton, countsOutfileLineEdit);
        QWidget::setTabOrder(countsOutfileLineEdit, findCountsOutfilePushButton);
        QWidget::setTabOrder(findCountsOutfilePushButton, minReadsSpinBox);
        QWidget::setTabOrder(minReadsSpinBox, strandedCheckBox);
        QWidget::setTabOrder(strandedCheckBox, antisenseCheckBox);
        QWidget::setTabOrder(antisenseCheckBox, multiCheckBox);
        QWidget::setTabOrder(multiCheckBox, biasCheckBox);
        QWidget::setTabOrder(biasCheckBox, maxDistSpinBox);
        QWidget::setTabOrder(maxDistSpinBox, minBelowMaxDistSpinBox);
        QWidget::setTabOrder(minBelowMaxDistSpinBox, regionCheckBox);
        QWidget::setTabOrder(regionCheckBox, regionStartNameLineEdit);
        QWidget::setTabOrder(regionStartNameLineEdit, regionStartSpinBox);
        QWidget::setTabOrder(regionStartSpinBox, regionEndNameLineEdit);
        QWidget::setTabOrder(regionEndNameLineEdit, regionEndSpinBox);
        QWidget::setTabOrder(regionEndSpinBox, indexStepSizeSpinBox);
        QWidget::setTabOrder(indexStepSizeSpinBox, bufferSizeBamSpinBox);
        QWidget::setTabOrder(bufferSizeBamSpinBox, bufferSizeMapSpinBox);
        QWidget::setTabOrder(bufferSizeMapSpinBox, bufferSizeOutSpinBox);
        QWidget::setTabOrder(bufferSizeOutSpinBox, buttonBox);

        retranslateUi(createProjectWidget);
        QObject::connect(regionCheckBox, SIGNAL(toggled(bool)), subRegionGroupBox, SLOT(setVisible(bool)));
        QObject::connect(biasCheckBox, SIGNAL(toggled(bool)), biasParameterGroupBox, SLOT(setVisible(bool)));
        QObject::connect(strandedCheckBox, SIGNAL(toggled(bool)), antisenseLabel, SLOT(setEnabled(bool)));
        QObject::connect(strandedCheckBox, SIGNAL(toggled(bool)), antisenseCheckBox, SLOT(setEnabled(bool)));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(createProjectWidget);
    } // setupUi

    void retranslateUi(QWidget *createProjectWidget)
    {
        createProjectWidget->setWindowTitle(QApplication::translate("createProjectWidget", "create a project", 0));
        groupBox->setTitle(QApplication::translate("createProjectWidget", "Create new project", 0));
        inputFilesGroupBox->setTitle(QApplication::translate("createProjectWidget", "Input", 0));
        annotationInfileLabel->setText(QApplication::translate("createProjectWidget", "Annotation:", 0));
        findAnnotationInfilePushButton->setText(QApplication::translate("createProjectWidget", "find", 0));
        readsInfileLabel->setText(QApplication::translate("createProjectWidget", "Alignments:", 0));
        findReadsInfilePushButton->setText(QApplication::translate("createProjectWidget", "find", 0));
        outputFilesGroupBox->setTitle(QApplication::translate("createProjectWidget", "Output", 0));
        annotationOutfileLabel->setText(QApplication::translate("createProjectWidget", "Annotation:", 0));
        findAnnotationOutfilePushButton->setText(QApplication::translate("createProjectWidget", "find", 0));
        readsOutfileLabel->setText(QApplication::translate("createProjectWidget", "Alignments:", 0));
        findReadsOutfilePushButton->setText(QApplication::translate("createProjectWidget", "find", 0));
        countsOutfileLabel->setText(QApplication::translate("createProjectWidget", "Counts:", 0));
        findCountsOutfilePushButton->setText(QApplication::translate("createProjectWidget", "find", 0));
        tabWidget->setTabText(tabWidget->indexOf(filesTab), QApplication::translate("createProjectWidget", "&Files", 0));
        generalParametersGroupBox->setTitle(QApplication::translate("createProjectWidget", "General", 0));
        multiLabel->setText(QApplication::translate("createProjectWidget", "Use multiple alignments:", 0));
        strandedLabel->setText(QApplication::translate("createProjectWidget", "Consider strand information:", 0));
        strandedCheckBox->setText(QString());
        biasCheckBox->setText(QString());
        multiCheckBox->setText(QString());
        biasLabel->setText(QApplication::translate("createProjectWidget", "Consider 3' bias:", 0));
        minReadsLabel->setText(QApplication::translate("createProjectWidget", "Minimal number of hits:", 0));
        antisenseLabel->setText(QApplication::translate("createProjectWidget", "Reads antisense to genes:", 0));
        antisenseCheckBox->setText(QString());
        biasParameterGroupBox->setTitle(QApplication::translate("createProjectWidget", "3' bias", 0));
        maxDistLabel->setText(QApplication::translate("createProjectWidget", "Maximal distance to 3' end:", 0));
        minBelowMaxDistLabel->setText(QApplication::translate("createProjectWidget", "Minimal number hits below maximal distance:", 0));
        tabWidget->setTabText(tabWidget->indexOf(parametersTab), QApplication::translate("createProjectWidget", "&Parameters", 0));
        regionCheckBox->setText(QApplication::translate("createProjectWidget", "use only a subregion", 0));
        subRegionGroupBox->setTitle(QApplication::translate("createProjectWidget", "Subregion", 0));
        regionStartNameLabel->setText(QApplication::translate("createProjectWidget", "Start with chromosome:", 0));
        regionStartLabel->setText(QApplication::translate("createProjectWidget", "at position:", 0));
        regionEndNameLabel->setText(QApplication::translate("createProjectWidget", "End with chromosome:", 0));
        regionEndLabel->setText(QApplication::translate("createProjectWidget", "at position:", 0));
        tabWidget->setTabText(tabWidget->indexOf(regionTab), QApplication::translate("createProjectWidget", "&Region", 0));
        indexSettingsGroupBox->setTitle(QApplication::translate("createProjectWidget", "Index", 0));
        indexStepSizeLabel->setText(QApplication::translate("createProjectWidget", "Size of index steps:", 0));
        bufferSettingsGroupBox->setTitle(QApplication::translate("createProjectWidget", "Buffers", 0));
        bufferSizeBamLabel->setText(QApplication::translate("createProjectWidget", "Size of buffer (reader):", 0));
        bufferSizeMapLabel->setText(QApplication::translate("createProjectWidget", "Size of buffer (mapper):", 0));
        bufferSizeOutLabel->setText(QApplication::translate("createProjectWidget", "Size of buffer (writer):", 0));
        tabWidget->setTabText(tabWidget->indexOf(settingsTab), QApplication::translate("createProjectWidget", "&Settings", 0));
    } // retranslateUi

};

namespace Ui {
    class createProjectWidget: public Ui_createProjectWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATEPROJECTWIDGET_H
