#include "applicationmodel.h"
#include "simpleinputdialog.h"
#include "equipmentinputdialog.h"
#include "sellinginputdialog.h"
#include "lrreportengine.h"
#include "querys.h"
#include "reports.h"

#include <QLineEdit>
#include <QTableView>
#include <QStackedWidget>

#include <QDate>
#include <QSqlRelationalTableModel>
#include <QDebug>

#include <QMessageBox>
#include <QInputDialog>
#include <QProgressDialog>
#include <QApplication>

AbstractApplicationModel::AbstractApplicationModel(QTableView *table, QStackedWidget *stackedWidget,
                                                   QSqlDatabase *db, QObject *parent):
    QObject(parent), table(table), stackedWidget(stackedWidget), report(new LimeReport::ReportEngine(parent)),
    m_progressDialog(nullptr), m_currentPage(0)
{
    model = new QSqlRelationalTableModel(parent, *db);
}

AbstractApplicationModel::~AbstractApplicationModel()
{
    delete model;
}

void AbstractApplicationModel::showDialog(const QModelIndex& index, QWidget *parent)
{
   qDebug() <<"do nothing";
}

void AbstractApplicationModel::showReport(int index, const QString& param)
{
    qDebug() <<"do nothing";
}

void AbstractApplicationModel::execQuery(int index, const QString& query)
{
    qDebug() <<"do nothing";
}

void AbstractApplicationModel::setFilter(const QString& filer)
{
    qDebug() <<"do nothing";
}

void AbstractApplicationModel::setPlaceHolderText(QLineEdit *widget)
{
    widget->setPlaceholderText(QString());
}

void AbstractApplicationModel::deleteRecord()
{
    QItemSelectionModel *select = table->selectionModel();
    if(select->hasSelection()){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(0, "Вопрос", "Вы действительно хотите удалить запись?", QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes){
            model->removeRow(select->selectedRows().first().row());
            model->submitAll();
        }
    }
    else
       QMessageBox::information(0, "Инфо", "Укажите запись!");
}

void AbstractApplicationModel::refresh()
{
    model->setFilter(QString());
    model->select();
}

void AbstractApplicationModel::clearFilter()
{
    model->setFilter(QString());
}

void AbstractApplicationModel::showAllColumn(int columnsSize)
{
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    for(int i = 0; i < columnsSize; ++i)
        table->showColumn(i);
}

void AbstractApplicationModel::selectTable(const QString &tableName)
{
    model->setTable(tableName);
    model->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
}

QDate AbstractApplicationModel::inputDate(const QString &title, bool &isOk)
{
  return QDate::fromString(QInputDialog::getText(table, tr("Ввод даты"),
            QString("%0 (дата в формате dd.MM.yyyy):").arg(title), QLineEdit::Normal,
              "01.01.2016", &isOk), "dd.MM.yyyy");
}

QString AbstractApplicationModel::inputData(const QString &title, bool &isOk)
{
    return QInputDialog::getText(table, tr("Ввод параметра запроса"),
              title, QLineEdit::Normal, "", &isOk);
}

QString AbstractApplicationModel::selectData(const QString &title, const QStringList &items, bool &isOk)
{
    return QInputDialog::getItem(table, tr("Ввод параметра запроса"),
                                             title, items, 0, false, &isOk);
}

void AbstractApplicationModel::renderStarted()
{
    m_currentPage = 0;
    m_progressDialog = new QProgressDialog(tr("Start render"),tr("Cancel"), 0, 0, 0);
    m_progressDialog->setWindowModality(Qt::WindowModal);
    connect(m_progressDialog, SIGNAL(canceled()), report, SLOT(cancelRender()));
    m_progressDialog->show();
    QApplication::processEvents();
}

void AbstractApplicationModel::renderPageFinished(int renderedPageCount)
{
    if (m_progressDialog){
        m_progressDialog->setLabelText(QString::number(renderedPageCount)+tr(" page rendered"));
        m_progressDialog->setValue(renderedPageCount);
    }
}

void AbstractApplicationModel::renderFinished()
{
    if (m_progressDialog){
        m_progressDialog->close();
        delete m_progressDialog;
    }
    m_progressDialog = 0;
}

//-------------------------

DatabaseModel::DatabaseModel(QTableView *table, QStackedWidget *stackedWidget,
                         QSqlDatabase *db, QObject *parent):
    AbstractApplicationModel(table, stackedWidget, db, parent) { }

QSqlRelationalTableModel* DatabaseModel::viewAdapter(QWidget *widgetPanel)
{
    table->setModel(model);
    stackedWidget->setCurrentIndex(0);
    widgetPanel->hide();
    return model;
}

//-------------------------

ReferenceModel::ReferenceModel(QTableView *table, QStackedWidget *stackedWidget,
                         QSqlDatabase *db, QObject *parent):
    AbstractApplicationModel(table, stackedWidget, db, parent) { }

QSqlRelationalTableModel* ReferenceModel::viewAdapter(QWidget *widgetPanel)
{
    table->setModel(model);
    stackedWidget->setCurrentIndex(2);
    widgetPanel->hide();
    return model;
}

//-------------------------

ProviderModel::ProviderModel(QTableView *table, QStackedWidget *stackedWidget,
                         QSqlDatabase *db, QObject *parent):
    AbstractApplicationModel(table, stackedWidget, db, parent) { }

QSqlRelationalTableModel* ProviderModel::viewAdapter(QWidget *widgetPanel)
{
    showAllColumn();
    selectTable("PROVIDER");
    model->select();
    table->setModel(model);
    model->setHeaderData(0, Qt::Horizontal, "№");
    model->setHeaderData(1, Qt::Horizontal, "Имя поставщика");
    model->setHeaderData(2, Qt::Horizontal, "Адрес");
    model->setHeaderData(3, Qt::Horizontal, "Телефон");
    table->setColumnWidth(0, 50);
    table->setColumnWidth(1, 150);
    table->setColumnWidth(2, 200);
    table->setColumnWidth(3, 100);
    stackedWidget->setCurrentIndex(1);
    widgetPanel->show();
    return model;
}

void ProviderModel::showDialog(const QModelIndex& index, QWidget *parent)
{
    SimpleInputDialog dlg(model, parent);
    if(index.isValid())
        dlg.exec(index);
    else
        dlg.exec();
}

void ProviderModel::setFilter(const QString& filer)
{
    model->setFilter(QString("PROVIDER_NAME LIKE '%%0%'").arg(filer));
}

void ProviderModel::setPlaceHolderText(QLineEdit *widget)
{
   widget->setPlaceholderText("Фильтер по имени поставщика");
}

//-------------------------

EquipmentModel::EquipmentModel(QTableView *table, QStackedWidget *stackedWidget,
                         QSqlDatabase *db, QObject *parent):
    AbstractApplicationModel(table, stackedWidget, db, parent) { }

QSqlRelationalTableModel* EquipmentModel::viewAdapter(QWidget *widgetPanel)
{
    showAllColumn();
    selectTable("EQUIPMENT");
    model->setRelation(1, QSqlRelation("PROVIDER", "ID_PROVIDER", "PROVIDER_NAME"));
    model->select();
    table->setModel(model);
    model->setHeaderData(0, Qt::Horizontal, "№");
    model->setHeaderData(1, Qt::Horizontal, "Поставщик");
    model->setHeaderData(2, Qt::Horizontal, "Наименование");
    model->setHeaderData(3, Qt::Horizontal, "Цена");
    model->setHeaderData(5, Qt::Horizontal, "Количество");
    table->setColumnWidth(0, 50);
    table->setColumnWidth(1, 150);
    table->setColumnWidth(2, 230);
    table->setColumnWidth(3, 75);
    table->setColumnWidth(5, 75);
    table->hideColumn(4);
    table->hideColumn(6);
    stackedWidget->setCurrentIndex(1);
    widgetPanel->show();
    return model;
}

void EquipmentModel::showDialog(const QModelIndex& index, QWidget *parent)
{
    EquipmentInputDialog dlg(model, parent);
    if(index.isValid())
        dlg.exec(index);
    else
        dlg.exec();
}

void EquipmentModel::setFilter(const QString& filer)
{
   model->setFilter(QString("EQUIPMENT_NAME LIKE '%%0%'").arg(filer));
}

void EquipmentModel::setPlaceHolderText(QLineEdit *widget)
{
   widget->setPlaceholderText("Фильтер по наименованию");
}

//-------------------------

EmployeeModel::EmployeeModel(QTableView *table, QStackedWidget *stackedWidget,
                         QSqlDatabase *db, QObject *parent):
    AbstractApplicationModel(table, stackedWidget, db, parent) { }

QSqlRelationalTableModel* EmployeeModel::viewAdapter(QWidget *widgetPanel)
{
    showAllColumn();
    selectTable("EMPLOYEE");
    model->select();
    table->setModel(model);
    model->setHeaderData(0, Qt::Horizontal, "№");
    model->setHeaderData(1, Qt::Horizontal, "ФИО");
    model->setHeaderData(2, Qt::Horizontal, "Должность");
    model->setHeaderData(3, Qt::Horizontal, "Телефон");
    table->setColumnWidth(0, 50);
    table->setColumnWidth(1, 150);
    table->setColumnWidth(2, 200);
    table->setColumnWidth(3, 100);
    stackedWidget->setCurrentIndex(1);
    widgetPanel->show();
    return model;
}

void EmployeeModel::showDialog(const QModelIndex& index, QWidget *parent)
{
    SimpleInputDialog dlg(model, parent);
    if(index.isValid())
        dlg.exec(index);
    else
        dlg.exec();
}

void EmployeeModel::setFilter(const QString& filer)
{
    model->setFilter(QString("EMPLOYEE_NAME LIKE '%%0%'").arg(filer));
}

void EmployeeModel::setPlaceHolderText(QLineEdit *widget)
{
   widget->setPlaceholderText("Фильтер по ФИО сотрудника");
}

//-------------------------

SellingModel::SellingModel(QTableView *table, QStackedWidget *stackedWidget,
                         QSqlDatabase *db, QObject *parent):
    AbstractApplicationModel(table, stackedWidget, db, parent) { }

QSqlRelationalTableModel* SellingModel::viewAdapter(QWidget *widgetPanel)
{
    showAllColumn();
    selectTable("SELLING");
    model->setRelation(1, QSqlRelation("EMPLOYEE", "ID_EMPLOYEE", "EMPLOYEE_NAME"));
    model->setRelation(2, QSqlRelation("EQUIPMENT", "ID_EQUIPMENT", "EQUIPMENT_NAME"));
    model->setRelation(3, QSqlRelation("PAYTYPE", "ID_PAYTYPE", "DEFINITION"));
    model->select();
    table->setModel(model);
    model->setHeaderData(0, Qt::Horizontal, "№");
    model->setHeaderData(2, Qt::Horizontal, "Наименование");
    model->setHeaderData(4, Qt::Horizontal, "Дата продажи");
    model->setHeaderData(5, Qt::Horizontal, "Количество");
    model->setHeaderData(6, Qt::Horizontal, "Сумма");
    table->setColumnWidth(0, 50);
    table->setColumnWidth(2, 200);
    table->setColumnWidth(4, 100);
    table->setColumnWidth(5, 75);
    table->setColumnWidth(6, 75);
    table->hideColumn(1);
    table->hideColumn(3);
    stackedWidget->setCurrentIndex(1);
    widgetPanel->show();
    return model;
}

void SellingModel::showDialog(const QModelIndex& index, QWidget *parent)
{
    SellingInputDialog dlg(model, parent);
    if(index.isValid())
        dlg.exec(index);
    else
        dlg.exec();
}

void SellingModel::setFilter(const QString& filer)
{
   model->setFilter(QString("SELLING_DATE LIKE '%%0%'").arg(filer));
}

void SellingModel::setPlaceHolderText(QLineEdit *widget)
{
   widget->setPlaceholderText("Фильтер по дате продажи");
}

//-------------------------

PaytypeModel::PaytypeModel(QTableView *table, QStackedWidget *stackedWidget,
                         QSqlDatabase *db, QObject *parent):
    AbstractApplicationModel(table, stackedWidget, db, parent) { }

QSqlRelationalTableModel* PaytypeModel::viewAdapter(QWidget *widgetPanel)
{
    showAllColumn();
    selectTable("PAYTYPE");
    model->select();
    table->setModel(model);
    model->setHeaderData(0, Qt::Horizontal, "№");
    model->setHeaderData(1, Qt::Horizontal, "Тип оплаты");
    model->setHeaderData(2, Qt::Horizontal, "Примечание");
    table->setColumnWidth(0, 50);
    table->setColumnWidth(1, 150);
    table->setColumnWidth(2, 200);
    stackedWidget->setCurrentIndex(1);
    widgetPanel->show();
    return model;
}

void PaytypeModel::showDialog(const QModelIndex& index, QWidget *parent)
{
    SimpleInputDialog dlg(model, parent);
    if(index.isValid())
        dlg.exec(index);
    else
        dlg.exec();
}

void PaytypeModel::setFilter(const QString& filer)
{
    model->setFilter(QString("DEFINITION LIKE '%%0%'").arg(filer));
}

void PaytypeModel::setPlaceHolderText(QLineEdit *widget)
{
   widget->setPlaceholderText("Фильтер по типу оплаты");
}

//-------------------------

QueryModel::QueryModel(QTableView *table, QStackedWidget *stackedWidget,
                         QSqlDatabase *db, QObject *parent):
    AbstractApplicationModel(table, stackedWidget, db, parent)
{
    listQuery.append(&QueryModel::execQuery1);
    listQuery.append(&QueryModel::execQuery2);
    listQuery.append(&QueryModel::execQuery3);
    listQuery.append(&QueryModel::execQuery4);
    listQuery.append(&QueryModel::execQuery5);
}

QSqlRelationalTableModel* QueryModel::viewAdapter(QWidget *widgetPanel)
{
    table->setModel(model);
    stackedWidget->setCurrentIndex(3);
    widgetPanel->hide();
    return model;
}

void QueryModel::execQuery(int index, const QString& query)
{
    (this->*listQuery[index])();
}

void QueryModel::setPlaceHolderText(QLineEdit *widget)
{
    widget->setPlaceholderText("Фильтр по данным выбоки не доступен");
}

void QueryModel::execQuery1()
{
    QSqlQueryModel *qmodel = new QSqlQueryModel(this);
    qmodel->setQuery(Querys::lastSelling, model->database());
    qmodel->setHeaderData(0, Qt::Horizontal, "Дата продажи");
    qmodel->setHeaderData(1, Qt::Horizontal, "ФИО сотрудника");
    qmodel->setHeaderData(2, Qt::Horizontal, "Наименование");
    qmodel->setHeaderData(3, Qt::Horizontal, "Цена");
    qmodel->setHeaderData(4, Qt::Horizontal, "Количество");
    qmodel->setHeaderData(5, Qt::Horizontal, "Сумма");
    qmodel->setHeaderData(6, Qt::Horizontal, "Тип оплаты");
    table->setModel(qmodel);
    table->setColumnWidth(0, 140);
    stackedWidget->setCurrentIndex(1);
}

void QueryModel::execQuery2()
{
    bool ok;
    QDate firstDate = inputDate("Укажите дату", ok);
    if (ok && firstDate.isValid()) {
        QSqlQueryModel *qmodel = new QSqlQueryModel(this);
        if (ok && firstDate.isValid()) {
            qmodel->setQuery(Querys::daySelling.arg(firstDate.toString("dd.MM.yyyy")), model->database());
            qmodel->setHeaderData(0, Qt::Horizontal, "Дата продажи");
            qmodel->setHeaderData(1, Qt::Horizontal, "ФИО сотрудника");
            qmodel->setHeaderData(2, Qt::Horizontal, "Наименование");
            qmodel->setHeaderData(3, Qt::Horizontal, "Цена");
            qmodel->setHeaderData(4, Qt::Horizontal, "Количество");
            qmodel->setHeaderData(5, Qt::Horizontal, "Сумма");
            qmodel->setHeaderData(6, Qt::Horizontal, "Тип оплаты");
            table->setModel(qmodel);
            table->setColumnWidth(0, 140);
            stackedWidget->setCurrentIndex(1);
        }
    }
}

void QueryModel::execQuery3()
{
    bool ok;
    QDate firstDate = inputDate("Укажите дату", ok);
    if (ok && firstDate.isValid()) {
        QSqlQueryModel *qmodel = new QSqlQueryModel(this);
        if (ok && firstDate.isValid()) {
            qmodel->setQuery(Querys::dayDelivery.arg(firstDate.toString("dd.MM.yyyy")), model->database());
            qmodel->setHeaderData(0, Qt::Horizontal, "Дата поставки");
            qmodel->setHeaderData(1, Qt::Horizontal, "Наименование");
            qmodel->setHeaderData(2, Qt::Horizontal, "Количество");
            qmodel->setHeaderData(3, Qt::Horizontal, "Цена");
            qmodel->setHeaderData(4, Qt::Horizontal, "Поствшик");
            qmodel->setHeaderData(5, Qt::Horizontal, "Адрес поставшика");
            qmodel->setHeaderData(6, Qt::Horizontal, "Телефон поставщика");
            table->setModel(qmodel);
            table->setColumnWidth(0, 140);
            stackedWidget->setCurrentIndex(1);
        }
    }
}

void QueryModel::execQuery4()
{
   bool ok;
   QDate firstDate = inputDate("Начало периода", ok);
   if (ok && firstDate.isValid()) {
       QDate secondDate = inputDate("Окончание периода", ok);
       if (ok && secondDate.isValid()) {
           QSqlQueryModel *qmodel = new QSqlQueryModel(this);
           QString query = Querys::sumPeriod.arg(firstDate.toString("dd.MM.yyyy"))
                   .arg(secondDate.toString("dd.MM.yyyy"));
           qmodel->setQuery(query, model->database());
           qmodel->setHeaderData(0, Qt::Horizontal, "Тип оплаты");
           qmodel->setHeaderData(1, Qt::Horizontal, "Сумма за период продаж");
           qmodel->setHeaderData(2, Qt::Horizontal, "Количество продаж");
           table->setModel(qmodel);
           table->setColumnWidth(0, 160);
           table->setColumnWidth(1, 160);
           table->setColumnWidth(2, 160);
           stackedWidget->setCurrentIndex(1);
       }
   }
}

void QueryModel::execQuery5()
{
    QSqlQueryModel *qmodel = new QSqlQueryModel(this);
    qmodel->setQuery(Querys::goodsInStock, model->database());
    qmodel->setHeaderData(0, Qt::Horizontal, "Наименование");
    qmodel->setHeaderData(1, Qt::Horizontal, "Количество");
    qmodel->setHeaderData(2, Qt::Horizontal, "Цена");
    table->setModel(qmodel);
    table->setColumnWidth(0, 160);
    table->setColumnWidth(1, 160);
    table->setColumnWidth(2, 160);
    stackedWidget->setCurrentIndex(1);
}

//-------------------------

ReportModel::ReportModel(QTableView *table, QStackedWidget *stackedWidget,
                         QSqlDatabase *db, QObject *parent):
    AbstractApplicationModel(table, stackedWidget, db, parent)
{
    listReports.append(&ReportModel::showReport1);
    listReports.append(&ReportModel::showReport2);
    listReports.append(&ReportModel::showReport3);
    listReports.append(&ReportModel::showReport4);
    listReports.append(&ReportModel::showReport5);
}

QSqlRelationalTableModel* ReportModel::viewAdapter(QWidget *widgetPanel)
{
    table->setModel(model);
    stackedWidget->setCurrentIndex(4);
    widgetPanel->hide();
    return model;
}

void ReportModel::showReport(int index, const QString& param)
{
    (this->*listReports[index])();
}

void ReportModel::showReport1()
{
    QSqlQueryModel *qmodel = new QSqlQueryModel(this);
    qmodel->setQuery(Querys::employeeList, model->database());
    report->dataManager()->addModel("master", qmodel, true);
    if(report->loadFromFile(Reports::employeeList)){
        report->previewReport();
        //report->designReport();
    }
}

void ReportModel::showReport2()
{
    QSqlQueryModel *qmodel = new QSqlQueryModel(this);
    qmodel->setQuery(Querys::priceList, model->database());
    report->dataManager()->addModel("master", qmodel, true);
    if(report->loadFromFile(Reports::priceList)){
        report->previewReport();
        //report->designReport();
    }
}

void ReportModel::showReport3()
{
    QSqlQueryModel *qmodel = new QSqlQueryModel(this);
    qmodel->setQuery(Querys::providersList, model->database());
    report->dataManager()->addModel("master", qmodel, true);
    if(report->loadFromFile(Reports::providersList)){
        report->previewReport();
        //report->designReport();
    }
}

void ReportModel::showReport4()
{
    bool ok;
    QDate firstDate = inputDate("Начало периода", ok);
    if (ok && firstDate.isValid()) {
        QDate secondDate = inputDate("Окончание периода", ok);
        if (ok && secondDate.isValid()) {
            QSqlQueryModel *qmodel = new QSqlQueryModel(this);
            QString query = Querys::countSelling.arg(firstDate.toString("dd.MM.yyyy"))
                    .arg(secondDate.toString("dd.MM.yyyy"));
            qmodel->setQuery(query, model->database());
            report->dataManager()->addModel("master", qmodel, true);
            if(report->loadFromFile(Reports::countSelling)) {
                report->dataManager()->setReportVariable("firstDate", firstDate.toString("dd.MM.yyyy"));
                report->dataManager()->setReportVariable("secondDate", secondDate.toString("dd.MM.yyyy"));
                report->previewReport();
                //report->designReport();
            }
        }
    }
}

void ReportModel::showReport5()
{
    bool ok;
    QDate firstDate = inputDate("Начало периода", ok);
    if (ok && firstDate.isValid()) {
        QDate secondDate = inputDate("Окончание периода", ok);
        if (ok && secondDate.isValid()) {
            QSqlQueryModel *qmodel = new QSqlQueryModel(this);
            QString query = Querys::deliveryPeriod.arg(firstDate.toString("dd.MM.yyyy"))
                    .arg(secondDate.toString("dd.MM.yyyy"));
            qmodel->setQuery(query, model->database());
            report->dataManager()->addModel("master", qmodel, true);
            if(report->loadFromFile(Reports::deliveryPeriod)) {
                report->dataManager()->setReportVariable("firstDate", firstDate.toString("dd.MM.yyyy"));
                report->dataManager()->setReportVariable("secondDate", secondDate.toString("dd.MM.yyyy"));
                report->previewReport();
                //report->designReport();
            }
        }
    }
}

//-------------------------

namespace CreateModel {

    void deleteObject(AbstractApplicationModel *object) {
        object->deleteLater();
    }

    QSharedPointer<AbstractApplicationModel> creatDatabaseModel(QTableView *table, QStackedWidget *stackedWidget, QSqlDatabase *db, QObject *parent)
        { return QSharedPointer<DatabaseModel>(new DatabaseModel(table, stackedWidget, db, parent), deleteObject); }

    QSharedPointer<AbstractApplicationModel> creatProviderModel(QTableView *table, QStackedWidget *stackedWidget, QSqlDatabase *db, QObject *parent)
        { return QSharedPointer<ProviderModel>(new ProviderModel(table, stackedWidget, db, parent), deleteObject); }

    QSharedPointer<AbstractApplicationModel> creatEquipmentModel(QTableView *table, QStackedWidget *stackedWidget, QSqlDatabase *db, QObject *parent)
        { return QSharedPointer<EquipmentModel>(new EquipmentModel(table, stackedWidget, db, parent), deleteObject); }

    QSharedPointer<AbstractApplicationModel> creatEmployeeModel(QTableView *table, QStackedWidget *stackedWidget, QSqlDatabase *db, QObject *parent)
        { return QSharedPointer<EmployeeModel>(new EmployeeModel(table, stackedWidget, db, parent), deleteObject); }

    QSharedPointer<AbstractApplicationModel> creatSellingModel(QTableView *table, QStackedWidget *stackedWidget, QSqlDatabase *db, QObject *parent)
        { return QSharedPointer<SellingModel>(new SellingModel(table, stackedWidget, db, parent), deleteObject); }

    QSharedPointer<AbstractApplicationModel> creatReferenceModel(QTableView *table, QStackedWidget *stackedWidget, QSqlDatabase *db, QObject *parent)
        { return QSharedPointer<ReferenceModel>(new ReferenceModel(table, stackedWidget, db, parent), deleteObject); }

    QSharedPointer<AbstractApplicationModel> creatPaytypeModel(QTableView *table, QStackedWidget *stackedWidget, QSqlDatabase *db, QObject *parent)
        { return QSharedPointer<PaytypeModel>(new PaytypeModel(table, stackedWidget, db, parent), deleteObject); }

    QSharedPointer<AbstractApplicationModel> creatQueryModel(QTableView *table, QStackedWidget *stackedWidget, QSqlDatabase *db, QObject *parent)
        { return QSharedPointer<QueryModel>(new QueryModel(table, stackedWidget, db, parent), deleteObject); }

    QSharedPointer<AbstractApplicationModel> creatReportModel(QTableView *table, QStackedWidget *stackedWidget, QSqlDatabase *db, QObject *parent)
        { return QSharedPointer<ReportModel>(new ReportModel(table, stackedWidget, db, parent), deleteObject); }


    const ObjectCalling CallingMenu[] = { creatDatabaseModel,
                                          creatProviderModel,
                                          creatEquipmentModel,
                                          creatSellingModel,
                                          creatReferenceModel,
                                          creatEmployeeModel,
                                          creatPaytypeModel,
                                          creatQueryModel,
                                          creatReportModel };
}






