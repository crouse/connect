#ifndef CONNECT_H
#define CONNECT_H

#include <QString>
#include <QDebug>
#include <QtSql>
#include <QMainWindow>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QCloseEvent>
#include <QtNetwork>
#include <QLabel>

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
    bool validate_input_values();
    bool update_sqlite_database();
    bool clear_lineEdits();
    bool complete_fields(QString name, QString value);
    void append_items2_tableView();
    void init_and_append_items2_tableView();
private:
    QLabel *status_label;
    QSqlDatabase db;
    QString local_ip;
    QString server_ip;
    void get_local_ip();
    void save_excel(QString fileName);

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_lineEditReceipt_editingFinished();

    void on_pushButtonOK_clicked();

    void on_pushButtonClear_clicked();

    void on_pushButtonExport_clicked();

    void on_pushButton_clicked();

    void on_action_triggered();

    void on_actionThanks_triggered();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::Connect *ui;
};

#endif // CONNECT_H
