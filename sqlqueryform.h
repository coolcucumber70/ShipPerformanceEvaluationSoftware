#ifndef SQLQUERYFORM_H
#define SQLQUERYFORM_H

#include <QWidget>

namespace Ui {
class SqlqueryForm;
}

class SqlqueryForm : public QWidget
{
    Q_OBJECT

public:
    explicit SqlqueryForm(QWidget *parent = nullptr);
    ~SqlqueryForm();

private:
    Ui::SqlqueryForm *ui;
};

#endif // SQLQUERYFORM_H
