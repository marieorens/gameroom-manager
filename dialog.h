#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include <QtCore>
#include <QtWidgets>
#include <QDebug>
#include <QStandardItemModel>
#include <QtSql/QSqlRecord>
#include <QVBoxLayout>
#include <QtSql/QSql>
#include <QtGui>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include "QMessageBox"
#include <QCompleter>
#include <QStringList>
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QUrlQuery>
#include <QLabel>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    void sendAdbPowerOff(const QString &tvIp);
    QString getTvIpByName(const QString &tvName);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_refresh_prices_clicked();

    void addSessionToList(const QString &game, const QString &console, int duree, const QString &selected_tv);

    void loadTVList();

    void loadSessionsToTable();
private:
    Ui::Dialog *ui;


};

#endif // DIALOG_H
