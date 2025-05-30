#ifndef SSLMAIN_H
#define SSLMAIN_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QPen>
#include <QBrush>
#include <QGraphicsSceneMouseEvent>
#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui {
class SSLMain;
}
QT_END_NAMESPACE

class ClickableEllipse : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT

public:
    ClickableEllipse(qreal x, qreal y, qreal w, qreal h)
        : QGraphicsEllipseItem(x, y, w, h)
    {
        QColor green("#25c665");
        setBrush(green);
        setPen(QPen(Qt::black));
        setAcceptHoverEvents(true);
        setFlag(QGraphicsItem::ItemIsSelectable);
    }

signals:
    void clicked(ClickableEllipse*);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override
    {
        emit clicked(this);  // avisa quem estiver ouvindo
        QGraphicsEllipseItem::mousePressEvent(event);
    }
};

class SSLMain : public QMainWindow
{
    Q_OBJECT

public:
    SSLMain(QWidget *parent = nullptr);
    ~SSLMain();

private slots:
    void onEllipseClicked(ClickableEllipse *ellipse);

private:
    Ui::SSLMain *ui;
    void setupMap();
    void desenhaCurvaS(QGraphicsScene *scene, int x1, int y1, int x2, int y2);
    QList<ClickableEllipse*> selectedEllipses;
};


#endif // SSLMAIN_H
