#include "structuredialog.h"
#include "ui_structuredialog.h"

StructureDialog::StructureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StructureDialog)
{
    ui->setupUi(this);
}

StructureDialog::~StructureDialog()
{
    delete ui;
}
