#include "dialog.h"
#include "ui_dialog.h"
#include <QtSql/QSqlError>
#include <mainwindow.h>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{

    QSqlDatabase music=QSqlDatabase::addDatabase("QSQLITE");
    music.setDatabaseName("salle.db");
    if (music.open()) {
        qDebug() << "Base de données 'music.db' en cours de création...........";
        qDebug() << "Base de données 'music.db' ouverte avec succès.";
        qDebug() << "Chemin courant : " << QDir::currentPath();

        QSqlQuery query;

        QString createTVQuery = "CREATE TABLE IF NOT EXISTS tvs ("
                                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                "nom_tv TEXT NOT NULL, "
                                "ip TEXT NOT NULL"
                                ")";

        QString createTableQuery = "CREATE TABLE IF NOT EXISTS sessions ("
                                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                   "nom_jeu TEXT NOT NULL, "
                                   "console TEXT NOT NULL, "
                                   "duree INTEGER NOT NULL, "
                                   "date_debut DATETIME DEFAULT CURRENT_TIMESTAMP, "
                                   "nom_tv TEXT)";




        loadSessionsToTable();

        if (query.exec(createTVQuery) && query.exec(createTableQuery)) {
            qDebug() << "Table 'sessions' et 'tvs' et 'users' créées avec succès.";}
        else{
            qDebug() << "Erreur lors de la création de la table : ";
        }
    } else {
        qDebug() << "Erreur lors de l'ouverture de la base de données : ";
    }
    if (music.open()) {
        qDebug() << "Base de données ouverte avec succès.";
    }
    ui->setupUi(this);
    loadTVList();
    this->setWindowTitle("Gestion Marcel");
    this->setWindowFlags(Qt::Window | Qt::WindowMaximizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
    this->setMinimumSize(800,600);
    this->setWindowIcon(QIcon());


    ui->jeu->setPlaceholderText("Tapez ou sélectionnez un jeu");

    QStringList jeuxList = {
        "NINJA GAIDEN 3",
        "NBA 2K17",
        "Mortal Kombat vs. DC Univers",
        "GTA V",
        "mmCM",
        "FIGHT NIGHT CHAMPION",
        "Dead to Rights: Retribution",
        "BLUR",
        "PES 2025 | BITBOX PATCH",
        "Assassin's Creed Rogue",
        "METAL GEAR SOLID V: GROUND ZEROES",
        "EA SPORTS FC 24",
        "CALL OF DUTY",
        "Captures",
        "METRO 2033 Redux",
        "Sekiro: Shadows Die Twice",
        "NARUTO SHIPPUDEN",
        "ASSASSIN'S CREED IV Black Flag",
        "Need For Speed Unbound",
        "Metro Exodus",
        "PES 2025",
        "FIFA 20",
        "FC 25",
        "MORTAL KOMBAT",
        "NARUTO SHIPPUDEN: Ultimate Ninja",
        "God of War Ragnarok",
        "Need for Speed Heat",
        "NBA 2K23",
        "TEKKEN 7",
        "Shadow of the Tomb Raider"
    };


    QCompleter *completer = new QCompleter(jeuxList, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->jeu->setCompleter(completer);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    this->close();
    MainWindow *log = new MainWindow();
    log->show();
}




void Dialog::on_pushButton_2_clicked()
{

    QString game = ui->jeu->currentText();
    QString console = ui->console->currentText();
    int duree = ui->duree->value();
    QString selectedtv = ui->tv->currentText();


    QSqlQuery query;
    query.prepare("INSERT INTO sessions(nom_jeu,console,duree,nom_tv) VALUES(:nom_jeu,:console,:duree,:nom_tv)");
    query.bindValue(":nom_jeu",game);
    query.bindValue(":console",console);
    query.bindValue(":duree",duree);
    query.bindValue(":nom_tv",selectedtv);

    if (query.exec()){

        QMessageBox::warning(this,"Session","Session enregistrée avec succès.Veuillez vous référer à la section 'Sessions en cours' pour effectuer le suivi.");
        ui->jeu->setCurrentText("Selectionnez un jeu");
        ui->duree->setValue(0);
        ui->tv->setCurrentText("Selectionnez une TV");
        ui->console->setCurrentText("Sélectionnez la console");
        addSessionToList(game, console, duree, selectedtv);
    }else{
        qDebug() << "Erreur lors de l'insertion : ";
    }
}

void Dialog::addSessionToList(const QString &game, const QString &console, int duree, const QString &selected_tv) {
    QWidget *sessionWidget = new QWidget(this);
    QHBoxLayout *sessionLayout = new QHBoxLayout(sessionWidget);

    QLabel *gameLabel = new QLabel(game);
    sessionLayout->addWidget(gameLabel);

    QLabel *tvLabel = new QLabel(selected_tv);
    sessionLayout->addWidget(tvLabel);

    QLabel *consoleLabel = new QLabel(console);
    sessionLayout->addWidget(consoleLabel);

    QLabel *durationLabel = new QLabel(QString::number(duree) + " minutes");
    sessionLayout->addWidget(durationLabel);

    QLabel *timerLabel = new QLabel(QString::number(duree) + " min");
    sessionLayout->addWidget(timerLabel);

    QPushButton *cutTvButton = new QPushButton("Session Terminée");
    cutTvButton->setEnabled(false);
    sessionLayout->addWidget(cutTvButton);

    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(sessionWidget->sizeHint());
    ui->listWidgetSessions->addItem(item);
    ui->listWidgetSessions->setItemWidget(item, sessionWidget);

    // Créer un minuteur local pour chaque session
    QTimer *sessionTimer = new QTimer(this);  // Nouveau timer pour chaque session
    int *secondsLeft = new int(duree * 60);  // Pointeur vers secondsLeft pour le gérer dans la lambda

    // Connectez le signal du minuteur à une fonction lambda qui gère l'affichage du timer
    connect(sessionTimer, &QTimer::timeout, [this, timerLabel, cutTvButton, selected_tv, sessionTimer, secondsLeft]() {
        if (*secondsLeft > 0) {
            (*secondsLeft)--;  // Décrémenter la valeur pointée par secondsLeft
            int minutes = *secondsLeft / 60;
            int seconds = *secondsLeft % 60;
            timerLabel->setText(QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));
        } else {
            cutTvButton->setEnabled(true);
            cutTvButton->setStyleSheet(
                "background-color: red; color: white; border-radius: 5px;"
                );
            sessionTimer->stop();  // Arrêter le minuteur une fois que la session est terminée

            QString tvIp = getTvIpByName(selected_tv);
            if (!tvIp.isEmpty()) {
                sendAdbPowerOff(tvIp);
            } else {
                qDebug() << "Impossible de récupérer l'IP de la TV : ";
            }
        }
    });

    // Démarrer le minuteur pour chaque session
    sessionTimer->start(1000);
}


void Dialog::sendAdbPowerOff(const QString &tvIp) {
    if (tvIp.isEmpty()) {
        qDebug() << "Adresse IP invalide. Impossible d'envoyer la commande ADB.";
        return;
    }

    QString adbCommand = QString("adb connect %1 && adb -s %1 shell input keyevent 26").arg(tvIp);
    int result = system(adbCommand.toStdString().c_str());

    if (result == 0) {
        qDebug() << "Commande ADB envoyée avec succès pour éteindre la TV : " << tvIp;
    } else {
        qDebug() << "Échec de l'envoi de la commande ADB pour la TV : " << tvIp;
    }
}


QString Dialog::getTvIpByName(const QString &tvName) {
    if (tvName.isEmpty()) {
        qDebug() << "Nom de TV invalide. Impossible de récupérer l'adresse IP.";
        return QString();
    }

    QSqlQuery query;
    query.prepare("SELECT ip FROM tvs WHERE nom_tv = :nom");
    query.bindValue(":nom", tvName);

    if (query.exec()) {
        if (query.next()) {
            QString ip = query.value(0).toString();
            qDebug() << "Adresse IP trouvée pour la TV" << tvName << ":" << ip;
            return ip;
        } else {
            qDebug() << "Aucune TV trouvée avec le nom :" << tvName;
            return QString();
        }
    } else {
        qDebug() << "Erreur lors de l'exécution de la requête SQL : " << query.lastError();
        return QString();
    }
}





void Dialog::on_pushButton_3_clicked()
{
    QString nomTV = ui->zone_tv->text();
    QString ipTV = ui->zone_ip->text();

    if (nomTV.isEmpty() || ipTV.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs.");
        return;
    }


    QProcess adbProcess;
    adbProcess.start("adb", {"connect", ipTV});
    if (!adbProcess.waitForFinished(7000)) {
        QMessageBox::warning(this, "Erreur", "La connexion à la TV a échoué. Vérifiez l'adresse IP.");
        return;
    }

    QString output = adbProcess.readAllStandardOutput();
    if (!output.contains("connected to")) {
        QMessageBox::warning(this, "Erreur", "Impossible de se connecter à la TV via ADB.");
        return;
    }


    QSqlQuery query;
    query.prepare("INSERT INTO tvs (nom_tv, ip) VALUES (:nom, :ip)");
    query.bindValue(":nom", nomTV);
    query.bindValue(":ip", ipTV);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "TV enregistrée avec succès !");
        loadTVList();
    } else {
        QMessageBox::warning(this, "Erreur", "Erreur lors de l'enregistrement dans la base de données.");
    }
}


QStandardItemModel *modelTVs;

void Dialog::loadTVList()
{

    if (!modelTVs) {
        modelTVs = new QStandardItemModel(this);
    }
    modelTVs->clear();

    QSqlQuery query("SELECT nom_tv FROM tvs");
    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur", "Impossible de charger la liste des TVs.");
        return;
    }

    while (query.next()) {
        QString nomTV = query.value("nom_tv").toString();
        QStandardItem *item = new QStandardItem(nomTV);
        modelTVs->appendRow(item);

        QString Tv = query.value(0).toString();
        ui->tv->addItem(Tv);
    }



    ui->listView->setModel(modelTVs);
}

void Dialog::loadSessionsToTable() {

    QSqlQuery query;
    query.prepare("SELECT nom_jeu, console, duree, date_debut, nom_tv  FROM sessions");

    if (!query.exec()) {
        qDebug() << "Erreur lors de la récupération des sessions :" << query.lastError();
        return;
    }




    if (ui->tableWidget) {

    } else {
        qDebug() << "tableWidget est nul";
    }
    int row = 0;
    while (query.next()) {
        QString game = query.value("nom_jeu").toString();
        QString console = query.value("console").toString();
        int duree = query.value("duree").toInt();
        QString tv = query.value("nom_tv").toString();
        QString date = query.value("date_debut").toString();


        int prix = 0;
        if (console == "PS3") {
            prix = (duree / 10) * 100;
        } else if (console == "PS4") {
            prix = (duree / 10) * 200;
        } else if (console == "XBOX") {
            prix = (duree / 10) * 300;
        }


        ui->tableWidget->insertRow(row);


        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(game));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(console));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(duree)));
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(date));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(tv));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(prix)));
        ui->tableWidget->resizeColumnsToContents();

        row++;
    }
}






void Dialog::on_refresh_prices_clicked()
{
    loadSessionsToTable();
}

