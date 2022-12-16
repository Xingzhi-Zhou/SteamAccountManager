#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("steam 切号器");
    setWindowIcon(QIcon("niren.png"));
    status = new QLabel("V0.1  Niren Game");
    statusBar()->addPermanentWidget(status);

    label_path = new QLabel("steam路径:");
    label_steam_path = new QLabel;
    label_account = new QLabel("账号:");
    account = new QComboBox;
    button_start = new QPushButton("启动");
    button_kill = new QPushButton("强制结束");

    label_path->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    label_account->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    label_steam_path->setMinimumWidth(250);

    hlayout_button = new QHBoxLayout;
    hlayout_button->addWidget(button_kill);
    hlayout_button->addStretch();
    hlayout_button->addWidget(button_start);


    central_widget = new QWidget;
    glayout_main = new QGridLayout(central_widget);
    glayout_main->addWidget(label_path, 0, 0);
    glayout_main->addWidget(label_steam_path, 0, 1);
    glayout_main->addWidget(label_account, 1, 0);
    glayout_main->addWidget(account, 1, 1);
    glayout_main->addLayout(hlayout_button, 2, 1);

    glayout_main->setSpacing(10);
//    glayout_main->setColumnStretch(0, 1);
//    glayout_main->setColumnStretch(1, 5);

//    glayout_main->setRowStretch(0, 1);
//    glayout_main->setRowStretch(1, 1);
//    glayout_main->setRowStretch(2, 1);

    setCentralWidget(central_widget);

    if(!get_steam_path()){
        statusBar()->showMessage("ERROR:" + QString::number(error_code), 100000);
    }
    label_steam_path->setText(steam_path);

    if(!load_user()){
        statusBar()->showMessage("ERROR:" + QString::number(error_code), 100000);
    }
    account->addItems(account_name_list);

    connect(button_kill, &QPushButton::clicked, this, &MainWindow::on_button_kill_clicked);
    connect(button_start, &QPushButton::clicked, this, &MainWindow::on_button_start_clicked);
}

MainWindow::~MainWindow()
{
}

bool MainWindow::get_steam_path(){
    QSettings reg(STEAM_REGEDIT, QSettings::NativeFormat);
    steam_path = reg.value("InstallPath").toString();
//    qDebug() << steam_path;
    if(steam_path == ""){
        error_code = 1;
        return false;
    }
    return true;
}

bool MainWindow::load_user(){
    vdf_filename = steam_path + VDF_PATH;
    QFile vdf_file(vdf_filename);

    if(!vdf_file.open(QIODevice::ReadOnly|QIODevice::Text)){
        error_code = 2;
        return false;
    }

    // parse file
    QString line_str;
    int lines = 0;
    int parse_state = 0;
    QString steam_appid, account_name, persona_name, remember_pwd;
    while(!vdf_file.atEnd()){
        // read lines
        lines++;
        line_str = vdf_file.readLine();
        if(lines==1 || lines==2){
            continue;
        }

        // parse
        switch(parse_state){
        // steam_appid
        case 0:
            steam_appid = line_str.simplified().remove("\"");
            parse_state = 1;
            break;
        // skip
        case 1:
            parse_state = 2;
            break;
        // AccountName
        case 2:
            account_name = line_str.simplified().remove("\"");
            parse_state = 3;
            break;
        // PersonaName
        case 3:
            persona_name = line_str.simplified().remove("\"");
            parse_state = 4;
            break;
        // RememberPassword
        case 4:
            remember_pwd = line_str.simplified().remove("\"");
            parse_state = 5;
            break;
        // WantsOfflineMode
        case 5:
            parse_state = 6;
            break;
        // SkipOfflineModeWarning
        case 6:
            parse_state = 7;
            break;
        // AllowAutoLogin
        case 7:
            parse_state = 8;
            break;
        // MostRecent
        case 8:
            parse_state = 9;
            break;
        // Timestamp
        case 9:
            parse_state = 10;
            break;
        // skip & save the user cfg
        case 10:
            account_name = account_name.split(" ")[1];
            persona_name = persona_name.split(" ")[1];
            remember_pwd = remember_pwd.split(" ")[1];
            if(remember_pwd.toInt()){
                steam_appid_list << steam_appid;
                account_name_list << account_name;
                persona_name_list << persona_name;
//                qDebug() << steam_appid << account_name << persona_name;
            }

            parse_state = 0;
            break;
        default:
            break;
        }
    }
    return true;
}

void MainWindow::kill_steam(){
    QProcess process;
    process.start(KILL_STEAM);
    process.waitForStarted();
    process.waitForFinished();
    process.start(KILL_STEAM_SERVICE);
    process.waitForStarted();
    process.waitForFinished();
}

void MainWindow::change_user(QString account_name){
    QSettings reg(STEAM_USER_REGEDIT, QSettings::NativeFormat);
    reg.setValue("AutoLoginUser", account_name);
}

void MainWindow::start_steam(){
    Sleep(3000);
    QProcess process;
    QString cmd = "\"" + steam_path + "\\steam.exe\"";
    process.startDetached(cmd);
}

void MainWindow::on_button_kill_clicked(){
    kill_steam();
}

void MainWindow::on_button_start_clicked(){
    QString name = account->currentText();
    kill_steam();
    change_user(name);
    start_steam();
}
