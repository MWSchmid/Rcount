#include "analysisprogressview.h"
#include "ui_analysisprogressview.h"

//#include "printTimeAndMem.h" //! MEM USAGE

analysisProgressView::analysisProgressView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::analysisProgressView)
{
    ui->setupUi(this);
    progress = 0;
    status = "idle";
    ui->label->setText(status);
    ui->progressBar->setValue(progress);
    //ui->pushButton->hide(); //! CANCEL DOES NOT WORK AT THE MOMENT - IT DOES NOW
    connect(ui->pushButton, SIGNAL(clicked()), this, SIGNAL(analysisCanceled()));
}

analysisProgressView::~analysisProgressView()
{
    delete ui;
}

QSize analysisProgressView::sizeHint() const
{
    return(ui->frame->sizeHint());
}

void analysisProgressView::updateProgress(int progressIN)
{
    progress = progressIN;
    ui->progressBar->setValue(progress);
    //print_time_and_memory(); //! MEM USAGE
}

void analysisProgressView::updateStatus(QString statusIN)
{
    status = statusIN;
    ui->label->setText(status);
}

