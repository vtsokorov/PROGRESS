#ifndef APPLICATIONMODEL_H
#define APPLICATIONMODEL_H

#include <QObject>
#include <QModelIndex>

class QSqlRelationalTableModel;
class QStackedWidget;
class QTableView;
class QSqlDatabase;
class QLineEdit;
class QProgressDialog;

namespace LimeReport {
class ReportEngine;
}

class AbstractApplicationModel : public QObject
{
    Q_OBJECT

public:

    AbstractApplicationModel(QTableView *table, QStackedWidget *stackedWidget,
                             QSqlDatabase *db, QObject *parent);
    virtual ~AbstractApplicationModel();
    virtual QSqlRelationalTableModel* viewAdapter(QWidget *widgetPanel) = 0;
    virtual void showDialog(const QModelIndex& index = QModelIndex(), QWidget *parent = 0);
    virtual void showReport(int index = 0, const QString& param = QString());
    virtual void execQuery(int index = 0, const QString& query = QString());
    virtual void setFilter(const QString& filer = QString());
    virtual void setPlaceHolderText(QLineEdit *widget);
    virtual void deleteRecord();
    virtual void refresh();

    void clearFilter();

protected:

    void showAllColumn(int columnsSize = 8);
    void selectTable(const QString &tableName);

    QDate inputDate(const QString &title, bool &isOk);
    QString inputData(const QString &title, bool &isOk);
    QString selectData(const QString &title, const QStringList &items, bool &isOk);

protected:

    QSqlRelationalTableModel *model;
    QTableView *table;
    QStackedWidget *stackedWidget;

    LimeReport::ReportEngine *report;
    QProgressDialog* m_progressDialog;
    int m_currentPage;

private slots:

    void renderStarted();
    void renderPageFinished(int renderedPageCount);
    void renderFinished();
};

class DatabaseModel : public AbstractApplicationModel
{
    Q_OBJECT

public:

    DatabaseModel(QTableView *table, QStackedWidget *stackedWidget,
                             QSqlDatabase *db, QObject *parent);
    QSqlRelationalTableModel* viewAdapter(QWidget *widgetPanel) override;
};

class ReferenceModel : public AbstractApplicationModel
{
    Q_OBJECT

public:

    ReferenceModel(QTableView *table, QStackedWidget *stackedWidget,
                             QSqlDatabase *db, QObject *parent);
    QSqlRelationalTableModel* viewAdapter(QWidget *widgetPanel) override;
};

class ProviderModel : public AbstractApplicationModel
{
    Q_OBJECT

public:

    ProviderModel(QTableView *table, QStackedWidget *stackedWidget,
                             QSqlDatabase *db, QObject *parent);
    ~ProviderModel() {    }
    QSqlRelationalTableModel* viewAdapter(QWidget *widgetPanel) override;
    void showDialog(const QModelIndex& index = QModelIndex(), QWidget *parent = 0) override;
    void setFilter(const QString& filer = QString()) override;
    void setPlaceHolderText(QLineEdit *widget) override;
};

class EquipmentModel : public AbstractApplicationModel
{
    Q_OBJECT

public:

    EquipmentModel(QTableView *table, QStackedWidget *stackedWidget,
                             QSqlDatabase *db, QObject *parent);
    ~EquipmentModel() {    }
    QSqlRelationalTableModel* viewAdapter(QWidget *widgetPanel) override;
    void showDialog(const QModelIndex& index = QModelIndex(), QWidget *parent = 0) override;
    void setFilter(const QString& filer = QString()) override;
    void setPlaceHolderText(QLineEdit *widget) override;
};

class EmployeeModel : public AbstractApplicationModel
{
    Q_OBJECT

public:

    EmployeeModel(QTableView *table, QStackedWidget *stackedWidget,
                             QSqlDatabase *db, QObject *parent);
    ~EmployeeModel() {    }
    QSqlRelationalTableModel* viewAdapter(QWidget *widgetPanel) override;
    void showDialog(const QModelIndex& index = QModelIndex(), QWidget *parent = 0) override;
    void setFilter(const QString& filer = QString()) override;
    void setPlaceHolderText(QLineEdit *widget) override;
};

class SellingModel : public AbstractApplicationModel
{
    Q_OBJECT

public:

    SellingModel(QTableView *table, QStackedWidget *stackedWidget,
                             QSqlDatabase *db, QObject *parent);
    ~SellingModel() {    }
    QSqlRelationalTableModel* viewAdapter(QWidget *widgetPanel) override;
    void showDialog(const QModelIndex& index = QModelIndex(), QWidget *parent = 0) override;
    void setFilter(const QString& filer = QString()) override;
    void setPlaceHolderText(QLineEdit *widget) override;
};

class PaytypeModel : public AbstractApplicationModel
{
    Q_OBJECT

public:

    PaytypeModel(QTableView *table, QStackedWidget *stackedWidget,
                             QSqlDatabase *db, QObject *parent);
    ~PaytypeModel() {    }
    QSqlRelationalTableModel* viewAdapter(QWidget *widgetPanel) override;
    void showDialog(const QModelIndex& index = QModelIndex(), QWidget *parent = 0) override;
    void setFilter(const QString& filer = QString()) override;
    void setPlaceHolderText(QLineEdit *widget) override;
};

class QueryModel : public AbstractApplicationModel
{
    Q_OBJECT

public:

    QueryModel(QTableView *table, QStackedWidget *stackedWidget,
                             QSqlDatabase *db, QObject *parent);
    ~QueryModel() {    }
    QSqlRelationalTableModel* viewAdapter(QWidget *widgetPanel) override;
    void execQuery(int index = 0, const QString& query = QString()) override;
    void setPlaceHolderText(QLineEdit *widget) override;

private:

    typedef void (QueryModel::*ExecQuery)();
    QList<ExecQuery> listQuery;

private:

    void execQuery1();
    void execQuery2();
    void execQuery3();
    void execQuery4();
    void execQuery5();
};

class ReportModel : public AbstractApplicationModel
{
    Q_OBJECT

public:

    ReportModel(QTableView *table, QStackedWidget *stackedWidget,
                             QSqlDatabase *db, QObject *parent);
    ~ReportModel() {    }
    QSqlRelationalTableModel* viewAdapter(QWidget *widgetPanel) override;
    void showReport(int index = 0, const QString& param = QString()) override;

private:

    typedef void (ReportModel::*ExecReport)();
    QList<ExecReport> listReports;

private:

    void showReport1();
    void showReport2();
    void showReport3();
    void showReport4();
    void showReport5();

};

namespace CreateModel {

    typedef QSharedPointer<AbstractApplicationModel> (*ObjectCalling)
                (QTableView *table, QStackedWidget *stackedWidget, QSqlDatabase *db, QObject *parent);
    extern const ObjectCalling CallingMenu[];

}



#endif // APPLICATIONMODEL_H
