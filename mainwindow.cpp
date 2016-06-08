#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //everything above this line was auto-generated in the constructor
    //we set the window title
    setWindowTitle("Othello");
    //we create the board and add it to the window (to the flow layout)
    _board = new table(this);
    ui->form_layout->addWidget(_board);
    //we set the timer here
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(update_info()));
    _timer->start();
}

//auto-generated destructor slightly altered
MainWindow::~MainWindow()
{
    delete ui;
    delete _board;
    delete _timer;
}

void MainWindow::on_left_button_clicked() {
    if(_board->get_state() == 0) //if the game is ready to start
        _board->set_state(1);   //start it
    else {
    //we ask the user if they wish to reset
        QMessageBox::StandardButton prompt;
        prompt = QMessageBox::question(this, "Reset?", "Do you want to restart the game?",
                                   QMessageBox::Yes|QMessageBox::No);
        if (prompt == QMessageBox::Yes)
            _board->set_state(0);
    }
}

void MainWindow::update_info() {
    switch(_board->get_state()) {
    case 0: //if the game is about to start
        ui->left_button->setDisabled(false);
        ui->right_button->setDisabled(false);
        ui->left_button->setText("New Game (1v1)");
        ui->right_button->setText("New Game (1vComp)");
        ui->info_label->setText("The game is ready to start.");
        //we set the game board to its starting state
        _board->reset();
        break;
    case 1: //if the game is in progress
        //we set the according button label
        if(_board->get_ind_comp())
        {
            ui->right_button->setText("Reset");
            ui->left_button->setDisabled(true);
        }
        else
        {
            ui->left_button->setText("Reset");
            ui->right_button->setDisabled(true);
        }

        //we check if the player currently in control can (not) make a move
        if(!_board->playable(_board->get_player())) {
            //if they can't, we need to check if a move was made previously
            if(!_board->move_made())
                //if a move wasn't made previously, and the current player can't make a move
                //the game ends
                _board->set_state(2);
            else {
                //otherwise, we adjust the indicator
                _board->set_move_made(false);
                //and switch control to the other player
                _board->switch_player();
            }
        }
        if(_board->get_player() == 'W')
            ui->info_label->setText("White player's turn.");
        else
            ui->info_label->setText("Black player's turn.");
        break;
    case 2: //if the game has ended
        ui->left_button->setText("Reset");
        short count_w = _board->count('W');
        short count_b = _board->count('B');
        QString s = "";
        if(count_w > count_b)
            //ui->info_label->setText("White player wins.");
            s.append("White player wins: ");
        else if(count_w < count_b)
            s.append("Black player wins: ");
        else
            s.append("Draw: ");
        s.append(QString::number(count_w));
        s.append("-");
        s.append(QString::number(count_b));
        ui->info_label->setText(s);
        break;
    //default:
        //return;
    }
    ui->info_label->setAlignment(Qt::AlignCenter);
}

//Computer game
void MainWindow::on_right_button_clicked()
{
    if(_board->get_state() == 0)//if the game is ready to start
    {
        _board->set_state(1);   //start it
        _board->set_ind_comp(true);
    }
    else {
    //we ask the user if they wish to reset
        QMessageBox::StandardButton prompt;
        prompt = QMessageBox::question(this, "Reset?", "Do you want to restart the game?",
                                   QMessageBox::Yes|QMessageBox::No);
        if (prompt == QMessageBox::Yes)
            _board->set_state(0);
    }
}
