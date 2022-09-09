#include "sqlqueryform.h"
#include "ui_sqlqueryform.h"

SqlqueryForm::SqlqueryForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SqlqueryForm)
{
    ui->setupUi(this);
}

SqlqueryForm::~SqlqueryForm()
{
    delete ui;
}
