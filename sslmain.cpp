#include "sslmain.h"
#include "deliverytree.h"
#include "editdialog.h"
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

void SSLMain::addCity(const QString &name, int x, int y, const QColor &textColor = "#fbfafa") {
    QGraphicsTextItem *label = scene->addText(name);
    label->setDefaultTextColor(textColor);
    label->setZValue(1);
    label->setPos(x - 20, y - 30);

    cityItem *city = new cityItem(name, x, y, 20, 20);
    city->setZValue(1);
    scene->addItem(city);
    cityMap[name] = QPointF(x, y);

    connect(city, &cityItem::clicked, this, [=](cityItem *c) {
        clickedCity(c->name);
        onCityItemClicked(c);
    });
}

void SSLMain::addPathCurve(int x1, int y1, int x2, int y2, const QColor &color, bool curve = false) {
    QPainterPath path;
    path.moveTo(x1 + 10, y1 + 10);
    if (curve) {
        path.cubicTo(x1 + 50, y1 - 50, x2 - 30, y2 - 10, x2 + 10, y2 + 10);
    } else {
        path.lineTo(x2 + 10, y2 + 10);
    }
    scene->addPath(path, QPen(color, 2));
}

void SSLMain::setupMap() {
    scene = new QGraphicsScene(this);
    QColor gray("#3c3c3c");
    QColor brown("#8f3f00");

    // City coords
    QMap<QString, QPoint> coords = {
                                    {"Cidade A", {50, 100}},
                                    {"Cidade B", {50, 400}},
                                    {"Cidade C", {350, 120}},
                                    {"Cidade D", {150, 250}},
                                    {"Cidade E", {500, 260}},
                                    };

    // Add cities
    for (const auto &name : coords.keys()) {
        const QPoint &p = coords[name];
        addCity(name, p.x(), p.y());
    }

    // Add conections
    addPathCurve(50, 100, 350, 120, gray, true);   // A → C
    addPathCurve(50, 100, 150, 250, gray);         // A → D
    addPathCurve(150, 250, 500, 260, brown, true); // D → E
    addPathCurve(350, 120, 500, 260, gray);        // C → E
    addPathCurve(50, 400, 150, 250, gray);         // B → D

    // Atribue to QGraphicsView
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
        pathItems.append(item);
    };
    pathMap[{ "Cidade C", "Cidade A" }] = pathMap[{ "Cidade A", "Cidade C" }];

    pathMap[{ "Cidade A", "Cidade D" }] = [=](QGraphicsScene* s){
        QPainterPath p;
        p.moveTo(pA.x() + 10, pA.y() + 10);
        p.lineTo(pD.x() + 10, pD.y() + 10);
        QGraphicsPathItem* item = s->addPath(p, QPen(green, 3));
        pathItems.append(item);
    };
    pathMap[{ "Cidade D", "Cidade A" }] = pathMap[{ "Cidade A", "Cidade D" }];

    pathMap[{ "Cidade D", "Cidade E" }] = [=](QGraphicsScene* s){
        QPainterPath p;
        p.moveTo(pD.x() + 10, pD.y() + 10);
        p.cubicTo(pD.x() + 50, pD.y() - 50, pE.x() - 30, pE.y() - 10, pE.x() + 10, pE.y() + 10);
        QGraphicsPathItem* item = s->addPath(p, QPen(green, 3));
        pathItems.append(item);
    };
    pathMap[{ "Cidade E", "Cidade D" }] = pathMap[{ "Cidade D", "Cidade E" }];

    pathMap[{ "Cidade C", "Cidade E" }] = [=](QGraphicsScene* s){
        QPainterPath p;
        p.moveTo(pC.x() + 10, pC.y() + 10);
        p.lineTo(pE.x() + 10, pE.y() + 10);
        QGraphicsPathItem* item = s->addPath(p, QPen(green, 3));
        pathItems.append(item);
    };
    pathMap[{ "Cidade E", "Cidade C" }] = pathMap[{ "Cidade C", "Cidade E" }];

    pathMap[{ "Cidade B", "Cidade D" }] = [=](QGraphicsScene* s){
        QPainterPath p;
        p.moveTo(pB.x() + 10, pB.y() + 10);
        p.lineTo(pD.x() + 10, pD.y() + 10);
        QGraphicsPathItem* item = s->addPath(p, QPen(green, 3));
        pathItems.append(item);
    };
    pathMap[{ "Cidade D", "Cidade B" }] = pathMap[{ "Cidade B", "Cidade D" }];
}

void SSLMain::highlightPath(const std::vector<std::string>& path)
{
    clearPathDrawing();

    for (size_t i = 0; i + 1 < path.size(); ++i)
    {
        std::pair<std::string, std::string> edge = { path[i], path[i+1] };
        if (pathMap.count(edge))
        {
            pathMap[edge](scene);
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

    // Estrada de terra
    graph.addEdge("Cidade D", "Cidade E", 78, true);
    graph.addEdge("Cidade E", "Cidade D", 78, true);

    if(ui->origin->text()!= ""){

    }else{
        QMessageBox::warning(this, "Aviso", "Informe o nome para adicionar!");
    }

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
                    highlightPath(graph.getPath());
                }
                else
                {
                    calcMsgBox.setText("Caminho não encontrado");
                    calcMsgBox.exec();
                }
            }

            if (ui->dTree->isChecked())
            {
                if (graph.decisionTree(originCityCalc.toStdString(), destinationCityCalc.toStdString())) {
                    ui->distanceResult->setText(QString::number(graph.getDistance()) + " km");
                    ui->costRestult->setText("R$ " + QString::number(graph.getCost(), 'f', 2));
                    ui->timeResult->setText(QString::number(graph.getTime(), 'f', 1) + " h");
                    highlightPath(graph.getPath());
                }
                else
                {
                    calcMsgBox.setText("Caminho não encontrado");
                    calcMsgBox.exec();
                }
            }

            ui->routerTable->addItem(
                ui->origin->text() + " -> " + ui->destination->text() + "\n" +
                "Distância: " + ui->distanceResult->text() + "\n" +
                "Custo: " + ui->costRestult->text() + "\n" +
                "Tempo Estimado: " + ui->timeResult->text()
                );
            lastOrigin = originCityCalc;
            lastDestination = destinationCityCalc;
            lastDistance = graph.getDistance();
            lastCost = graph.getCost();
            lastTime = graph.getTime();
            ui->origin->setText("");
            ui->destination->setText("");
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

void SSLMain::on_registerBtn_clicked()
{
    QMessageBox productMsgBox;
    productMsgBox.setIcon(QMessageBox::Warning);
    productMsgBox.setWindowTitle("Dados Faltando");
    productMsgBox.setStyleSheet("QLabel { color: #fbfafa; }"
                             "QMessageBox { background-color: #2b2b2b; }");

    if(ui->productInput->text()!="")
    {
        if(ui->weightInput->text()!="")
        {
            ui->packageList->addItem(ui->productInput->text()+" : "+ui->weightInput->text());
        }
        else
        {
            productMsgBox.setText("Preencha o peso do produto");
            productMsgBox.exec();
        }
    }
    else
    {
        productMsgBox.setText("Preencha com um produto.");
        productMsgBox.exec();
    }
}

void SSLMain::on_deleteBtn_clicked()
{
    QMessageBox delMsgBox;
    delMsgBox.setIcon(QMessageBox::Warning);
    delMsgBox.setWindowTitle("Nenhum item selecionado");
    delMsgBox.setStyleSheet("QLabel { color: #fbfafa; }"
                             "QMessageBox { background-color: #2b2b2b; }");

    QListWidgetItem *selectedItem = ui->packageList->currentItem();

    if (!selectedItem || !(ui->packageList->row(selectedItem) >= 0)) {
        delMsgBox.setText("Por favor, selecione um item para deletar.");
        delMsgBox.exec();
        return;
    }

    foreach (QListWidgetItem * NAME, ui->packageList->selectedItems()) {
        delete ui->packageList->takeItem(ui->packageList->row(NAME));
    }

    ui->packageList->clearSelection();
    ui->packageList->setCurrentItem(nullptr);
}


void SSLMain::on_editBtn_clicked()
{
    QMessageBox editMsgBox;
    editMsgBox.setIcon(QMessageBox::Warning);
    editMsgBox.setWindowTitle("Nenhum item selecionado");
    editMsgBox.setStyleSheet("QLabel { color: #fbfafa; }"
                                "QMessageBox { background-color: #2b2b2b; }");

    QListWidgetItem *selectedItem = ui->packageList->currentItem();

    if (!selectedItem || !(ui->packageList->row(selectedItem) >= 0)) {
        editMsgBox.setText("Por favor, selecione um item para editar.");
        editMsgBox.exec();
        return;
    }

    QString itemText = selectedItem->text();
    QStringList parts = itemText.split(" : ");
    if (parts.size() != 2) return;

    EditDialog dialog(this);
    dialog.setValues(parts[0], parts[1]);

    if (dialog.exec() == QDialog::Accepted) {
        QString newProduct = dialog.getProduct();
        QString newWeight = dialog.getWeight();
        selectedItem->setText(newProduct + " : " + newWeight);
    }
}


void SSLMain::on_totalCalcBtn_clicked()
{
    ui->totalList->clear();

    DeliveryTree deliveryTree;

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle("Dados Faltando");
    msgBox.setStyleSheet("QLabel { color: #fbfafa; }"
                         "QMessageBox { background-color: #2b2b2b; }");

    QListWidgetItem* selectedRoute = ui->routerTable->currentItem();

    if(!selectedRoute)
    {
        msgBox.setText("Por favor, selecione uma rota na lista.");
        msgBox.exec();
        return;
    }

    QListWidgetItem* selectedProductItem = ui->packageList->currentItem();
    if (!selectedProductItem)
    {
        msgBox.setText("Por favor, selecione um produto na lista.");
        msgBox.exec();
        return;
    }

    QString productText = selectedProductItem->text();
    QStringList parts = productText.split(" : ");
    if (parts.size() != 2) return;

    QString nomeProduto = parts[0];
    QString pesoStr = parts[1].remove("kg").trimmed();
    bool ok;
    double peso = pesoStr.toDouble(&ok);
    if (!ok) return;

    double custoFinal = lastCost + (peso * 0.5);
    double tempoFinal = lastTime + (peso * 0.02);

    deliveryTree.insert(nomeProduto, peso, lastOrigin, lastDestination, lastDistance, custoFinal, tempoFinal);

    deliveryTree.fillListWidget(ui->totalList);
}

