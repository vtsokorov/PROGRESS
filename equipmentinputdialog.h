#ifndef EQUIPMENTINPUTDIALOG_H
#define EQUIPMENTINPUTDIALOG_H

#include <QDialog>
#include <QModelIndex>

namespace Ui {
class EquipmentInputDialog;
}


class QSqlRelationalTableModel;
class QDataWidgetMapper;
class QComboBox;

class EquipmentInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EquipmentInputDialog(QSqlRelationalTableModel *model, QWidget *parent = 0);
    ~EquipmentInputDialog();

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
    Ui::EquipmentInputDialog *ui;
    QSqlRelationalTableModel *model;
    QDataWidgetMapper *mapper;
    QModelIndex modelIndex;
    int currentRow;
};

#endif // EQUIPMENTINPUTDIALOG_H
