#include "sslmain.h"
#include "ui_sslmain.h"
#include "graph.h"

SSLMain::SSLMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SSLMain)
{
    ui->setupUi(this);
    connect(ui->searchAddr, &QLineEdit::returnPressed, this, &SSLMain::processSearchAddress);

    // Header Menu
    QButtonGroup *grupo = new QButtonGroup(this);
    grupo->setExclusive(true);
    grupo->addButton(ui->btnMap);
    grupo->addButton(ui->btnDelivery);
    grupo->addButton(ui->btnCosts);

    setupMap();
    setupPathMap();
}

SSLMain::~SSLMain()
{
    delete ui;
}

void SSLMain::processSearchAddress() {
    QString texto = ui->searchAddr->text().trimmed();

    if (texto.isEmpty()) return;

    if (!cityMap.contains(texto)) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Cidade não encontrada");
        msgBox.setText("Cidade não está no mapa.");
        msgBox.setStyleSheet("QLabel { color: #fbfafa; }"
                             "QMessageBox { background-color: #2b2b2b; }"); // opcional: fundo escuro
        msgBox.exec();
        return;
    }

    if (originCity.isEmpty()) {
        originCity = texto;
        textOriginCity = true;
        ui->origin->setText(originCity);
    } else if (destinationCity.isEmpty()) {
        destinationCity = texto;
        textDestinationCity = true;
        ui->destination->setText(destinationCity);
    } else {
        originCity = texto;
        destinationCity.clear();
        textOriginCity = true;
        textDestinationCity = false;
        ui->origin->setText(originCity);
        ui->destination->setText("");
    }

    ui->searchAddr->clear();
}

void SSLMain::clickedCity(QString cityName) {
    if (originCity.isEmpty() || textDestinationCity) {
        originCity = cityName;
        textOriginCity = false;
        destinationCity.clear();
        textDestinationCity = false;
        ui->origin->setText(originCity);
        ui->destination->setText("");

        // Limpa o caminho desenhado, se houver
        clearPathDrawing();
    } else if (destinationCity.isEmpty()) {
        destinationCity = cityName;
        textDestinationCity = false;
        ui->destination->setText(destinationCity);
    } else {
        originCity = cityName;
        destinationCity.clear();
        textOriginCity = false;
        textDestinationCity = false;
        ui->origin->setText(originCity);
        ui->destination->setText("");

        // Limpa o caminho desenhado, se houver
        clearPathDrawing();
    }
}

void SSLMain::clearPathDrawing() {
    for (auto item : pathItems) {
        scene->removeItem(item);
        delete item;
    }
    pathItems.clear();
}


void SSLMain::onCityItemClicked(cityItem *city)
{
    QColor selected("#fbfafa");
    QColor original("#25c665");
    if (selectedCities.contains(city)) {
        return;
    }

    if (selectedCities.size() == 2) {
        cityItem *oldest = selectedCities.takeFirst();
        oldest->setBrush(original);
    }

    selectedCities.append(city);
    city->setBrush(selected);
}

void SSLMain::setupMap()
{
    QColor gray("#3c3c3c");
    QColor brown("#8f3f00");

    // Cria a cena onde vamos desenhar
    scene = new QGraphicsScene(this);

    // City A Text
    int x1 = 50, y1 = 100;
    QGraphicsTextItem *cityAt = scene->addText("Cidade A");
    cityAt->setDefaultTextColor(Qt::green);
    cityAt->setPos(x1-29, y1-25);

    // City A Ellipse
    cityItem* cityA = new cityItem("Cidade A", x1, y1, 20, 20);
    cityA->setZValue(1);
    scene->addItem(cityA);
    cityMap["Cidade A"] = QPointF(x1,y1);

    connect(cityA, &cityItem::clicked, this, [=](cityItem *c) {
        clickedCity(c->name);
        onCityItemClicked(c);
    });

    // Para atualizar a cor (opcional):
    //cityA->setBrush((cityA->name == originCity) ? Qt::blue :
    //                   (cityA->name == destinationCity) ? Qt::red : Qt::green);

    // City B Text
    int x2 = 50, y2 = 400;
    QGraphicsTextItem *cityBt = scene->addText("Cidade B");
    cityBt->setDefaultTextColor(Qt::green);
    cityBt->setPos(x2, y2+25);

    // City B Ellipse
    cityItem* cityB = new cityItem("Cidade B", x2, y2, 20,20);
    cityB->setZValue(1);
    scene->addItem(cityB);
    cityMap["Cidade B"] = QPointF(x2,y2);

    connect(cityB, &cityItem::clicked, this, [=](cityItem *c) {
        clickedCity(c->name);
        onCityItemClicked(c);
    });

    // City C Text
    int x3 = 350, y3 = 120;
    QGraphicsTextItem *cityCt = scene->addText("Cidade C");
    cityCt->setDefaultTextColor(Qt::green);
    cityCt->setPos(x3+25, y3);

    // City C Ellipse
    cityItem* cityC = new cityItem("Cidade C", x3, y3, 20,20);
    cityC->setZValue(1);
    scene->addItem(cityC);
    cityMap["Cidade C"] = QPointF(x3,y3);

    connect(cityC, &cityItem::clicked, this, [=](cityItem *c) {
        clickedCity(c->name);
        onCityItemClicked(c);
    });

    // City D Text
    int x4 = 150, y4 = 250;
    QGraphicsTextItem *cityDt = scene->addText("Cidade D");
    cityDt->setDefaultTextColor(Qt::green);
    cityDt->setPos(x4, y4+25);

    // City D Ellipse
    cityItem* cityD = new cityItem("Cidade D", x4, y4, 20, 20);
    cityD->setZValue(1);
    scene->addItem(cityD);
    cityMap["Cidade D"] = QPointF(x4,y4);

    connect(cityD, &cityItem::clicked, this, [=](cityItem *c) {
        clickedCity(c->name);
        onCityItemClicked(c);
    });

    // City E Text
    int x5 = 500, y5 = 260;
    QGraphicsTextItem *cityEt = scene->addText("Cidade E");
    cityEt->setDefaultTextColor(Qt::green);
    cityEt->setPos(x5, y5+25);

    // City E Ellipse
    cityItem* cityE = new cityItem("Cidade E", x5, y5, 20,20);
    cityE->setZValue(1);
    scene->addItem(cityE);
    cityMap["Cidade E"] = QPointF(x5,y5);

    connect(cityE, &cityItem::clicked, this, [=](cityItem *c){
        clickedCity(c->name);
        onCityItemClicked(c);
    });

    // A → C
    {
        QPainterPath curveAC;
        curveAC.moveTo(x1 + 10, y1 + 10);
        curveAC.cubicTo(x1 + 50, y1 - 50, x3 - 30, y3 - 10, x3 + 10, y3 + 10);
        scene->addPath(curveAC, QPen(gray, 2));
    }

    // A → D
    {
        QPainterPath lineAD;
        lineAD.moveTo(x1 + 10, y1 + 10);
        lineAD.lineTo(x4 + 10, y4 + 10);
        scene->addPath(lineAD, QPen(gray, 2));
    }

    // D → E
    {
        QPainterPath curveDE;
        curveDE.moveTo(x4 + 10, y4 + 10);
        curveDE.cubicTo(x4 + 100, y4, x5 - 100, y5 + 20, x5 + 10, y5 + 10);
        scene->addPath(curveDE, QPen(brown, 2));
    }

    // C → E
    {
        QPainterPath lineCE;
        lineCE.moveTo(x3 + 10, y3 + 10);
        lineCE.lineTo(x5 + 10, y5 + 10);
        scene->addPath(lineCE, QPen(gray, 2));
    }

    // B → D
    {
        QPainterPath lineBD;
        lineBD.moveTo(x2 + 10, y2 + 10);
        lineBD.lineTo(x4 + 10, y4 + 10);
        scene->addPath(lineBD, QPen(gray, 2));
    }

    // Associa a cena ao QGraphicsView do Designer
    ui->mapView->setScene(scene);
}

void SSLMain::setupPathMap()
{
    auto pA = cityMap["Cidade A"];
    auto pB = cityMap["Cidade B"];
    auto pC = cityMap["Cidade C"];
    auto pD = cityMap["Cidade D"];
    auto pE = cityMap["Cidade E"];
    QColor green("#25c665");

    pathMap[{ "Cidade A", "Cidade C" }] = [=](QGraphicsScene* s){
        QPainterPath p;
        p.moveTo(pA.x() + 10, pA.y() + 10);
        p.cubicTo(pA.x() + 50, pA.y() - 50, pC.x() - 30, pC.y() - 10, pC.x() + 10, pC.y() + 10);
        QGraphicsPathItem* item = s->addPath(p, QPen(green, 3));
        pathItems.append(item);  // Adiciona à lista para depois remover
    };
    pathMap[{ "Cidade C", "Cidade A" }] = pathMap[{ "Cidade A", "Cidade C" }];

    pathMap[{ "Cidade A", "Cidade D" }] = [=](QGraphicsScene* s){
        QPainterPath p;
        p.moveTo(pA.x() + 10, pA.y() + 10);
        p.lineTo(pD.x() + 10, pD.y() + 10);
        QGraphicsPathItem* item = s->addPath(p, QPen(green, 3));
        pathItems.append(item);  // Adiciona à lista para depois remover
    };
    pathMap[{ "Cidade D", "Cidade A" }] = pathMap[{ "Cidade A", "Cidade D" }];

    pathMap[{ "Cidade D", "Cidade E" }] = [=](QGraphicsScene* s){
        QPainterPath p;
        p.moveTo(pD.x() + 10, pD.y() + 10);
        p.cubicTo(pD.x() + 100, pD.y(), pE.x() - 100, pE.y() + 20, pE.x() + 10, pE.y() + 10);
        QGraphicsPathItem* item = s->addPath(p, QPen(green, 3));
        pathItems.append(item);  // Adiciona à lista para depois remover
    };
    pathMap[{ "Cidade E", "Cidade D" }] = pathMap[{ "Cidade D", "Cidade E" }];

    pathMap[{ "Cidade C", "Cidade E" }] = [=](QGraphicsScene* s){
        QPainterPath p;
        p.moveTo(pC.x() + 10, pC.y() + 10);
        p.lineTo(pE.x() + 10, pE.y() + 10);
        QGraphicsPathItem* item = s->addPath(p, QPen(green, 3));
        pathItems.append(item);  // Adiciona à lista para depois remover
    };
    pathMap[{ "Cidade E", "Cidade C" }] = pathMap[{ "Cidade C", "Cidade E" }];

    pathMap[{ "Cidade B", "Cidade D" }] = [=](QGraphicsScene* s){
        QPainterPath p;
        p.moveTo(pB.x() + 10, pB.y() + 10);
        p.lineTo(pD.x() + 10, pD.y() + 10);
        QGraphicsPathItem* item = s->addPath(p, QPen(green, 3));
        pathItems.append(item);  // Adiciona à lista para depois remover
    };
    pathMap[{ "Cidade D", "Cidade B" }] = pathMap[{ "Cidade B", "Cidade D" }];
}

void SSLMain::highlightPath(const std::vector<std::string>& path)
{
    clearPathDrawing();  // limpa caminhos antigos

    for (size_t i = 0; i + 1 < path.size(); ++i)
    {
        std::pair<std::string, std::string> edge = { path[i], path[i+1] };
        if (pathMap.count(edge))
        {
            pathMap[edge](scene);  // desenha e registra na pathItems
        }
    }
}

void SSLMain::on_btnMap_clicked()
{
    ui->body->setCurrentIndex(0);
}


void SSLMain::on_btnDelivery_clicked()
{
    ui->body->setCurrentIndex(1);
}


void SSLMain::on_btnCosts_clicked()
{
    ui->body->setCurrentIndex(2);
}


void SSLMain::on_calcBtn_clicked()
{
    QMessageBox calcMsgBox;
    calcMsgBox.setIcon(QMessageBox::Warning);
    calcMsgBox.setWindowTitle("Dados Faltando");
    calcMsgBox.setStyleSheet("QLabel { color: #fbfafa; }"
                             "QMessageBox { background-color: #2b2b2b; }");

    Graph graph;
    graph.addEdge("Cidade A", "Cidade D", 58);
    graph.addEdge("Cidade D", "Cidade A", 58);
    graph.addEdge("Cidade A", "Cidade C", 85);
    graph.addEdge("Cidade C", "Cidade A", 85);
    graph.addEdge("Cidade B", "Cidade D", 55);
    graph.addEdge("Cidade D", "Cidade B", 55);
    graph.addEdge("Cidade C", "Cidade E", 57);
    graph.addEdge("Cidade E", "Cidade C", 57);

    // Estrada de terra com penalidade
    graph.addEdge("Cidade D", "Cidade E", 78);  // 30
    graph.addEdge("Cidade E", "Cidade D", 78);  // 30

    if(ui->origin->text()!="")
    {
        if(ui->destination->text()!="")
        {
            QString originCityCalc = ui->origin->text();
            QString destinationCityCalc = ui->destination->text();

            // Dijkstra
            if(ui->dijkstra->isChecked())
            {
                if (graph.dijkstra(originCityCalc.toStdString(), destinationCityCalc.toStdString())) {
                    ui->distanceResult->setText(QString::number(graph.getDistance()) + " km");
                    ui->costRestult->setText("R$ " + QString::number(graph.getCost(), 'f', 2));
                    ui->timeResult->setText(QString::number(graph.getTime(), 'f', 1) + " h");
                    //scene->clear();      // limpa cena
                    //setupMap();          // redesenha o mapa base
                    highlightPath(graph.getPath());  // desenha caminho azul
                }
                else
                {
                    calcMsgBox.setText("Caminho não encontrado");
                }
            }

            // Decision Tree
            if(ui->dTree->isChecked())
            {

            }
        }
        else
        {
            calcMsgBox.setText("Selecione uma cidade de destino");
            calcMsgBox.exec();
        }
    }
    else
    {
        calcMsgBox.setText("Selecione uma cidade de origem");
        calcMsgBox.exec();
    }
}
