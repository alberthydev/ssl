#include "sslmain.h"
#include "ui_sslmain.h"

SSLMain::SSLMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SSLMain)
{
    ui->setupUi(this);
}

SSLMain::~SSLMain()
{
    delete ui;
}
