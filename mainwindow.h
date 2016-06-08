#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "table.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //constructor and destructor
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui; //automatically added
    table *_board;
    QTimer *_timer; //this timer will be used to update the information text

private slots:
    void on_left_button_clicked(); //when the left button is pressed
    void update_info(); //this method will update button labels and the info text

    void on_right_button_clicked();
};

#endif // MAINWINDOW_H
