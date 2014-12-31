#include "connect.h"
#include "ui_connect.h"
#include <QMenu>
#include <QValidator>
#include <QStringList>
#include <QCompleter>

Connect::Connect(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Connect)
{
    ui->setupUi(this);
    //setStyleSheet("background-color:white");

    //completor
    QStringList family_list;
    family_list << "父亲" << "母亲" << "兄弟" << "姐妹";
    QCompleter *family_completer = new QCompleter(family_list, this);
    family_completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEditBuddhistDisciplesOfFamily->setCompleter(family_completer);

    viewModel = new QStandardItemModel();
    viewModel->setHorizontalHeaderItem(0, new QStandardItem(QObject::trUtf8("姓名")));
    viewModel->setHorizontalHeaderItem(1, new QStandardItem(QObject::trUtf8("手机")));
    viewModel->setHorizontalHeaderItem(2, new QStandardItem(QObject::trUtf8("收据编号")));
    ui->tableView->setModel(viewModel);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    viewModel->setItem(0, 0, new QStandardItem("屈庆磊"));
    viewModel->setItem(0, 1, new QStandardItem("18201620963"));

    viewModel->setItem(1, 0, new QStandardItem("小明"));
    viewModel->setItem(1, 1, new QStandardItem("17201620963"));

    /* set validators */
    QRegExp regExpGender("^[\u7537\u5973]+$"); // 性别只允许输入男或者女
    ui->lineEditGender->setValidator(new QRegExpValidator(regExpGender, this));

    QRegExp regExpPhoneNum("1[3|5|7|8|][0-9]{9}"); // 判断输入是否为手机号，只允许输入正常手机号
    ui->lineEditPhoneNum->setValidator(new QRegExpValidator(regExpPhoneNum, this));

    QRegExp regExpTel("\\d{3}-\\d{8}|\\d{4}-\\d{7}");
    ui->lineEditTelphoneNum->setValidator(new QRegExpValidator(regExpTel, this));

    QRegExp regExpBirthday("^(19|20)\\d{2}.(1[0-2]|0?[1-9]).(0?[1-9]|[1-2][0-9]|3[0-1])$");
    ui->lineEditBirthday->setValidator(new QRegExpValidator(regExpBirthday, this));

    QRegExp regExpEditTime("^(19|20)\\d{2}.(1[0-2]|0?[1-9]).(0?[1-9]|[1-2][0-9]|3[0-1])$");
    ui->lineEditTime->setValidator(new QRegExpValidator(regExpEditTime, this));

    QRegExp regExpGraduateTime("^(19|20)\\d{2}.(1[0-2]|0?[1-9]).(0?[1-9]|[1-2][0-9]|3[0-1])$");
    ui->lineEditGraduateTime->setValidator(new QRegExpValidator(regExpGraduateTime, this));

    QRegExp regExpFirstJobDate("^(19|20)\\d{2}.(1[0-2]|0?[1-9]).(0?[1-9]|[1-2][0-9]|3[0-1])$");
    ui->lineEditFirstJobDate->setValidator(new QRegExpValidator(regExpFirstJobDate, this));

    QRegExp regExpSecondJobDate("^(19|20)\\d{2}.(1[0-2]|0?[1-9]).(0?[1-9]|[1-2][0-9]|3[0-1])$");
    ui->lineEditSecondJobDate->setValidator(new QRegExpValidator(regExpSecondJobDate, this));

    QRegExp regExpRetirementDate("^(19|20)\\d{2}.(1[0-2]|0?[1-9]).(0?[1-9]|[1-2][0-9]|3[0-1])$");
    ui->lineEditRetirementDate->setValidator(new QRegExpValidator(regExpRetirementDate, this));

    QRegExp regExpID("^[1-9]\\d{5}[1-9]\\d{3}((0\\d)|(1[0-2]))(([0|1|2]\\d)|3[0-1])((\\d{4})|\\d{3}[Xx])$");
    ui->lineEditID->setValidator(new QRegExpValidator(regExpID, this));

    QRegExp regExpYearsOfLearningBuhddhism("^[1-9]\\d?$");
    ui->lineEditYearsOfLearningBuddhism->setValidator(new QRegExpValidator(regExpYearsOfLearningBuhddhism, this));

    QRegExp regExpPostcode("^[1-9][0-9]{5}$");
    ui->lineEditPostcode->setValidator(new QRegExpValidator(regExpPostcode, this));

    // database setting
    db = QSqlDatabase::addDatabase("QSQLITE");
    //db.setDatabaseName(":memory"); // in memory db
    db.setDatabaseName("/Users/quqinglei/Desktop/memory.db");
    db.open();

    /*
    // mysql
    QCoreApplication::addLibraryPath("/Users/quqinglei/Qt5.4.0/5.4/clang_64/plugins/sqldrivers");
    QCoreApplication::addLibraryPath("/opt/local/lib");
    QCoreApplication::addLibraryPath("/usr/local/lib");
    QCoreApplication::addLibraryPath("/usr/lib");
    QCoreApplication::addLibraryPath("/opt/local/lib/mysql55/mysql/");

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("10.0.0.18");
    db.setDatabaseName("connect");
    db.setUserName("connect");
    db.setPassword("connect");
    db.open();
    qDebug() << QCoreApplication::libraryPaths();
    */

    QSqlQuery query;
    QString createTableSql = "                             \
            create table if not exists people (            \
            name                             varchar(32),  \
            gender                           varchar(10),  \
            job                              varchar(32),  \
            hobby                            varchar(64),  \
            fname                            varchar(32),  \
            birthday                         varchar(32),  \
            personnel_id                     varchar(32),  \
            phone_num                        varchar(20),  \
            race                             varchar(20),  \
            degree                           varchar(32),  \
            health                           varchar(32),  \
            telphone_num                     varchar(20),  \
            edit_time                        varchar(32),  \
            receipt                          varchar(32),  \
            workplace                        varchar(64),  \
            province                         varchar(64),  \
            city                             varchar(64),  \
            district                         varchar(64),  \
            address                          varchar(64),  \
            postcode                         varchar(64),  \
            graduate_time                    varchar(32),  \
            graduate_school                  varchar(64),  \
            first_job_entry_time             varchar(64),  \
            first_job_workplace              varchar(64),  \
            second_job_entry_time            varchar(32),  \
            second_job_workplace             varchar(64),  \
            retirement_date                  varchar(32),  \
            retirement_workplace             varchar(64),  \
            year2start_learning_buddhism     varchar(32),  \
            years_of_learning_buddhism       integer,      \
            deep_understanding_of_dharma     varchar(64),  \
            reason2learning_dharma           varchar(128), \
            nums_of_buddhism_book            integer,      \
            easy2learn_buddhism_book         varchar(128), \
            hard2read                        varchar(128), \
            maxim                            varchar(128), \
            buddhist_disciples_of_family     varchar(128), \
            editor                           varchar(128), \
            others                           varchar(128)  \
            );";
    query.exec(createTableSql);
}

Connect::~Connect()
{
    delete ui;
}

bool Connect::update_items()
{
    return true;
}

void Connect::on_tableView_doubleClicked(const QModelIndex &index)
{
    switch(index.column()) {
    case 0:
        ui->lineEditName->setText(index.data().toString());
        break;
    case 1:
        ui->lineEditPhoneNum->setText(index.data().toString());
        break;
    case 2:
        ui->lineEditID->setText(index.data().toString());
        break;
    }
}

void Connect::on_lineEditReceipt_editingFinished()
{
   ui->lineEditAddress->setText("helloWorld");
   complete_fields("receipt", ui->lineEditReceipt->text());
   qDebug() << "one_lineEditReceipt_editingFinished";
}

bool Connect::check_lineEdit_items()
{
    bool name = ui->lineEditName->text().isEmpty();
    bool receipt = ui->lineEditReceipt->text().isEmpty();

    if (name || receipt) return false;
    else return true;
}

void Connect::on_pushButtonOK_clicked()
{
    if (!check_lineEdit_items()) {
        QMessageBox::information(this, "", "请至少填写姓名、收据编号");
        return;
    }

    validate_input_values();
    update_sqlite_database();
    clear_lineEdits();
}

void Connect::on_pushButtonNext_clicked()
{
   qDebug() << "on_pushButtonNext_clicked()";
}

void Connect::on_pushButtonPrevious_clicked()
{
   qDebug() << "on_pushButtonPrevious_clicked()";
}


/* user defined functions */
bool Connect::validate_input_values()
{
    //[1] valid ID card
    if (ui->lineEditID->text().isEmpty()) {
        qDebug() << "ID card is empty";
    } else {
        if (ui->lineEditID->text().length() != 18) {
            QMessageBox::information(this, "", "身份证号必须是18位得组合");
            /* 如果身份证输入错误，提交的时候会弹出对话框，确定后会把焦点转移到身份证输入框 */
            ui->lineEditID->setFocus();
        }
    }

    //[2] valid phone num 手机号必须位数为11位数字，正则无法检测输入比这个短
    if (ui->lineEditPhoneNum->text().isEmpty()) {
        qDebug() << "phone num is empty";
    } else {
        if (ui->lineEditPhoneNum->text().length() != 11) {
            QMessageBox::information(this, "", "手机号必须是11位");
            ui->lineEditPhoneNum->setFocus();
        }
    }

    return true;
}

bool Connect::update_sqlite_database()
{
    QSqlQuery query;
    // 如果存在一行一模一样的，先删除
    QString deleteDuplicateSql = "delete from people where receipt = "
            + QString("'") + ui->lineEditReceipt->text() + QString("'");

    qDebug() << deleteDuplicateSql;
    query.exec(deleteDuplicateSql);
    query.prepare("INSERT INTO people (             \
                  name                             ,\
                  gender                           ,\
                  job                              ,\
                  hobby                            ,\
                  fname                            ,\
                  birthday                         ,\
                  personnel_id                     ,\
                  phone_num                        ,\
                  race                             ,\
                  degree                           ,\
                  health                           ,\
                  telphone_num                     ,\
                  edit_time                        ,\
                  receipt                          ,\
                  workplace                        ,\
                  province                         ,\
                  city                             ,\
                  district                         ,\
                  address                          ,\
                  postcode                         ,\
                  graduate_time                    ,\
                  graduate_school                  ,\
                  first_job_entry_time             ,\
                  first_job_workplace              ,\
                  second_job_entry_time            ,\
                  second_job_workplace             ,\
                  retirement_date                  ,\
                  retirement_workplace             ,\
                  year2start_learning_buddhism     ,\
                  years_of_learning_buddhism       ,\
                  deep_understanding_of_dharma     ,\
                  reason2learning_dharma           ,\
                  nums_of_buddhism_book            ,\
                  easy2learn_buddhism_book         ,\
                  hard2read                        ,\
                  maxim                            ,\
                  buddhist_disciples_of_family     ,\
                  editor                           ,\
                  others                            \
                  ) VALUES (                        \
                  :name                            ,\
                  :gender                          ,\
                  :job                             ,\
                  :hobby                           ,\
                  :fname                           ,\
                  :birthday                        ,\
                  :personnel_id                    ,\
                  :phone_num                       ,\
                  :race                            ,\
                  :degree                          ,\
                  :health                          ,\
                  :telphone_num                    ,\
                  :edit_time                       ,\
                  :receipt                         ,\
                  :workplace                       ,\
                  :province                        ,\
                  :city                            ,\
                  :district                        ,\
                  :address                         ,\
                  :postcode                        ,\
                  :graduate_time                   ,\
                  :graduate_school                 ,\
                  :first_job_entry_time            ,\
                  :first_job_workplace             ,\
                  :second_job_entry_time           ,\
                  :second_job_workplace            ,\
                  :retirement_date                 ,\
                  :retirement_workplace            ,\
                  :year2start_learning_buddhism    ,\
                  :years_of_learning_buddhism      ,\
                  :deep_understanding_of_dharma    ,\
                  :reason2learning_dharma          ,\
                  :nums_of_buddhism_book           ,\
                  :easy2learn_buddhism_book        ,\
                  :hard2read                       ,\
                  :maxim                           ,\
                  :buddhist_disciples_of_family    ,\
                  :editor                          ,\
                  :others                          )"
    );

    query.bindValue(":name", ui->lineEditName->text());
    query.bindValue(":gender", ui->lineEditGender->text());
    query.bindValue(":job", ui->lineEditJob->text());
    query.bindValue(":hobby", ui->lineEditHobby->text());
    query.bindValue(":fname", ui->lineEditFName->text());
    query.bindValue(":birthday", ui->lineEditBirthday->text());
    query.bindValue(":personel_id", ui->lineEditID->text());
    query.bindValue(":phone_num", ui->lineEditPhoneNum->text());
    query.bindValue(":race", ui->lineEditRace->text());
    query.bindValue(":degree", ui->lineEditDegree->text());
    query.bindValue(":health", ui->lineEditHealth->text());
    query.bindValue(":telephone_num", ui->lineEditTelphoneNum->text());
    query.bindValue(":edit_time", ui->lineEditTime->text());
    query.bindValue(":receipt", ui->lineEditReceipt->text());
    query.bindValue(":workplace", ui->lineEditWorkPlace->text());
    query.bindValue(":province", ui->lineEditProvince->text());
    query.bindValue(":city", ui->lineEditCity->text());
    query.bindValue(":district", ui->lineEditDistrict->text());
    query.bindValue(":address", ui->lineEditAddress->text());
    query.bindValue(":postcode", ui->lineEditPostcode->text());
    query.bindValue(":graduate_time", ui->lineEditGraduateTime->text());
    query.bindValue(":graduate_school", ui->lineEditGraduateSchool->text());
    query.bindValue(":first_job_entry_time", ui->lineEditFirstJobDate->text());
    query.bindValue(":first_job_worksplace", ui->lineEditFirstJobWorkspace->text());
    query.bindValue(":second_job_entry_time", ui->lineEditSecondJobDate->text());
    query.bindValue(":second_job_workplace", ui->lineEditSecondJobWorkspace->text());
    query.bindValue(":retirement_date", ui->lineEditRetirementDate->text());
    query.bindValue(":retirement_workplace", ui->lineEditRetirementWorkspace->text());
    query.bindValue(":years2start_learning_buddhism", ui->lineEditLearnBuddhismDate->text());
    query.bindValue(":years_of_learning_buhddhism", ui->lineEditYearsOfLearningBuddhism->text());
    query.bindValue(":deep_understanding_of_dharma", ui->lineEditBuddhismLevel->text());
    query.bindValue(":reason2learning_dharma", ui->lineEditReasonToLearnDharma->text());
    query.bindValue(":nums_of_buddhism_book", ui->lineEditReadNumsOfBuddhismBook->text());
    query.bindValue(":easy2learn_buddhism_book", ui->lineEditEasyToLearnBuddhismBook->text());
    query.bindValue(":hard2read", ui->lineEditHardToLearnBuddhismBook->text());
    query.bindValue(":maxim", ui->lineEditMaximOfBuddhism->text());
    query.bindValue(":buddhist_disciples_of_family", ui->lineEditBuddhistDisciplesOfFamily->text());
    query.bindValue(":editor", ui->lineEditor->text());
    query.bindValue(":others", ui->lineEditOthers->text());

    bool rt = query.exec();
    if (!rt) {
        qDebug() << "rt:" << rt << query.lastError().text();
        // 输出错误到屏幕！[tbd] 这个得考虑到输入重复问题，下面的错误用户未必理解，所以得考虑到
        QMessageBox::information(this, "", query.lastError().text());
    }

    return true;
}

bool Connect::clear_lineEdits()
{
    ui->lineEditName->clear();
    //ui->lineEditGender->clear();
    ui->lineEditJob->clear();
    ui->lineEditHobby->clear();
    ui->lineEditFName->clear();
    ui->lineEditBirthday->clear();
    ui->lineEditID->clear();
    ui->lineEditPhoneNum->clear();
    ui->lineEditDegree->clear();
    ui->lineEditHealth->clear();
    ui->lineEditTelphoneNum->clear();
    ui->lineEditTime->clear();
    ui->lineEditReceipt->clear();
    ui->lineEditWorkPlace->clear();
    ui->lineEditDistrict->clear();
    ui->lineEditAddress->clear();
    ui->lineEditPostcode->clear();
    ui->lineEditGraduateTime->clear();
    ui->lineEditGraduateSchool->clear();
    ui->lineEditFirstJobDate->clear();
    ui->lineEditFirstJobWorkspace->clear();
    ui->lineEditSecondJobDate->clear();
    ui->lineEditSecondJobWorkspace->clear();
    ui->lineEditRetirementDate->clear();
    ui->lineEditRetirementWorkspace->clear();
    ui->lineEditLearnBuddhismDate->clear();
    ui->lineEditYearsOfLearningBuddhism->clear();
    ui->lineEditBuddhismLevel->clear();
    ui->lineEditReasonToLearnDharma->clear();
    ui->lineEditReadNumsOfBuddhismBook->clear();
    ui->lineEditEasyToLearnBuddhismBook->clear();
    ui->lineEditHardToLearnBuddhismBook->clear();
    ui->lineEditMaximOfBuddhism->clear();
    ui->lineEditBuddhistDisciplesOfFamily->clear();
    ui->lineEditOthers->clear();
    return true;
}


bool Connect::complete_fields(QString name, QString value)
{
    // 1. 从本地数据库查询是否有此记录，如果有记录，补全所有选项
    // 2. 从服务器数据库查询是否有记录，如果有记录，补全所有选项
    qDebug() << name << value;
    QSqlQuery query;
    QString sql = QString(
            "select \
            name,\
            gender,\
            job,\
            hobby,\
            fname,\
            birthday,\
            personnel_id,\
            phone_num,\
            race,\
            degree,\
            health,\
            telphone_num,\
            edit_time,\
            receipt,\
            workplace,\
            province,\
            city,\
            district,\
            address,\
            postcode,\
            graduate_time,\
            graduate_school,\
            first_job_entry_time,\
            first_job_workplace,\
            second_job_entry_time,\
            second_job_workplace,\
            retirement_date,\
            retirement_workplace,\
            year2start_learning_buddhism,\
            years_of_learning_buddhism,\
            deep_understanding_of_dharma,\
            reason2learning_dharma,\
            nums_of_buddhism_book,\
            easy2learn_buddhism_book,\
            hard2read,\
            maxim,\
            buddhist_disciples_of_family,\
            editor,\
            others\
            from people where %1 = %2"
     ).arg(name).arg(value);
    query.exec(sql);
    while(query.next()) {
        qDebug() << "name:(" << query.value(0).toString();
        ui->lineEditName->setText(query.value(0).toString());
        ui->lineEditGender->setText(query.value(1).toString());
        ui->lineEditJob->setText(query.value(2).toString());
        ui->lineEditHobby->setText(query.value(3).toString());
        ui->lineEditFName->setText(query.value(4).toString());
        ui->lineEditBirthday->setText(query.value(5).toString());
        ui->lineEditID->setText(query.value(6).toString());
        ui->lineEditPhoneNum->setText(query.value(7).toString());
        ui->lineEditRace->setText(query.value(8).toString());
        ui->lineEditDegree->setText(query.value(9).toString());
        ui->lineEditHealth->setText(query.value(10).toString());
        ui->lineEditTelphoneNum->setText(query.value(11).toString());
        ui->lineEditTime->setText(query.value(12).toString());
        ui->lineEditReceipt->setText(query.value(13).toString());
        ui->lineEditWorkPlace->setText(query.value(14).toString());
        ui->lineEditProvince->setText(query.value(15).toString());
        ui->lineEditCity->setText(query.value(16).toString());
        ui->lineEditDistrict->setText(query.value(17).toString());
        ui->lineEditAddress->setText(query.value(18).toString());
        ui->lineEditPostcode->setText(query.value(19).toString());
        ui->lineEditGraduateTime->setText(query.value(20).toString());
        ui->lineEditGraduateSchool->setText(query.value(21).toString());
        ui->lineEditFirstJobDate->setText(query.value(22).toString());
        ui->lineEditFirstJobWorkspace->setText(query.value(23).toString());
        ui->lineEditSecondJobDate->setText(query.value(24).toString());
        ui->lineEditSecondJobWorkspace->setText(query.value(25).toString());
        ui->lineEditRetirementDate->setText(query.value(26).toString());
        ui->lineEditRetirementWorkspace->setText(query.value(27).toString());
        ui->lineEditLearnBuddhismDate->setText(query.value(28).toString());
        ui->lineEditYearsOfLearningBuddhism->setText(query.value(29).toString());
        ui->lineEditBuddhismLevel->setText(query.value(30).toString());
        ui->lineEditReasonToLearnDharma->setText(query.value(31).toString());
        ui->lineEditReadNumsOfBuddhismBook->setText(query.value(32).toString());
        ui->lineEditEasyToLearnBuddhismBook->setText(query.value(33).toString());
        ui->lineEditHardToLearnBuddhismBook->setText(query.value(34).toString());
        ui->lineEditMaximOfBuddhism->setText(query.value(35).toString());
        ui->lineEditBuddhistDisciplesOfFamily->setText(query.value(36).toString());
        ui->lineEditor->setText(query.value(37).toString());
        ui->lineEditOthers->setText(query.value(38).toString());
    }
    return true;
}


void Connect::on_pushButtonClear_clicked()
{
    clear_lineEdits();
}
