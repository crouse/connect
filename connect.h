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
    QStandardItemModel *viewModel_search;
    bool check_lineEdit_items();
    bool validate_input_values();
    bool update_database();
    bool clear_lineEdits();
    bool complete_fields(QString name, QString value);
    void append_items2_tableView();
    bool init_and_append_items2_tableView();
    bool init_db();
    bool db_port_test();
    bool modify_or_not();
    void hide_search_table();
    void hide_menu_and_button();
    void show_search_table();
    bool test_if_connected();
    void set_old_model_view();
    void set_new_model_view();
    bool create_table();
    bool admin_init_all();
    bool append_model_data(int c, QString where);
private:
    QLabel *status_label;
    QSqlDatabase db;
    QString local_ip;
    QString server_ip;
    int dbid;
    int if_query_is_set;
    bool if_connected;
    void get_local_ip();
    void save_excel(QString fileName);
    void excel(QString fileName);

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_lineEditReceipt_editingFinished();

    void on_pushButtonOK_clicked();

    void on_pushButtonExport_clicked();

    void on_pushButton_clicked();

    void on_pushButtonDatabase_pressed();

    void on_action_triggered();

    void on_actionQueryAnyThing_triggered();

    void on_tableView_2_doubleClicked(const QModelIndex &index);

    void on_actionDbBack_triggered();

    void on_actionJoinin_triggered();

    void on_lineEdit_Name_returnPressed();

    void on_lineEdit_Name_editingFinished();

    void on_lineEditCode_editingFinished();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::Connect *ui;
};

#endif // CONNECT_H
