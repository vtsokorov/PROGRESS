#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

namespace Ui {
class MainWindow;
}

class AbstractApplicationModel;
class QSqlRelationalTableModel;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

    void show();

private slots:

    void selectedItem(const QModelIndex &index);
    void selectedQuery(const QModelIndex &index);
    void selectedReports(const QModelIndex &index);
    void openEditDialog(const QModelIndex &index);

    void filterOn();
    void filterOff();

    void editRecord();
    void addRecord();
    void deleteRecord();
    void refresh();

private:

    void setupWidget();

private:

    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSharedPointer<AbstractApplicationModel> item;
    QSqlRelationalTableModel *model;

    QLabel *lwUser;
    QLabel *lwHost;
    QLabel *lwBase;
    QLabel *lwPort;
};

#endif // MAINWINDOW_H
