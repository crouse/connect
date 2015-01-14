#include "connect.h"
#include "ui_connect.h"
#include <QMenu>
#include <QValidator>
#include <QStringList>
#include <QCompleter>
#include <QList>
#include <QDateTime>
#include <QtXlsx>
#include <QFileDialog>
#include <QInputDialog>

Connect::Connect(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Connect)
{
    // 默认变量
    server_ip = "192.168.1.5";
    ui->setupUi(this);
    status_label = new QLabel;

    //completor
    {
        // 民族
        QStringList race_list;
        race_list << " acz 阿昌族" << " bz 白族" << " baz 保安族" << " blz 布朗族" << " cxz 朝鲜族" << " dwez 达斡尔族" << " dz 傣族" << " daz 德昂族"
                  << " dxz 东乡族" << " dz 侗族" << " dlz 独龙族" << " elsz 俄罗斯族" << " elcz 鄂伦春族" << " ewkz 鄂温克族" << " gsz 高山族" << " glz 仡佬族"
                  << " hnz 哈尼族" << " hskz 哈萨克族" << " hz 汉族" << " hzz 赫哲族" << " hz 回族" << " jnz 基诺族" << " jz 京族" << " jpz 景颇族" << " hekzz 哈尔克孜族"
                  << " lhz 拉祜族" << " lz 黎族" << " lsz 傈僳族" << " lbz 珞巴族" << " mz 满族" << " mnz 毛南族" << " mbz 门巴族" << " mgz 蒙古族" << " mz 苗族"
                  << " mlz 仫佬族" << " nxz 纳西族" << " nz 怒族" << " pmz 普米族" << " qz 羌族" << " slz 撒拉族" << " sz 畲族" << " sz 水族" << " tjkz 塔吉克族"
                  << " ttez 塔塔尔族" << "tjz 土家族" << " tz 土族" << " wz 佤族" << " wwez 维吾尔族" << " wzbkz 乌孜别克族" << " xbz 锡伯族" << " yz 瑶族"
                  << " yz 彝族" << " ygz 裕固族" << " zz 藏族" << " zz 壮族";
        QCompleter *race_completer = new QCompleter(race_list, this);
        race_completer->setCaseSensitivity(Qt::CaseInsensitive);
        ui->lineEditRace->setCompleter(race_completer);

        // 文化程度
        QStringList degree_list;
        degree_list << " 1. 小学" << " 2. 初中" << " 3. 高中" << " 4. 大学专科" << " 5. 大学本科" << " 6. 硕士" << " 7. 博士" << " 8. 博士后";
        QCompleter *degree_completer = new QCompleter(degree_list, this);
        ui->lineEditDegree->setCompleter(degree_completer);

        // 健康状况
        QStringList health_list;
        health_list << " 1. 良好" << " 2. 一般" << " 3. 欠佳" << " 4. 重病";
        QCompleter *health_completer = new QCompleter(health_list, this);
        ui->lineEditHealth->setCompleter(health_completer);

        // 是否深刻了解佛法
        QStringList knowledge_list;
        knowledge_list << " 1. 完全不了解" << " 2. 不深刻" << " 3. 比较深刻";
        QCompleter *knowledge_completer = new QCompleter(knowledge_list, this);
        knowledge_completer->setCaseSensitivity(Qt::CaseInsensitive);
        ui->lineEditBuddhismLevel->setCompleter(knowledge_completer);

        // 以何因缘了解佛法
        QStringList reason_list;
        reason_list << " 1. 由法师(居士)引导学佛" << " 2. 由读佛教经典领悟学佛" << " 3. 由经历启发学佛";
        QCompleter *reason_completer = new QCompleter(reason_list, this);
        reason_completer->setCaseSensitivity(Qt::CaseInsensitive);
        ui->lineEditReasonToLearnDharma->setCompleter(reason_completer);

        // 家庭成员三宝弟子
        QStringList family_list;
        family_list << " 1. 父亲" << " 2. 母亲" << " 3. 兄弟" << " 4. 姐妹" << " 5. 妻子" << " 6. 孩子";
        QCompleter *family_completer = new QCompleter(family_list, this);
        family_completer->setCaseSensitivity(Qt::CaseInsensitive);
        ui->lineEditBuddhistDisciplesOfFamily->setCompleter(family_completer);

        // 学佛小组种类
        QStringList learn_kinds_list;
        learn_kinds_list << " 1. 周日山上 " << " 2. 平常山下" << " 3. 京外" << " 4. 周日山上/平常山下" << " 5. 周日山上/京外"
                   << " 6. 平常上下/京外" << " 7. 周日山上/平常山下/京外";
        QCompleter *learn_kinds_completer = new QCompleter(learn_kinds_list, this);
        ui->lineEditLearnKind->setCompleter(learn_kinds_completer);
    }

    // left table view
    {
        viewModel = new QStandardItemModel();
        viewModel->setHorizontalHeaderItem(0, new QStandardItem(QObject::trUtf8("姓名")));
        viewModel->setHorizontalHeaderItem(1, new QStandardItem(QObject::trUtf8("手机")));
        viewModel->setHorizontalHeaderItem(2, new QStandardItem(QObject::trUtf8("收据编号")));
        viewModel->setHorizontalHeaderItem(3, new QStandardItem(QObject::trUtf8("皈依证号")));
        viewModel->setHorizontalHeaderItem(4, new QStandardItem(QObject::trUtf8("学佛小组地址")));
        ui->tableView->setModel(viewModel);
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }

    // Regex to test if insert is right
    {
        /* set validators [below] */
        QRegExp regExpGender("^[\u7537\u5973]+$"); // 性别只允许输入男或者女
        ui->lineEditGender->setValidator(new QRegExpValidator(regExpGender, this));

        QRegExp regExpPhoneNum("1[3|5|7|8|][0-9]{9}"); // 判断输入是否为手机号，只允许输入正常手机号
        ui->lineEditPhoneNum->setValidator(new QRegExpValidator(regExpPhoneNum, this));

        QRegExp regExpTel("\\d{3}-\\d{8}|\\d{4}-\\d{7}");
        ui->lineEditTelephoneNum->setValidator(new QRegExpValidator(regExpTel, this));

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
        /* set validators [above] */
    }

    /* set edit time [below] */
    QDateTime time = QDateTime::currentDateTime();
    QString edit_date = time.toString("yyyy.MM.dd");
    ui->lineEditTime->setText(edit_date);
    /* set edit time [above] */

    // set local ip at status bar
    get_local_ip();
    status_label->setStyleSheet("font-size:9px");
    status_label->setText(QString(" Local Address: [%1], Server Default Address: [%2]").arg(local_ip, server_ip));
    statusBar()->addWidget(status_label);

    if (local_ip != server_ip) {
        ui->pushButtonExport->hide();
    }
}

Connect::~Connect()
{
    delete ui;
}

void Connect::on_tableView_doubleClicked(const QModelIndex &index)
{
    switch(index.column()) {
    case 0:
        complete_fields("name", index.data().toString());
        viewModel->removeRow(index.row());
        break;
    case 1:
        complete_fields("phone_num", index.data().toString());
        viewModel->removeRow(index.row());
        break;
    case 2:
        complete_fields("receipt", index.data().toString());
        viewModel->removeRow(index.row());
        break;
    }
}

void Connect::on_lineEditReceipt_editingFinished()
{
    complete_fields("receipt", ui->lineEditReceipt->text());
}

bool Connect::check_lineEdit_items()
{
    bool name = ui->lineEditName->text().isEmpty();
    bool receipt = ui->lineEditReceipt->text().isEmpty();
    bool editor = ui->lineEditor->text().isEmpty();

    if (name || receipt || editor) return false;
    else return true;
}

void Connect::on_pushButtonOK_clicked()
{
    if (!check_lineEdit_items()) {
        QMessageBox::information(this, "", "请至少填写姓名、收据编号、编辑人");
        return;
    }

    validate_input_values();
    if (update_database()) {
        append_items2_tableView();
    }
    clear_lineEdits();
    ui->lineEditName->setFocus();
}

/* user defined functions */
bool Connect::validate_input_values()
{
    //[1] valid ID card
    if (ui->lineEditID->text().isEmpty()) {
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

bool Connect::update_database()
{
    QSqlQuery query;
    // 如果存在一行一模一样的，先删除
    QString deleteDuplicateSql = QString("delete from people where id = '%1'").arg(dbid);
    dbid = 0;

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
                  telephone_num                     ,\
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
                  others                           ,\
                  learn_dharma_kinds               ,\
                  learn_dharma_address             ,\
                  code                             \
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
                :telephone_num                    ,\
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
                :others                          ,\
                :learn_dharma_kinds              ,\
                :learn_dharma_address            ,\
                :code                          )"
            );

    query.bindValue(":name", ui->lineEditName->text());
    query.bindValue(":gender", ui->lineEditGender->text());
    query.bindValue(":job", ui->lineEditJob->text());
    query.bindValue(":hobby", ui->lineEditHobby->text());
    query.bindValue(":fname", ui->lineEditFName->text());
    query.bindValue(":birthday", ui->lineEditBirthday->text());
    query.bindValue(":personnel_id", ui->lineEditID->text());
    query.bindValue(":phone_num", ui->lineEditPhoneNum->text());
    query.bindValue(":race", ui->lineEditRace->text().section(' ', -1));
    query.bindValue(":degree", ui->lineEditDegree->text().section(' ', -1));
    query.bindValue(":health", ui->lineEditHealth->text().section(' ', -1));
    query.bindValue(":telephone_num", ui->lineEditTelephoneNum->text());
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
    query.bindValue(":first_job_workplace", ui->lineEditFirstJobWorkspace->text());
    query.bindValue(":second_job_entry_time", ui->lineEditSecondJobDate->text());
    query.bindValue(":second_job_workplace", ui->lineEditSecondJobWorkspace->text());
    query.bindValue(":retirement_date", ui->lineEditRetirementDate->text());
    query.bindValue(":retirement_workplace", ui->lineEditRetirementWorkspace->text());
    query.bindValue(":year2start_learning_buddhism", ui->lineEditLearnBuddhismDate->text());
    query.bindValue(":years_of_learning_buddhism", ui->lineEditYearsOfLearningBuddhism->text());
    query.bindValue(":deep_understanding_of_dharma", ui->lineEditBuddhismLevel->text().section(' ', -1));
    query.bindValue(":reason2learning_dharma", ui->lineEditReasonToLearnDharma->text().section(' ', -1));
    query.bindValue(":nums_of_buddhism_book", ui->lineEditReadNumsOfBuddhismBook->text());
    query.bindValue(":easy2learn_buddhism_book", ui->lineEditEasyToLearnBuddhismBook->text());
    query.bindValue(":hard2read", ui->lineEditHardToLearnBuddhismBook->text());
    query.bindValue(":maxim", ui->lineEditMaximOfBuddhism->text());
    query.bindValue(":buddhist_disciples_of_family", ui->lineEditBuddhistDisciplesOfFamily->text().section(' ', -1));
    query.bindValue(":editor", ui->lineEditor->text());
    query.bindValue(":others", ui->lineEditOthers->text());
    query.bindValue(":learn_dharma_kinds", ui->lineEditLearnKind->text().section(' ', -1));
    query.bindValue(":learn_dharma_address", ui->lineEditLearnAddress->text());
    query.bindValue(":code", ui->lineEditCode->text());

    bool rt = query.exec();
    if (!rt) {
        // 输出错误到屏幕！[tbd] 这个得考虑到输入重复问题，下面的错误用户未必理解，所以得考虑到
        QMessageBox::information(this, "", QString("数据库错误，请联系开发运维人员：") + query.lastError().text());
        db.close();
        return false;
    }

    query.clear();

    return true;
}

bool Connect::clear_lineEdits()
{
    ui->lineEditName->clear();
    ui->lineEditGender->clear();
    ui->lineEditJob->clear();
    ui->lineEditHobby->clear();
    ui->lineEditFName->clear();
    ui->lineEditBirthday->clear();
    ui->lineEditID->clear();
    ui->lineEditPhoneNum->clear();
    ui->lineEditDegree->clear();
    ui->lineEditHealth->clear();
    ui->lineEditTelephoneNum->clear();
    ui->lineEditRace->clear();
    ui->lineEditProvince->clear();
    ui->lineEditLearnKind->clear();
    ui->lineEditLearnAddress->clear();
    ui->lineEditCode->clear();
    ui->lineEditCity->clear();
    //ui->lineEditTime->clear();
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
    int count = 0;
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
                telephone_num,\
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
                others,\
                learn_dharma_kinds,\
                learn_dharma_address,\
                code,\
                id\
                from people where %1 = '%2'"
            ).arg(name).arg(value);
    query.exec(sql);
    while(query.next()) {
        count++;
        dbid = query.value(42).toInt();
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
        ui->lineEditTelephoneNum->setText(query.value(11).toString());
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
        ui->lineEditLearnKind->setText(query.value(39).toString());
        ui->lineEditLearnAddress->setText(query.value(40).toString());
        ui->lineEditCode->setText(query.value(41).toString());
    }
    qDebug() << "query count: " << count;
    if (count >1) {
        QMessageBox::information(this, "", "数据库中包含多条同样的数据，填充采用的是随机记录，如果不对，请使用收据编号查询");
        ui->lineEditReceipt->setFocus();
    }

    query.clear();
    return true;
}


void Connect::on_pushButtonClear_clicked()
{
    clear_lineEdits();
    ui->lineEditName->setFocus();
}

void Connect::append_items2_tableView()
{
    QString name = ui->lineEditName->text();
    QString phone = ui->lineEditPhoneNum->text();
    QString receipt = ui->lineEditReceipt->text();
    QString code = ui->lineEditCode->text();
    QString learnAddress = ui->lineEditLearnAddress->text();

    QList <QStandardItem*> standardItemList;
    QStandardItem *nameItem = new QStandardItem(name);
    QStandardItem *phoneItem = new QStandardItem(phone);
    QStandardItem *receiptItem = new QStandardItem(receipt);
    QStandardItem *codeItem = new QStandardItem(code);
    QStandardItem *learnAddressItem = new QStandardItem(learnAddress);
    viewModel->appendRow(standardItemList << nameItem << phoneItem << receiptItem << codeItem << learnAddressItem);
}

void Connect::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton ask = QMessageBox::question(this, "connect", "你确定要关闭程序?\n", QMessageBox
                                                            ::Cancel|QMessageBox::Yes);
    if (ask != QMessageBox::Yes) {
        event->ignore();
        qDebug() << "ignore close event";
    } else {
        db.close();
        qDebug() << "db connection is closed";
    }
}

void Connect::get_local_ip()
{
    QString tmp_ip;
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
            tmp_ip = address.toString();
        if (!tmp_ip.startsWith("127")) {
            local_ip = tmp_ip;
        }
    }
}

void Connect::save_excel(QString fileName)
{
    int i = 2;
    int j;
    QXlsx::Document xlsx;
    QSqlQuery query;
    query.exec("select * from people");
    {
        xlsx.write("A1", "姓名");
        xlsx.write("B1", "性别");
        xlsx.write("C1", "职业");
        xlsx.write("D1", "特长");
        xlsx.write("E1", "法名");
        xlsx.write("F1", "出生年月");
        xlsx.write("G1", "身份证号");
        xlsx.write("H1", "手机号码");
        xlsx.write("I1", "民族");
        xlsx.write("J1", "文化程度");
        xlsx.write("K1", "健康状况");
        xlsx.write("L1", "固定电话");
        xlsx.write("M1", "填表时间");
        xlsx.write("N1", "收据编号");
        xlsx.write("O1", "工作单位");
        xlsx.write("P1", "省(直辖市/自治区)");
        xlsx.write("Q1", "市");
        xlsx.write("R1", "区/县");
        xlsx.write("S1", "通讯地址");
        xlsx.write("T1", "邮政编码");
        xlsx.write("U1", "毕业时间");
        xlsx.write("V1", "毕业学校");
        xlsx.write("W1", "工作入职时间1");
        xlsx.write("X1", "工作单位1");
        xlsx.write("Y1", "工作入职时间2");
        xlsx.write("Z1", "工作入职单位2");
        xlsx.write("AA1", "退休时间");
        xlsx.write("AB1", "退休单位");
        xlsx.write("AC1", "学佛时间");
        xlsx.write("AD1", "学佛时长");
        xlsx.write("AE1", "是否深刻理解佛法");
        xlsx.write("AF1", "以何因缘接触佛法");
        xlsx.write("AG1", "所读经典部数");
        xlsx.write("AH1", "认为易学经典");
        xlsx.write("AI1", "认为不易学经典");
        xlsx.write("AJ1", "感悟最深的一句话");
        xlsx.write("AK1", "家庭成员三宝弟子");
        xlsx.write("AL1", "编辑人");
        xlsx.write("AM1", "其他");
        xlsx.write("AN1", "学佛小组种类");
        xlsx.write("AO1", "学佛小组地址");
        xlsx.write("AP1", "皈依证编号");

    }
    // excel A-Z, AA-AZ, BA-BZ...
    // 总共42列，所以下面代码是可以的，再多就不能这么写了，我图省事就这么写了！
    while(query.next()) {
        char start = 'A';
        char ne = 'A';
        for(j = 0; j < 42; j++) {
            if (j < 26) {
                xlsx.write(QString("%1%2").arg(start).arg(i), query.value(j).toString());
                start++;
            } else {
                xlsx.write(QString("A%1%2").arg(ne).arg(i), query.value(j).toString());
                ne++;
            }
        }
        i++;
    }
    query.clear();
    xlsx.saveAs(fileName);
}

void Connect::on_pushButtonExport_clicked()
{
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this, "打开保存文件路径", "", "xlsx (*.xlsx)");
    if (!fileName.isNull()) {
        qDebug() << "save file" << fileName;
        save_excel(fileName);
    } else {
        return;
    }
}


void Connect::on_pushButton_clicked()
{
    init_and_append_items2_tableView();
}


bool Connect::init_and_append_items2_tableView()
{
    QSqlQuery query;
    QString sql;
    QString editor = ui->lineEditor->text();
    if (editor.isEmpty()) {
        sql = QString("select name, phone_num, receipt, code, learn_dharma_address from people");
    } else {
        sql = QString("select name, phone_num, receipt, code, learn_dharma_address from people where editor = '%1'").arg(editor);
    }

    query.exec(sql);

    viewModel->clear();
    viewModel->setHorizontalHeaderItem(0, new QStandardItem(QObject::trUtf8("姓名")));
    viewModel->setHorizontalHeaderItem(1, new QStandardItem(QObject::trUtf8("手机")));
    viewModel->setHorizontalHeaderItem(2, new QStandardItem(QObject::trUtf8("收据编号")));
    viewModel->setHorizontalHeaderItem(3, new QStandardItem(QObject::trUtf8("皈依证号")));
    viewModel->setHorizontalHeaderItem(4, new QStandardItem(QObject::trUtf8("学佛小组地址")));

    while(query.next()) {
        QString name = query.value(0).toString();
        QString phone = query.value(1).toString();
        QString receipt = query.value(2).toString();
        QString code = query.value(3).toString();
        QString learn_address = query.value(4).toString();

        QList <QStandardItem*> standardItemList;
        QStandardItem *nameItem = new QStandardItem(name);
        QStandardItem *phoneItem = new QStandardItem(phone);
        QStandardItem *receiptItem = new QStandardItem(receipt);
        QStandardItem *codeItem = new QStandardItem(code);
        QStandardItem *learnAddressItem = new QStandardItem(learn_address);
        viewModel->appendRow(standardItemList << nameItem << phoneItem << receiptItem << codeItem << learnAddressItem);
    }

    query.clear();
    return true;
}

bool Connect::init_db()
{
    // mysql
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(server_ip);
    db.setDatabaseName("connect");
    db.setUserName("lei");
    db.setPassword("123456");
    if(!db.open()) {
        QMessageBox::critical(this, "数据库错误", db.lastError().text());
        return false;
    }

    QSqlQuery query;
    QString createTableSql = "                             \
            create table if not exists people (            \
                id int(11) primary key not null auto_increment,\
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
                telephone_num                    varchar(20),  \
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
                years_of_learning_buddhism       varchar(10),  \
                deep_understanding_of_dharma     varchar(64),  \
                reason2learning_dharma           varchar(128), \
                nums_of_buddhism_book            varchar(10),  \
                easy2learn_buddhism_book         varchar(128), \
                hard2read                        varchar(128), \
                maxim                            varchar(128), \
                buddhist_disciples_of_family     varchar(128), \
                editor                           varchar(128), \
                others                           varchar(128), \
                learn_dharma_kinds               varchar(64),  \
                learn_dharma_address             varchar(128), \
                code                             varchar(64) \
                ) ENGINE=InnoDB DEFAULT CHARSET=utf8;";
    query.exec(createTableSql);
    query.clear();
    return true;
}

void Connect::on_pushButtonDatabase_pressed()
{
    if (!db.isOpen()) {
        if (init_db()) {
            ui->pushButtonDatabase->setDisabled(true);
        }
    }
}

void Connect::on_action_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this, "server address", "server address", QLineEdit::Normal, "192.168.1.5", &ok);
    if (ok && !text.isEmpty()) {
        server_ip = text;
    }
}
