#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include "RenderObject.h"
#include "MarkerSqr.h"
#include <QHBoxLayout>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void buttons_inCreate();
    void buttons_inModify();
    void updateRender();
    void setSelectList(QString listName);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_10_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_horizontalSlider_sliderMoved(int position);

    void on_verticalSlider_sliderMoved(int position);

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_21_clicked();

private:
    Ui::MainWindow *ui;
    RenderObject * r_obj;
    MarkerSqr * m_obj;
    bool save_cancel;
    QWidget * renderingContainer;
    QHBoxLayout * horizontalLayout;

};

#endif // MAINWINDOW_H
