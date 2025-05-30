#include "sslmain.h"
#include "ui_sslmain.h"

SSLMain::SSLMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SSLMain)
{
    ui->setupUi(this);

    // Cria o grupo de botões
    QButtonGroup *grupo = new QButtonGroup(this);
    grupo->setExclusive(true); // Garante que apenas um fique selecionado por vez

    // Adiciona os botões ao grupo (use os objectNames definidos no Qt Designer)
    grupo->addButton(ui->btnMap);
    grupo->addButton(ui->btnDelivery);
    grupo->addButton(ui->btnCosts);

    setupMap();
}


SSLMain::~SSLMain()
{
    delete ui;
}

void SSLMain::desenhaCurvaS(QGraphicsScene *scene, int x1, int y1, int x2, int y2){
    QPainterPath path;
    path.moveTo(x1, y1);

    // Calcula ponto médio entre as cidades
    int midX = (x1 + x2) / 2;
    int midY = (y1 + y2) / 2;

    // Diferença entre os pontos
    int dx = x2 - x1;
    int dy = y2 - y1;

    // Gera pontos de controle cruzados para formar o "S"
    QPointF control1(midX - dy / 2, midY + dx / 2);  // vira à esquerda e sobe
    QPointF control2(midX + dy / 2, midY - dx / 2);  // vira à direita e desce

    // Use as coordenadas dos pontos de controle, não o QPointF diretamente
    path.cubicTo(control1.x(), control1.y(),
                 control2.x(), control2.y(),
                 x2, y2);

    // Adiciona à cena
    scene->addPath(path, QPen(Qt::gray, 2));
}

void SSLMain::onEllipseClicked(ClickableEllipse *ellipse)
{
    QColor green("#25c665");
    if (selectedEllipses.contains(ellipse)) {
        // Se já está selecionada, não faz nada (ou pode tirar seleção se quiser)
        return;
    }

    if (selectedEllipses.size() == 2) {
        // Remove a cor vermelha da primeira (mais antiga)
        ClickableEllipse *oldest = selectedEllipses.takeFirst();
        oldest->setBrush(green);
    }

    // Adiciona a nova selecionada e pinta de vermelho
    selectedEllipses.append(ellipse);
    // ui->origin->setText("TESTE"); -> Pensar em uma forma de pegar cidade 1 e 2 e jogar para o certo
    ellipse->setBrush(Qt::green);
}

void SSLMain::setupMap()
{
    QColor gray("#3c3c3c");
    QColor brown("#8f3f00");

    // Cria a cena onde vamos desenhar
    QGraphicsScene *scene = new QGraphicsScene(this);

    // Ponto 1
    int x1 = 50, y1 = 100;
    ClickableEllipse *ellipse1 = new ClickableEllipse(x1,y1,20,20);
    ellipse1->setZValue(1);
    scene->addItem(ellipse1);
    connect(ellipse1, &ClickableEllipse::clicked, this, &SSLMain::onEllipseClicked);

    QGraphicsTextItem *cidade1 = scene->addText("Cidade A");
    cidade1->setDefaultTextColor(Qt::green);
    cidade1->setPos(x1-29, y1-25);

    // Ponto 2
    int x2 = 50, y2 = 400;
    ClickableEllipse *ellipse2 = new ClickableEllipse(x2,y2,20,20);
    ellipse2->setZValue(1);
    scene->addItem(ellipse2);
    connect(ellipse2, &ClickableEllipse::clicked, this, &SSLMain::onEllipseClicked);

    QGraphicsTextItem *cidade2 = scene->addText("Cidade B");
    cidade2->setDefaultTextColor(Qt::green);
    cidade2->setPos(x2, y2+25);

    // Ponto 3
    int x3 = 350, y3 = 120;
    ClickableEllipse *ellipse3 = new ClickableEllipse(x3,y3,20,20);
    ellipse3->setZValue(1);
    scene->addItem(ellipse3);
    connect(ellipse3, &ClickableEllipse::clicked, this, &SSLMain::onEllipseClicked);

    QGraphicsTextItem *cidade3 = scene->addText("Cidade C");
    cidade3->setDefaultTextColor(Qt::green);
    cidade3->setPos(x3+25, y3);

    // Ponto 4
    int x4 = 150, y4 = 250;
    ClickableEllipse *ellipse4 = new ClickableEllipse(x4,y4,20,20);
    ellipse4->setZValue(1);
    scene->addItem(ellipse4);
    connect(ellipse4, &ClickableEllipse::clicked, this, &SSLMain::onEllipseClicked);

    QGraphicsTextItem *cidade4 = scene->addText("Cidade D");
    cidade4->setDefaultTextColor(Qt::green);
    cidade4->setPos(x4, y4+25);

    // Ponto 5
    int x5 = 500, y5 = 260;
    ClickableEllipse *ellipse5 = new ClickableEllipse(x5,y5,20,20);
    ellipse5->setZValue(1);
    scene->addItem(ellipse5);
    connect(ellipse5, &ClickableEllipse::clicked, this, &SSLMain::onEllipseClicked);

    QGraphicsTextItem *cidade5 = scene->addText("Cidade E");
    cidade5->setDefaultTextColor(Qt::green);
    cidade5->setPos(x5, y5+25);


    // A → C (curva)
    {
        QPainterPath curvaAC;
        curvaAC.moveTo(x1 + 10, y1 + 10);
        curvaAC.cubicTo(x1 + 50, y1 - 50, x3 - 30, y3 - 10, x3 + 10, y3 + 10);
        scene->addPath(curvaAC, QPen(gray, 2));
    }

    // A → D (reta)
    scene->addLine(x1 + 10, y1 + 10, x4 + 10, y4 + 10, QPen(gray, 2));

    // D → E (curva)
    {
        QPainterPath curvaDE;
        curvaDE.moveTo(x4 + 10, y4 + 10);
        curvaDE.cubicTo(x4 + 100, y4, x5 - 100, y5 + 20, x5 + 10, y5 + 10);
        scene->addPath(curvaDE, QPen(brown, 2));
    }

    // C → E (reta)
    scene->addLine(x3 + 10, y3 + 10, x5 + 10, y5 + 10, QPen(gray, 2));

    // B → D (reta ou curva leve)
    scene->addLine(x2 + 10, y2 + 10, x4 + 10, y4 + 10, QPen(gray, 2));

    // Associa a cena ao QGraphicsView do Designer
    ui->mapView->setScene(scene);
}
