#ifndef STRUCTUREDIALOG_H
#define STRUCTUREDIALOG_H

#include <QDialog>

namespace Ui {
class StructureDialog;
}

class StructureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StructureDialog(QWidget *parent = nullptr);
    ~StructureDialog();

private:
    Ui::StructureDialog *ui;
};

#endif // STRUCTUREDIALOG_H
