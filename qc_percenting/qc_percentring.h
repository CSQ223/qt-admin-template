#ifndef QC_PERCENTRING_H
#define QC_PERCENTRING_H

#include <QColor>
#include <QPainter>
#include <QWidget>

class QC_PercentRing : public QWidget
{
    Q_OBJECT
public:
    explicit QC_PercentRing(QWidget *parent = nullptr);
    void setValue(int data);
    void setTextColor(QColor color);
    void setOutCircleColor(QColor color);
    void setMidCircleColor(QColor color);
    void setInnerCircleColor(QColor color);
    void setCenterCircleColor(QColor color);
    void setPieColorStart(QColor color);

protected:
    void paintEvent(QPaintEvent *);
    void drawScale(QPainter *painter);
    void drawColorPie(QPainter *painter);
    void drawCenterCircle(QPainter *painter);
    void drawOuterCircle(QPainter *painter);
    void drawMiddleCircle(QPainter *painter);
    void drawInterCircle(QPainter *painter);

    void drawText(QPainter *painter);
private:
    int value;
    QColor textColor;
    QColor pieColorStart;
    QColor outerCircleColor;
    QColor innerCircleColor;
    QColor midCircleColor;
    QColor centerCircleColor;
signals:

public slots:
};

#endif // QC_PERCENTRING_H
