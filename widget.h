#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "moveform.h"
#include "structureform.h"
#include "assessmentform.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    moveform* m;
    structureform* s;
    AssessmentForm* a;
};
#endif // WIDGET_H
