#ifndef SELLINGINPUTDIALOG_H
#define SELLINGINPUTDIALOG_H

#include <QDialog>
#include <QModelIndex>

namespace Ui {
class SellingInputDialog;
}

class QSqlRelationalTableModel;
class QDataWidgetMapper;
class QComboBox;

class SellingInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SellingInputDialog(QSqlRelationalTableModel *model, QWidget *parent = 0);
    ~SellingInputDialog();

public slots:

    int exec(const QModelIndex &index);
    int exec() Q_DECL_OVERRIDE;
    void clearWindow();
    void submit();
    void refresh();

private:

    void manualMapping();
    int relationIndex(QComboBox *widget);

private:
    Ui::SellingInputDialog *ui;
    QSqlRelationalTableModel *model;
    QDataWidgetMapper *mapper;
    QModelIndex modelIndex;
    int currentRow;
};

#endif // SELLINGINPUTDIALOG_H
