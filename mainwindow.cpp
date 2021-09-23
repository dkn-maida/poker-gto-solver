#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stdio.h"
#include "include/runtime/qsolverjob.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this->ui->actionChinese, SIGNAL(triggered()), this, SLOT(on_chinese_local()));
    connect(this->ui->actionEng, SIGNAL(triggered()), this, SLOT(on_en_local()));
    qSolverJob = new QSolverJob;
    qSolverJob->setContext(this->getLogArea());
    qSolverJob->current_mission = QSolverJob::MissionType::LOADING;
    qSolverJob->start();
    this->setWindowTitle("TexasSolver");
}

MainWindow::~MainWindow()
{
    delete qSolverJob;
    delete ui;
}

void MainWindow::on_buttomSolve_clicked()
{   
    qSolverJob->max_iteration = ui->iterationText->text().toInt();
    qSolverJob->accuracy = ui->exploitabilityText->text().toFloat();
    qSolverJob->print_interval = ui->logIntervalText->text().toInt();
    qSolverJob->thread_number = ui->threadsText->text().toInt();
    qSolverJob->current_mission = QSolverJob::MissionType::SOLVING;
    qSolverJob->start();
}

Ui::MainWindow * MainWindow::getPriUi(){
    return this->ui;
}

QTextEdit * MainWindow::getLogArea(){
    return this->ui->logOutput;
}

void MainWindow::on_clearLogButtom_clicked()
{
    this->ui->logOutput->clear();
}

void MainWindow::on_chinese_local(){

}

void MainWindow::on_en_local(){

}

vector<float> sizes_convert(QString input){
    QStringList list = input.split(" ");
    vector<float> sizes;
    foreach(QString num, list)
        sizes.push_back(num.toFloat());
    return sizes;
}

void MainWindow::on_buildTreeButtom_clicked()
{
    qSolverJob->range_ip = this->ui->ipRangeText->toPlainText().toStdString();
    qSolverJob->range_oop = this->ui->oopRangeText->toPlainText().toStdString();
    qSolverJob->board = this->ui->boardText->toPlainText().toStdString();
    qSolverJob->raise_limit = this->ui->boardText->toPlainText().toInt();
    qSolverJob->ip_commit = this->ui->potText->text().toFloat() / 2;
    qSolverJob->oop_commit = this->ui->potText->text().toFloat() / 2;
    qSolverJob->stack = this->ui->effectiveStackText->text().toFloat();
    qSolverJob->mode = this->ui->mode_box->currentIndex() == 0 ? QSolverJob::Mode::HOLDEM:QSolverJob::Mode::SHORTDECK;

    StreetSetting gbs_flop_ip = StreetSetting(sizes_convert(ui->flop_ip_bet->text()),
                                              sizes_convert(ui->flop_ip_raise->text()),
                                              vector<float>{},
                                              ui->flop_ip_allin->isChecked()
                                              );
    StreetSetting gbs_turn_ip = StreetSetting(sizes_convert(ui->turn_ip_bet->text()),
                                              sizes_convert(ui->turn_ip_raise->text()),
                                              vector<float>{},
                                              ui->turn_ip_allin->isChecked()
                                              );
    StreetSetting gbs_river_ip = StreetSetting(sizes_convert(ui->river_ip_bet->text()),
                                              sizes_convert(ui->river_ip_raise->text()),
                                              vector<float>{},
                                              ui->river_ip_allin->isChecked()
                                              );

    StreetSetting gbs_flop_oop = StreetSetting(sizes_convert(ui->flop_oop_bet->text()),
                                              sizes_convert(ui->flop_oop_raise->text()),
                                              vector<float>{},
                                              ui->flop_oop_allin->isChecked()
                                              );
    StreetSetting gbs_turn_oop = StreetSetting(sizes_convert(ui->turn_oop_bet->text()),
                                              sizes_convert(ui->turn_oop_raise->text()),
                                              sizes_convert(ui->turn_oop_donk->text()),
                                              ui->turn_oop_allin->isChecked()
                                              );
    StreetSetting gbs_river_oop = StreetSetting(sizes_convert(ui->river_oop_bet->text()),
                                              sizes_convert(ui->river_oop_raise->text()),
                                              sizes_convert(ui->river_oop_donk->text()),
                                              ui->river_oop_allin->isChecked()
                                              );

    qSolverJob->gtbs = make_shared<GameTreeBuildingSettings>(gbs_flop_ip,gbs_turn_ip,gbs_river_ip,gbs_flop_oop,gbs_turn_oop,gbs_river_oop);
    qSolverJob->current_mission = QSolverJob::MissionType::BUILDTREE;
    qSolverJob->start();
}
