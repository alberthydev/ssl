#include "editdialog.h"
#include "qmessagebox.h"
#include "ui_editdialog.h"

EditDialog::EditDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditDialog)
{
    ui->setupUi(this);
}

EditDialog::~EditDialog()
{
    delete ui;
}

void EditDialog::setValues(const QString &product, const QString &weight)
{
    ui->productEditInput->setText(product);
    ui->weightEditInput->setText(weight);
}

QString EditDialog::getProduct() const
{
    return ui->productEditInput->text();
}

QString EditDialog::getWeight() const
{
    return ui->weightEditInput->text();
}

void EditDialog::on_okBtn_clicked()
{
    QMessageBox editMsgBox;
    editMsgBox.setIcon(QMessageBox::Warning);
    editMsgBox.setWindowTitle("Campos vazios");
    editMsgBox.setStyleSheet("QLabel { color: #fbfafa; }"
                             "QMessageBox { background-color: #2b2b2b; }");

    if (ui->productEditInput->text().isEmpty() || ui->weighEdittLabel->text().isEmpty()) {
        editMsgBox.setText("Preencha todos os campos.");
        return;
    }

    accept();
}


void EditDialog::on_cancelBtn_clicked()
{
    reject();
}

