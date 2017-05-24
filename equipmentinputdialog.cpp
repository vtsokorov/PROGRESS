#include "equipmentinputdialog.h"
#include "ui_equipmentinputdialog.h"

#include <QSqlRelationalTableModel>
#include <QDataWidgetMapper>
#include <QMessageBox>

EquipmentInputDialog::EquipmentInputDialog(QSqlRelationalTableModel *model, QWidget *parent) :
    QDialog(parent), ui(new Ui::EquipmentInputDialog), model(model), mapper(new QDataWidgetMapper(parent)),
    currentRow(0)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint |
                   Qt::MSWindowsFixedSizeDialogHint);

    connect(ui->pbCancel, &QAbstractButton::clicked, this, &EquipmentInputDialog::close);
    connect(ui->pbOk, &QAbstractButton::clicked, this, &EquipmentInputDialog::submit);

    ui->leField3->setValidator(new QRegExpValidator(QRegExp("[.0-9]+"), this));
    ui->leField5->setValidator(new QIntValidator(0, 10000000, this));

    mapper->setModel(model);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->addMapping(ui->label_1, 0, "text");

    ui->cbField1->setModel(model->relationModel(1));
    ui->cbField1->setModelColumn(1);
    mapper->addMapping(ui->cbField1, 1);
    mapper->addMapping(ui->leField2, 2);
    mapper->addMapping(ui->leField3, 3);
    mapper->addMapping(ui->leField4, 4);
    mapper->addMapping(ui->leField5, 5);
    mapper->addMapping(ui->deField6, 6);
}

EquipmentInputDialog::~EquipmentInputDialog()
{
    delete ui;
}

void EquipmentInputDialog::manualMapping()
{
   ui->leField3->setText(QString::number(ui->leField3->text().toDouble(), 'f', 2));
}

int EquipmentInputDialog::exec(const QModelIndex &index)
{
     modelIndex = index;
     currentRow = modelIndex.row();
     mapper->setCurrentIndex(currentRow);
     manualMapping();
     return QDialog::exec();
}

int EquipmentInputDialog::exec()
{
    this->clearWindow();
    currentRow = 0;
    mapper->setCurrentIndex(-1);
    return QDialog::exec();
}

void EquipmentInputDialog::clearWindow()
{
    ui->label_1->clear();
    ui->cbField1->setCurrentIndex(-1);
    ui->leField2->clear();
    ui->leField3->clear();
    ui->leField4->clear();
    ui->leField5->clear();
    ui->deField6->clear();
}

int EquipmentInputDialog::relationIndex(QComboBox *widget)
{
    QSqlTableModel *m = qobject_cast<QSqlTableModel *>(widget->model());
    return m->data(m->index(widget->currentIndex(), 0)).toInt();
}

void EquipmentInputDialog::refresh()
{
    model->setFilter(QString());
    model->select();
}

void EquipmentInputDialog::submit()
{
    if(!modelIndex.isValid())
        if(!model->insertRow(currentRow))
            return;

    if(ui->leField3->text().toDouble() <= 0){
        QMessageBox::warning(this, "Ошибка", "Цена должна быть больше нуля");
        refresh();
        return;
    }

    if(ui->cbField1->currentIndex() == -1){
        QMessageBox::warning(this, "Ошибка", "Не указана связь между главной и дочерней таблицей");
        refresh();
        return;
    }

    model->setData(model->index(currentRow, 1), relationIndex(ui->cbField1));
    model->setData(model->index(currentRow, 2), ui->leField2->text());
    model->setData(model->index(currentRow, 3), ui->leField3->text());
    model->setData(model->index(currentRow, 4), ui->leField4->text());
    model->setData(model->index(currentRow, 5), ui->leField5->text());
    model->setData(model->index(currentRow, 6), QDate::fromString(ui->deField6->text(), "dd.MM.yyyy"));


    if(model->submitAll())
        accept();
    else{
        QMessageBox::warning(this, "Ошибка", "Не удается сохранить данные");
         refresh();
    }

}


