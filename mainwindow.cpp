#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logindialog.h"
#include "pglobal.h"
#include "applicationmodel.h"

#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupWidget();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupWidget()
{
    lwUser = new QLabel(this);
    lwUser->setAlignment(Qt::AlignHCenter);
    lwUser->setMinimumSize(lwUser->sizeHint());

    lwHost = new QLabel(this);
    lwHost->setAlignment(Qt::AlignHCenter);
    lwHost->setMinimumSize(lwHost->sizeHint());

    lwBase = new QLabel(this);
    lwBase->setAlignment(Qt::AlignLeft);
    lwBase->setMinimumSize(lwBase->sizeHint());

    lwPort = new QLabel(this);
    lwPort->setAlignment(Qt::AlignHCenter);
    lwPort->setMinimumSize(lwPort->sizeHint());

    statusBar()->addWidget(lwUser);
    statusBar()->addWidget(lwHost);
    statusBar()->addWidget(lwPort);
    statusBar()->addWidget(lwBase, 1);

    ui->stackedWidget->setCurrentIndex(1);
    ui->treeWidget->expandAll();

    connect(ui->pbFilter, &QAbstractButton::clicked, this, &MainWindow::filterOn);
    connect(ui->pbClear, &QAbstractButton::clicked, this, &MainWindow::filterOff);
    connect(ui->pbAdd, &QAbstractButton::clicked, this, &MainWindow::addRecord);
    connect(ui->pbEdit, &QAbstractButton::clicked, this, &MainWindow::editRecord);
    connect(ui->pbDel, &QAbstractButton::clicked, this, &MainWindow::deleteRecord);
    connect(ui->pbRefresh, &QAbstractButton::clicked, this, &MainWindow::refresh);

    connect(ui->treeWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(selectedItem(QModelIndex)));
    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(openEditDialog(QModelIndex)));
    connect(ui->listQuerys, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(selectedQuery(QModelIndex)));
    connect(ui->listReports, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(selectedReports(QModelIndex)));
    emit ui->treeWidget->clicked(ui->treeWidget->model()->index(0,0));
}

void MainWindow::show()
{
    LoginDialog dlg(&db, this);
    while(!db.isOpen()){
        dlg.exec();
    }
    lwUser->setText(" Пользователь: " + db.userName() + " ");
    lwHost->setText(" Сервер: " + db.hostName() + " ");
    lwPort->setText(" Порт: "+QString::number(db.port()) + " ");
    lwBase->setText(" Размещение БД: " + db.databaseName() + " ");
    QMainWindow::show();

}

void MainWindow::selectedItem(const QModelIndex &index)
{
    int i = index.row();
    int j = index.parent().row()+1;

    auto callableObject = CreateModel::CallingMenu[ArrayIndex::indexes[i][j]];
    item = callableObject(ui->tableView, ui->stackedWidget, &db, this);

    model = item->viewAdapter(ui->panel);

    ui->leFilter->clear();
    item->setPlaceHolderText(ui->leFilter);
}

void MainWindow::filterOn()
{
   item->setFilter(ui->leFilter->text());
}

void MainWindow::filterOff()
{
   item->clearFilter();
   ui->leFilter->clear();
}

void MainWindow::openEditDialog(const QModelIndex &index)
{
    item->showDialog(index, this);
}

void MainWindow::editRecord()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if(select->hasSelection()){
        item->showDialog(select->selectedRows().first(), this);
    }
    else
        QMessageBox::information(this, "Инфо", "Укажите запись!");
}

void MainWindow::addRecord()
{
    item->showDialog(QModelIndex(), this);
}

void MainWindow::deleteRecord()
{
     item->deleteRecord();
}
void MainWindow::refresh()
{
     item->refresh();
}

void MainWindow::selectedQuery(const QModelIndex &index)
{
    item->execQuery(index.row());
}

void MainWindow::selectedReports(const QModelIndex &index)
{
    item->showReport(index.row());
}
