#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define STEAM_REGEDIT "HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Valve\\Steam"
#define STEAM_USER_REGEDIT "HKEY_CURRENT_USER\\SOFTWARE\\Valve\\Steam"
#define VDF_PATH "\\config\\loginusers.vdf"

#define KILL_STEAM "taskkill /f /im steam.exe"
#define KILL_STEAM_SERVICE "taskkill /f /im SteamService.exe"

#include <QMainWindow>
#include <QSettings>
#include <QFile>
#include <QTextCodec>
#include <QProcess>
#include <windows.h>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QStatusBar>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QDebug>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    // steam
    int error_code;
    QString steam_path;
    QString vdf_filename;
    QStringList steam_appid_list;
    QStringList account_name_list;
    QStringList persona_name_list;
    // GUI init
    void frame_init();
    // Load the config of steam
    bool get_steam_path();
    bool load_user();
    //
    void kill_steam();
    void start_steam();
    void change_user(QString account_name);

    // UI
    QLabel *label_path;
    QLabel *label_steam_path;

    QLabel *label_account;
    QComboBox *account;

    QPushButton *button_start;
    QPushButton *button_kill;

    QLabel *status;

    QGridLayout *glayout_main;
    QHBoxLayout *hlayout_button;
    QWidget *central_widget;

private slots:
    void on_button_start_clicked();
    void on_button_kill_clicked();
};
#endif // MAINWINDOW_H
