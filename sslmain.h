#ifndef SSLMAIN_H
#define SSLMAIN_H

#include <QMainWindow>
#include <QMessageBox>
#include <QButtonGroup>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QPen>
#include <QBrush>
#include <QGraphicsSceneMouseEvent>
#include <QList>
#include <QMap>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui {
class SSLMain;
}
QT_END_NAMESPACE

class cityItem : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT

public:
    QString name;
    cityItem(const QString &name, qreal x, qreal y, qreal w, qreal h)
        : QGraphicsEllipseItem(x, y, w, h), name(name)
    {
        QColor green("#25c665");
        setBrush(green);
        setPen(QPen(Qt::black));
        setFlag(QGraphicsItem::ItemIsSelectable);
    }

signals:
    void clicked(cityItem*);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override
    {
        emit clicked(this);
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
    void onCityItemClicked(cityItem *city);
    void processSearchAddress();
    void clickedCity(QString cityName);
    void on_btnMap_clicked();
    void on_btnDelivery_clicked();
    void on_calcBtn_clicked();

    void on_registerBtn_clicked();

    void on_deleteBtn_clicked();

    void on_editBtn_clicked();

    void on_totalCalcBtn_clicked();

private:
    Ui::SSLMain *ui;
    QGraphicsScene* scene;
    void addCity(const QString &name, int x, int y, const QColor &textColor);
    void addPathCurve(int x1, int y1, int x2, int y2, const QColor &color, bool curve);
    void setupMap();
    QList<cityItem*> selectedCities;
    QString originCity;
    QString destinationCity;
    bool textOriginCity = false;
    bool textDestinationCity = false;
    QMap<QString, QPointF> cityMap;
    void setupPathMap();
    void highlightPath(const std::vector<std::string>& path);
    QVector<QGraphicsPathItem*> highlightedPaths;
    QList<QGraphicsItem*> pathItems;
    void clearPathDrawing();
    std::map<std::pair<std::string, std::string>, std::function<void(QGraphicsScene*)>> pathMap;
    QString lastOrigin;
    QString lastDestination;
    double lastDistance;
    double lastCost;
    double lastTime;
};


#endif // SSLMAIN_H
