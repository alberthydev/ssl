#ifndef SSLMAIN_H
#define SSLMAIN_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class SSLMain;
}
QT_END_NAMESPACE

class SSLMain : public QMainWindow
{
    Q_OBJECT

public:
    SSLMain(QWidget *parent = nullptr);
    ~SSLMain();

private:
    Ui::SSLMain *ui;
};
#endif // SSLMAIN_H
