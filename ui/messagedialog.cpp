#include "messagedialog.h"
#include "ui_messagedialog.h"

MessageDialog::MessageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageDialog)
{
    ui->setupUi(this);
}

MessageDialog::~MessageDialog()
{
    delete ui;
}

void MessageDialog::setMessage(QString title, QString msg)
{
    setWindowTitle(title);
    ui->label->setText(title);
    ui->textBrowser->setText(msg);
}
