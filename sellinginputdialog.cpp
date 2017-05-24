#include "sellinginputdialog.h"
#include "ui_sellinginputdialog.h"

#include <QSqlRelationalTableModel>
#include <QDataWidgetMapper>
#include <QMessageBox>

SellingInputDialog::SellingInputDialog(QSqlRelationalTableModel *model, QWidget *parent) :
    QDialog(parent), ui(new Ui::SellingInputDialog), mapper(new QDataWidgetMapper(parent)),
    model(model), currentRow(0)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint |
                   Qt::MSWindowsFixedSizeDialogHint);

    ui->leField6->setReadOnly(true);

    connect(ui->pbCancel, &QAbstractButton::clicked, this, &SellingInputDialog::close);
    connect(ui->pbOk, &QAbstractButton::clicked, this, &SellingInputDialog::submit);

    ui->leField5->setValidator(new QIntValidator(0, 10000000, this));

    mapper->setModel(model);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->addMapping(ui->label_1, 0, "text");

    ui->cbField1->setModel(model->relationModel(1));
    ui->cbField1->setModelColumn(1);
    mapper->addMapping(ui->cbField1, 1);

    ui->cbField2->setModel(model->relationModel(2));
    ui->cbField2->setModelColumn(2);
    mapper->addMapping(ui->cbField2, 2);

    ui->cbField3->setModel(model->relationModel(3));
    ui->cbField3->setModelColumn(1);
    mapper->addMapping(ui->cbField3, 3);

    mapper->addMapping(ui->deField4, 5);
    mapper->addMapping(ui->leField5, 5);
    mapper->addMapping(ui->leField6, 6);
}

SellingInputDialog::~SellingInputDialog()
{
    delete ui;
}

void SellingInputDialog::manualMapping()
{
   ui->leField6->setText(QString::number(ui->leField6->text().toDouble(), 'f', 2));

}

int SellingInputDialog::exec(const QModelIndex &index)
{
     modelIndex = index;
     currentRow = modelIndex.row();
     mapper->setCurrentIndex(currentRow);
     manualMapping();
     return QDialog::exec();
}

int SellingInputDialog::exec()
{
    this->clearWindow();
    currentRow = 0;
    mapper->setCurrentIndex(-1);
    return QDialog::exec();
}

void SellingInputDialog::clearWindow()
{
    ui->label_1->clear();
    ui->cbField1->clear();
    ui->cbField2->clear();
    ui->deField4->clear();
    ui->leField5->clear();
    ui->leField6->clear();
    ui->cbField1->setCurrentIndex(-1);
    ui->cbField2->setCurrentIndex(-1);
    ui->cbField3->setCurrentIndex(-1);
    ui->deField4->setDate(QDate::currentDate());
}

int SellingInputDialog::relationIndex(QComboBox *widget)
{
    QSqlTableModel *m = qobject_cast<QSqlTableModel *>(widget->model());
    return m->data(m->index(widget->currentIndex(), 0)).toInt();
}
void SellingInputDialog::refresh()
{
    model->setFilter(QString());
    model->select();
}

void SellingInputDialog::submit()
{
    if(!modelIndex.isValid())
        if(!model->insertRow(currentRow))
            return;

    if(ui->leField5->text().toDouble() <= 0){
        QMessageBox::warning(this, "Ошибка", "Количество должно быть больше нуля");
        refresh();
        return;
    }

    if(ui->cbField1->currentIndex() == -1 ||
            ui->cbField2->currentIndex() == -1 ||
            ui->cbField3->currentIndex() == -1){
        QMessageBox::warning(this, "Ошибка", "Не указана связь между главной и дочерней таблицей");
        refresh();
        return;
    }

    model->setData(model->index(currentRow, 1), relationIndex(ui->cbField1));
    model->setData(model->index(currentRow, 2), relationIndex(ui->cbField2));
    model->setData(model->index(currentRow, 3), relationIndex(ui->cbField3));

    model->setData(model->index(currentRow, 4), QDate::fromString(ui->deField4->text(), "dd.MM.yyyy"));
    model->setData(model->index(currentRow, 5), ui->leField5->text());

    if(model->submitAll())
        accept();
    else{
        QMessageBox::warning(this, "Ошибка", "Не удается осуществить продажу. \n"
                                             "Возможно товара нет в наличии.");
        refresh();
    }
}
