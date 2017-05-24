#ifndef SIMPLEINPUTDIALOG_H
#define SIMPLEINPUTDIALOG_H

#include <QDialog>
#include <QModelIndex>

class QDataWidgetMapper;
class QSqlRelationalTableModel;

namespace Ui {
class SimpleInputDialog;
}

class SimpleInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SimpleInputDialog(QSqlRelationalTableModel *model, QWidget *parent = 0);
    ~SimpleInputDialog();

public slots:

    int exec(const QModelIndex &index);
    int exec() Q_DECL_OVERRIDE;
    void clearWindow();
    void submit();
    void refresh();

private:
    Ui::SimpleInputDialog *ui;
    QDataWidgetMapper *mapper;
    QSqlRelationalTableModel *model;
    QModelIndex modelIndex;
    int currentRow;
    bool windowType;
};

#endif // SIMPLEINPUTDIALOG_H
