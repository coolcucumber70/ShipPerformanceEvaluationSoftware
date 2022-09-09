#pragma once

#include <QChartView>
#include <QMouseEvent>
#include <QGraphicsSimpleTextItem>
#include <QtCharts/QSplineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QTimer>
#include <QDateTime>
QT_CHARTS_USE_NAMESPACE

class ChartView : public QChartView
{
	Q_OBJECT

public:
    ChartView(QWidget *parent = nullptr);
	~ChartView();
    const int maxsize=10;
    void saveAxisRange();// 保存坐标区域，用于复位
    void updateseries(QVector<float>&data);
    void updateseries(QVector<QPointF> &data);
    QVector<QPointF> buffer;
protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
    void leaveEvent(QEvent *);


private:
	QPoint m_lastPoint;
	bool m_isPress;
	bool m_ctrlPress;
	bool m_alreadySaveRange;
	double m_xMin, m_xMax, m_yMin, m_yMax;
    QGraphicsSimpleTextItem* m_coordItem;
    QValueAxis *axisX;
    QValueAxis *axisY;
    QChart *Chart;//画线（这里可以考虑使用函数数组）
    QSplineSeries* m_series;
public slots:


};
