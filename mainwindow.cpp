#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "RenderObject.h"
#include <QHBoxLayout>
#include <QMessageBox>
#include <QColorDialog>

int irp=0;
int indx1 = 0;
QStringList strl;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //задание видимости объектов на форме
    ui->pushButton_2->setVisible(false);
    ui->pushButton_3->setVisible(false);
    ui->label->setVisible(false);
    ui->lineEdit->setVisible(false);
    ui->lineEdit_2->setVisible(false);
    ui->pushButton_6->setVisible(false);
    ui->pushButton_7->setVisible(false);
    ui->pushButton_8->setVisible(false);
    ui->pushButton_9->setVisible(false);
    ui->pushButton_10->setVisible(false);
    ui->pushButton_13->setVisible(false);
    ui->pushButton_14->setVisible(false);
    ui->pushButton_15->setVisible(false);
    ui->pushButton_16->setVisible(false);
    ui->pushButton_17->setVisible(false);
    ui->pushButton_19->setVisible(false);
    ui->pushButton_20->setVisible(false);
    ui->label_3->setVisible(false);
    ui->label_4->setVisible(false);
    ui->label_5->setVisible(false);
    ui->label_6->setVisible(false);
    ui->label_7->setVisible(false);
    ui->label_8->setVisible(false);
    ui->label_9->setVisible(false);
    ui->label_10->setVisible(false);
    ui->label_11->setVisible(false);
    ui->label_12->setVisible(false);
    ui->label_13->setVisible(false);
    ui->horizontalSlider->setVisible(false);
    ui->verticalSlider->setVisible(false);
    ui->radioButton->setVisible(false);
    ui->radioButton_2->setVisible(false);
    ui->radioButton_3->setVisible(false);
    //рисование окна на форме mainwindow
    r_obj = new RenderObject();
    QHBoxLayout * horizontalLayout = new QHBoxLayout(ui->stackedWidget->widget(0));
    horizontalLayout->setSpacing(6);
    horizontalLayout->setContentsMargins(11, 11, 11, 11);
    horizontalLayout->setObjectName("horizontalLayout");
    QWidget * renderingContainer = QWidget::createWindowContainer(r_obj, ui->stackedWidget->widget(0));
    horizontalLayout->addWidget(renderingContainer);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()//Create PolyLine
{
    indx1 = 1;//индекс для передачи сигнала в другой класс
    ui->pushButton_2->setVisible(true);
    ui->pushButton_3->setVisible(true);
    ui->label->setVisible(true);
    ui->lineEdit->clear();
    ui->lineEdit->setVisible(true);
    ui->pushButton->setVisible(false);
    ui->pushButton_4->setVisible(false);
    ui->pushButton_5->setVisible(false);
    ui->label_2->setVisible(false);
    ui->label_3->setVisible(true);
    r_obj->setCreateLine();//вызов метода создания точек и линий
}

void MainWindow::on_pushButton_2_clicked()//Save in Create PolyLine
{
    indx1 = 2;
    QString namePolyLine;
    if(ui->listWidget->count()>0)
    {
        for(int i=0; i<ui->listWidget->count();i++)
        {
            if(ui->listWidget->item(i)->text()==ui->lineEdit->text())
            {
                QMessageBox::critical(0,"Ошибка", "              Внимание!\n"
                                              "Объект с такм именем уже существует!");
                break;
            }
            else
            {
                namePolyLine=ui->lineEdit->text();
            }
        }
    }
    else
    {
        namePolyLine=ui->lineEdit->text();
    }
    ui->listWidget->clear();
    if(namePolyLine.isEmpty())
    {
        ui->listWidget->addItems(strl);
        QMessageBox::critical(0,"Ошибка", "       Внимание!\n"
                                          "Не заполнено поле!");
    }
    else
    {

        strl<<namePolyLine;
        ui->listWidget->addItems(strl);
        r_obj->saveLine( ui->lineEdit->text() );//сохранение имени линии
        buttons_inCreate();//задание видимости объектов на форме
    }
}

void MainWindow::on_pushButton_3_clicked()//Cancel in Create PolyLine
{
    indx1 = 3;
    r_obj->cancelCreateLine();//вызов метода отмены создания точек и линий
    buttons_inCreate();
}

void MainWindow::on_pushButton_4_clicked()//Delete PolyLine
{
    indx1 = 4;
    if(strl.isEmpty())
    {
        QMessageBox::critical(0,"Ошибка", "         Внимание!\n"
                                          "Нет объектов для удаления!");
    }
    else
    {
        for( int i = 0 ; i < ui->listWidget->count() ; i ++ )
        {
            if(ui->listWidget->currentItem()->text()==strl[i])
            {
                r_obj->deleteLine(ui->listWidget->currentItem()->text());//вызов метода удаления точек и линий
                strl.removeAt(i);
                ui->listWidget->clear();
                ui->listWidget->addItems(strl);
                break;
            }
        }
    }
}

void MainWindow::on_pushButton_5_clicked()//Modify PolyLine
{
    indx1 = 5;
    if(strl.isEmpty())
    {
        QMessageBox::critical(0,"Ошибка", "           Внимание!\n"
                                          "Нет объектов для редактирования!");
    }
    else
    {
    ui->pushButton->setVisible(false);
    ui->pushButton_4->setVisible(false);
    ui->pushButton_5->setVisible(false);
    ui->pushButton_6->setVisible(true);
    ui->pushButton_7->setVisible(true);
    ui->label_2->setVisible(false);
    ui->label_4->setVisible(true);
    ui->pushButton_9->setVisible(true);
    ui->pushButton_10->setVisible(true);
    }
}

void MainWindow::on_pushButton_6_clicked()//Add Point in Modify PolyLine
{
    indx1 = 6;
    ui->pushButton_6->setVisible(false);
    ui->pushButton_7->setVisible(false);
    ui->pushButton_8->setVisible(true);
    ui->pushButton_9->setVisible(true);
    ui->pushButton_10->setVisible(false);
    ui->label_4->setVisible(false);
    ui->label_5->setVisible(true);
    r_obj->addNewPoint(ui->listWidget->currentItem()->text(),indx1);//вызов метода добавления точек
}

void MainWindow::on_pushButton_7_clicked()//Remove Point in Modify PolyLine
{
    indx1 = 7;
    ui->pushButton_6->setVisible(false);
    ui->pushButton_7->setVisible(false);
    ui->pushButton_8->setVisible(true);
    ui->pushButton_9->setVisible(true);
    ui->pushButton_10->setVisible(false);
    ui->label_4->setVisible(false);
    ui->label_6->setVisible(true);
    r_obj->removePoint(ui->listWidget->currentItem()->text(),indx1);//вызов метода удаления точек
}

void MainWindow::on_pushButton_8_clicked()//Save in Modify PolyLine
{
    indx1 = 8;
    r_obj->saveModifyLine(ui->listWidget->currentItem()->text(),indx1);//вызов метода сохранения измененных точек
    buttons_inModify();
    ui->label_4->setVisible(false);
    ui->label_2->setVisible(true);
    ui->label_5->setVisible(false);
    ui->label_6->setVisible(false);
    ui->label_7->setVisible(false);
}

void MainWindow::on_pushButton_9_clicked()//Cancel in Modify PolyLine
{
    indx1 = 9;
    r_obj->cancelModifyLine(ui->listWidget->currentItem()->text(),indx1);//вызов метода отмены сохранения измененных точек
    buttons_inModify();
    ui->label_4->setVisible(false);
    ui->label_2->setVisible(true);
    ui->label_5->setVisible(false);
    ui->label_6->setVisible(false);
    ui->label_7->setVisible(false);
}

void MainWindow::buttons_inCreate()
{
    ui->pushButton->setVisible(true);
    ui->pushButton_2->setVisible(false);
    ui->pushButton_3->setVisible(false);
    ui->label->setVisible(false);
    ui->lineEdit->setVisible(false);
    ui->pushButton_4->setVisible(true);
    ui->pushButton_5->setVisible(true);
    ui->label_2->setVisible(true);
    ui->label_3->setVisible(false);
}

void MainWindow::buttons_inModify()
{
    ui->pushButton->setVisible(true);
    ui->pushButton_4->setVisible(true);
    ui->pushButton_5->setVisible(true);
    ui->pushButton_6->setVisible(false);
    ui->pushButton_7->setVisible(false);
    ui->pushButton_8->setVisible(false);
    ui->pushButton_9->setVisible(false);
    ui->pushButton_10->setVisible(false);
}

void MainWindow::updateRender()
{
    horizontalLayout->removeWidget(renderingContainer);
    delete renderingContainer;
    delete horizontalLayout;
    r_obj = new RenderObject();
    horizontalLayout = new QHBoxLayout(ui->stackedWidget->widget(0));
    renderingContainer = QWidget::createWindowContainer(r_obj, ui->stackedWidget->widget(0));
    horizontalLayout->setSpacing(6);
    horizontalLayout->setContentsMargins(11, 11, 11, 11);
    horizontalLayout->setObjectName("horizontalLayout");
    horizontalLayout->addWidget(renderingContainer);
}

void MainWindow::setSelectList(QString listName)
{
    //qDebug()<<listName;
    qDebug()<<ui->listWidget->count();
    for(int i=0; i<ui->listWidget->count();i++)
    {

        qDebug()<<ui->listWidget->item(i)->text();
        if(ui->listWidget->item(i)->text()==listName)
        {

            ui->listWidget->setCurrentItem(ui->listWidget->item(i),QItemSelectionModel::Select);
        }
    }
}


void MainWindow::on_pushButton_10_clicked()//Move Point
{
    indx1=10;
    ui->pushButton_6->setVisible(false);
    ui->pushButton_7->setVisible(false);
    ui->pushButton_8->setVisible(true);
    ui->pushButton_9->setVisible(true);
    ui->pushButton_10->setVisible(false);
    ui->label_4->setVisible(false);
    ui->label_6->setVisible(false);
    ui->label_7->setVisible(true);
    r_obj->movePoint(ui->listWidget->currentItem()->text(),indx1);//вызов метода перемещения точек
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    indx1=11;
    r_obj->selectPoint(ui->listWidget->currentItem()->text(),indx1);
}

void MainWindow::on_pushButton_11_clicked()//Create Ellipse
{
    indx1=111;
    ui->lineEdit_2->clear();
    r_obj->createEllipse(indx1);
    ui->pushButton_11->setVisible(false);
    ui->pushButton_12->setVisible(false);
    ui->pushButton_14->setVisible(true);
    ui->pushButton_15->setVisible(true);
    ui->pushButton_18->setVisible(false);
    ui->lineEdit_2->setVisible(true);
    ui->label_9->setVisible(true);
}

void MainWindow::on_pushButton_12_clicked()//Delete Ellipse
{
    indx1=12;
    if(strl.isEmpty())
    {
        QMessageBox::critical(0,"Ошибка", "         Внимание!\n"
                                          "Нет объектов для удаления!");
    }
    else
    {
        for( int i = 0 ; i < ui->listWidget->count() ; i ++ )
        {
            if(ui->listWidget->currentItem()->text()==strl[i])
            {
                r_obj->deleteEllipse(indx1, ui->listWidget->currentItem()->text());
                strl.removeAt(i);
                ui->listWidget->clear();
                ui->listWidget->addItems(strl);
                break;
            }
        }
    }
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    r_obj->resizeEllipseHoz(position, ui->listWidget->currentItem()->text());
}

void MainWindow::on_verticalSlider_sliderMoved(int position)
{
    r_obj->resizeEllipseVert(position, ui->listWidget->currentItem()->text());
}

void MainWindow::on_pushButton_13_clicked()//Set color
{
    QColor color;
    if(ui->radioButton->isChecked())
    {
    color = QColor(Qt::red);
    }
    if(ui->radioButton_3->isChecked())
    {
    color = QColor(Qt::blue);
    }
    if(ui->radioButton_2->isChecked())
    {
    color = QColor(Qt::green);
    }
    if(!ui->radioButton->isChecked() && !ui->radioButton_2->isChecked() && !ui->radioButton_3->isChecked())
    {
        QMessageBox::critical(0,"Ошибка", "      Внимание!\n"
                                          "Цвет эллипса не задан!");
    }
    else
    {
        r_obj->setEllipseColor(color, ui->listWidget->currentItem()->text());
        ui->pushButton_11->setVisible(true);
        ui->pushButton_12->setVisible(true);
        ui->pushButton_13->setVisible(false);
        ui->pushButton_18->setVisible(true);
        ui->radioButton->setVisible(false);
        ui->radioButton_2->setVisible(false);
        ui->radioButton_3->setVisible(false);
    }

}

void MainWindow::on_pushButton_14_clicked()//Save Ellipse
{
    indx1=14;
    QString nameEllipse;
    ui->listWidget->clear();
    nameEllipse=ui->lineEdit_2->text();
    if(nameEllipse.isEmpty())
    {
        ui->listWidget->addItems(strl);
        QMessageBox::critical(0,"Ошибка", "       Внимание!\n"
                                          "Не заполнено поле!");
    }
    else
    {
        strl<<nameEllipse;
        ui->listWidget->addItems(strl);
        r_obj->saveEllipse( ui->lineEdit_2->text(),indx1 );//сохранение имени линии
        ui->pushButton_11->setVisible(true);
        ui->pushButton_12->setVisible(true);
        ui->pushButton_14->setVisible(false);
        ui->pushButton_15->setVisible(false);
        ui->pushButton_18->setVisible(true);
        ui->label_9->setVisible(false);
        ui->lineEdit_2->setVisible(false);
    }
}

void MainWindow::on_pushButton_15_clicked()//Cancel create Ellipse
{
    indx1=15;
    r_obj->cancelCreateEllipse(indx1);
    ui->pushButton_11->setVisible(true);
    ui->pushButton_12->setVisible(true);
    ui->pushButton_14->setVisible(false);
    ui->pushButton_15->setVisible(false);
    ui->pushButton_18->setVisible(true);
    ui->label_9->setVisible(false);
    ui->lineEdit_2->setVisible(false);
}

void MainWindow::on_pushButton_16_clicked()//Change Color
{
    ui->pushButton_13->setVisible(true);
    ui->radioButton->setVisible(true);
    ui->radioButton_2->setVisible(true);
    ui->radioButton_3->setVisible(true);
    ui->pushButton_16->setVisible(false);
    ui->pushButton_17->setVisible(false);
    ui->pushButton_20->setVisible(false);

}

void MainWindow::on_pushButton_18_clicked()//Modify Ellipse
{
    ui->pushButton_11->setVisible(false);
    ui->pushButton_12->setVisible(false);
    ui->pushButton_16->setVisible(true);
    ui->pushButton_17->setVisible(true);
    ui->pushButton_18->setVisible(false);
    ui->pushButton_20->setVisible(true);

}

void MainWindow::on_pushButton_20_clicked()//Cancel Modify Ellipse
{
    ui->pushButton_11->setVisible(true);
    ui->pushButton_12->setVisible(true);
    ui->pushButton_16->setVisible(false);
    ui->pushButton_17->setVisible(false);
    ui->pushButton_18->setVisible(true);
    ui->pushButton_20->setVisible(false);
}

void MainWindow::on_pushButton_17_clicked()//Resize Ellipse
{
    ui->horizontalSlider->setVisible(true);
    ui->verticalSlider->setVisible(true);
    ui->pushButton_16->setVisible(false);
    ui->pushButton_17->setVisible(false);
    ui->pushButton_19->setVisible(true);
    ui->pushButton_20->setVisible(false);
    ui->label_8->setVisible(true);
}

void MainWindow::on_pushButton_19_clicked()//OK in Resize Ellipse
{
    ui->pushButton_11->setVisible(true);
    ui->pushButton_12->setVisible(true);
    ui->pushButton_18->setVisible(true);
    ui->pushButton_19->setVisible(false);
    ui->horizontalSlider->setVisible(false);
    ui->verticalSlider->setVisible(false);
    ui->label_8->setVisible(false);
}
