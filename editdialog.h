#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>

namespace Ui {
class EditDialog;
}

class EditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditDialog(QWidget *parent = nullptr);
    ~EditDialog();

    void setValues(const QString &product, const QString &weight);
    QString getProduct() const;
    QString getWeight() const;

private slots:
    void on_okBtn_clicked();

    void on_cancelBtn_clicked();

private:
    Ui::EditDialog *ui;
};

#endif // EDITDIALOG_H
