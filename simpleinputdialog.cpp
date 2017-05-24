#include "simpleinputdialog.h"
#include "ui_simpleinputdialog.h"

#include <QSqlRelationalTableModel>
#include <QDataWidgetMapper>
#include <QMessageBox>

SimpleInputDialog::SimpleInputDialog(QSqlRelationalTableModel *model, QWidget *parent) :
    QDialog(parent), ui(new Ui::SimpleInputDialog), mapper(new QDataWidgetMapper(parent)),
    model(model), currentRow(0), windowType(true)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint |
                   Qt::MSWindowsFixedSizeDialogHint);

    connect(ui->pbCancel, &QAbstractButton::clicked, this, &SimpleInputDialog::close);
    connect(ui->pbOk, &QAbstractButton::clicked, this, &SimpleInputDialog::submit);

    if(model->tableName() == "PROVIDER")
    {
        ui->label_2->setText("Имя поставщика");
        ui->label_3->setText("Адрес");
        ui->label_4->setText("Телефон");
        windowType = true;
    }
    if(model->tableName() == "PAYTYPE")
    {
        ui->label_2->setText("Тип оплаты");
        ui->label_3->setText("Примечание");
        ui->label_4->hide(); ui->leField3->hide();
        resize(size().width(), size().height()-50);
        windowType = false;
    }
    if(model->tableName() == "EMPLOYEE")
    {
        ui->label_2->setText("ФИО");
        ui->label_3->setText("Должность");
        ui->label_4->setText("Телефон");
        windowType = true;
    }

    mapper->setModel(model);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->addMapping(ui->label_1, 0, "text");
    mapper->addMapping(ui->leField1, 1);
    mapper->addMapping(ui->leField2, 2);
    if(windowType)
        mapper->addMapping(ui->leField3, 3);
}

SimpleInputDialog::~SimpleInputDialog()
{
    delete ui;
}

int SimpleInputDialog::exec(const QModelIndex &index)
{
     modelIndex = index;
     currentRow = modelIndex.row();
     mapper->setCurrentIndex(currentRow);
     return QDialog::exec();
}

int SimpleInputDialog::exec()
{
    this->clearWindow();
    currentRow = 0;
    mapper->setCurrentIndex(-1);
    return QDialog::exec();
}

void SimpleInputDialog::clearWindow()
{
    ui->label_1->clear();
    ui->leField1->clear();
    ui->leField1->clear();
    ui->leField1->clear();
}

void SimpleInputDialog::refresh()
{
    model->setFilter(QString());
    model->select();
}

void SimpleInputDialog::submit()
{
    if(!modelIndex.isValid())
        if(!model->insertRow(currentRow))
            return;

    model->setData(model->index(currentRow, 1), ui->leField1->text());
    model->setData(model->index(currentRow, 2), ui->leField2->text());
    if(windowType)
        model->setData(model->index(currentRow, 3), ui->leField3->text());

    if(model->submitAll())
        accept();
    else{
        QMessageBox::warning(this, "Ошибка", "Не удается сохранить данные");
        refresh();
    }

}

