#ifndef CONNECT_H
#define CONNECT_H

#include <QString>
#include <QDebug>
#include <QtSql>
#include <QMainWindow>
#include <QMessageBox>
#include <QStandardItemModel>

namespace Ui {
class Connect;
}

class Connect : public QMainWindow
{
    Q_OBJECT

public:
    explicit Connect(QWidget *parent = 0);
    ~Connect();
    QStandardItemModel *viewModel;
    bool check_lineEdit_items();
    bool update_items();
    bool validate_input_values();
    bool update_sqlite_database();
    bool clear_lineEdits();
    bool complete_fields(QString name, QString value);
    void append_items2_tableView();
private:
    QSqlDatabase db;

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_lineEditReceipt_editingFinished();

    void on_pushButtonOK_clicked();

    void on_pushButtonNext_clicked();

    void on_pushButtonPrevious_clicked();

    void on_pushButtonClear_clicked();

private:
    Ui::Connect *ui;
};

#endif // CONNECT_H
