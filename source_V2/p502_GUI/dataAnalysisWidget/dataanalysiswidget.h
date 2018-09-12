#ifndef DATAANALYSISWIDGET_H
#define DATAANALYSISWIDGET_H

#include <QMap>
#include <QWidget>
#include <QStringListModel>

#include "../createProjectWidget/createprojectwidget.h"

class settingsTreeModel;
class resultsTableModel;

namespace Ui {
    class dataAnalysisWidget;
}

class dataAnalysisWidget : public QWidget
{
    Q_OBJECT

public:
    explicit dataAnalysisWidget(QWidget *parent = 0);
    ~dataAnalysisWidget();

private:
    Ui::dataAnalysisWidget *ui;
    createProjectWidget *_createProjectWidget;
    QStringListModel *_projectListModel;
    QMap<QString, settingsTreeModel*> _projectToSettings;
    QMap<QString, resultsTableModel*> _projectToResults;

private slots:
    void createProject();
    void loadProject();
    void closeProject();
    void readProjectFile(QString fileName);
    void readCountTable(QString fileName);
    void updateOtherViews(QModelIndex);
};

#endif // DATAANALYSISWIDGET_H
