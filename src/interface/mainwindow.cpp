#include "mainwindow.h"

#include <qgifimage.h>

#include <QColorDialog>
#include <QTimer>
#include <vector>

#include "glview.h"
#include "ui_mainwindow.h"
extern "C" {
#include "../base/affine.h"
#include "../base/parsing_obj.h"
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  settings = new QSettings("settings.ini", QSettings::IniFormat, this);

  openGLWidget = new GlView(this);
  openGLWidget->move(0, 0);
  openGLWidget->resize(800, 600);
  openGLWidget->show();

  ui->spinBoxLineWidth->setValue(1.0f);

  connect(openGLWidget, &GlView::signalTextToForm, this,
          &MainWindow::slotTextFromGlView);

  loadSettings();

  count = 0;
  connect(openGLWidget, &GlView::signalTextToForm, this,
          &MainWindow::slotTextFromGlView);

  screenFolder = "/";
  QDateTime date = QDateTime::currentDateTime();
  screenTime = date.toString("dd_MM_yyyy_hh_mm_ss");

  joyMove();
  ui->radioButtonMove->setChecked(1);

  ui->statusBar->showMessage("Written by phyliscr, morrisro and arnoldrh.",
                             5000);
}

MainWindow::~MainWindow() {
  saveSettings();
  kill_structs((openGLWidget->vertex), (openGLWidget->polygons),
               openGLWidget->polygons_size);
  delete ui;
}

//************************************************************
//************************************************************
//************************************************************

void MainWindow::slotTextFromGlView(const QString &message) {
  ui->statusBar->showMessage(message);
}

void MainWindow::setBGColor(QString color_hex_string) {
  QColor color(color_hex_string);
  openGLWidget->bg_red = color.redF();
  openGLWidget->bg_green = color.greenF();
  openGLWidget->bg_blue = color.blueF();
  openGLWidget->bg_alpha = color.alphaF();
}

void MainWindow::setLineColor(QString color_hex_string) {
  QColor color(color_hex_string);
  openGLWidget->line_red = color.redF();
  openGLWidget->line_green = color.greenF();
  openGLWidget->line_blue = color.blueF();
  openGLWidget->line_alpha = color.alphaF();
}

void MainWindow::setPointColor(QString color_hex_string) {
  QColor color(color_hex_string);
  openGLWidget->point_red = color.redF();
  openGLWidget->point_green = color.greenF();
  openGLWidget->point_blue = color.blueF();
  openGLWidget->point_alpha = color.alphaF();
}

bool MainWindow::choiceScreenFolder() {
  bool error = 0;
  screenFolder = QFileDialog::getExistingDirectory(
      this, tr("Open Directory"), "~",
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (screenFolder.isEmpty() && screenFolder.isNull())
    error = 1;
  else
    ui->statusBar->showMessage(screenFolder, 3000);

  return error;
}

void MainWindow::updateFrames() {
  if (count == 50) {
    QGifImage gif(QSize(640, 480));
    gif.setDefaultDelay(100);
    for (int i = 0; i < 50; i++) gif.addFrame(frames[i]);
    gif.save(screenFolder + "/screencast_" + screenTime + ".gif");
    timer->stop();
  } else {
    QPixmap pixmap = openGLWidget->grab();
    QImage image = pixmap.toImage();
    image = image.scaled(640, 480);
    frames[count] = (image);
    count++;
  }
}

//************************************************************
//************************* SETTINGS *************************
//************************************************************
void MainWindow::loadSettings() {
  openGLWidget->ortho = settings->value("ortho", "1").toInt();
  ui->checkBoxOrtho->setChecked(openGLWidget->ortho);

  openGLWidget->draw_style = settings->value("draw_style", "0").toInt();
  switch (openGLWidget->draw_style) {
    case 0:
      ui->radioButtonSolid->setChecked(1);
      break;
    case 1:
      ui->radioButtonStripple->setChecked(1);
      break;
    case 2:
      ui->radioButtonPoint->setChecked(1);
      break;
    default:
      break;
  }

  openGLWidget->background_color =
      settings->value("background_color", "#000000").toString();
  ui->frameBFColor->setStyleSheet(
      "background-color: " + openGLWidget->background_color + ";");
  setBGColor(openGLWidget->background_color);

  openGLWidget->line_width = settings->value("line_width", "0").toInt();
  ui->spinBoxLineWidth->setValue(openGLWidget->line_width);
  openGLWidget->line_color =
      settings->value("line_color", "#ffffff").toString();
  ui->frameLineColor->setStyleSheet(
      "background-color: " + openGLWidget->line_color + ";");
  setLineColor(openGLWidget->line_color);

  openGLWidget->point_color =
      settings->value("point_color", "#ffffff").toString();
  ui->framePointColor->setStyleSheet(
      "background-color: " + openGLWidget->point_color + ";");
  setPointColor(openGLWidget->point_color);

  openGLWidget->point_width = settings->value("point_width", "0").toInt();
  ui->spinBoxPointWidth->setValue(openGLWidget->point_width);
  openGLWidget->point_round = settings->value("point_round", "0").toInt();
  ui->checkBoxRound->setChecked(openGLWidget->point_round);

  openGLWidget->joy_step = settings->value("joy_step", "100").toInt();
  ui->spinBoxStep->setValue(openGLWidget->joy_step);
}

void MainWindow::saveSettings() {
  settings->setValue("ortho", openGLWidget->ortho);
  settings->setValue("draw_style", openGLWidget->draw_style);
  settings->setValue("background_color", openGLWidget->background_color);

  settings->setValue("line_width", openGLWidget->line_width);
  settings->setValue("line_color", openGLWidget->line_color);

  settings->setValue("point_color", openGLWidget->point_color);
  settings->setValue("point_width", openGLWidget->point_width);
  settings->setValue("point_round", openGLWidget->point_round);
  settings->setValue("joy_step", openGLWidget->joy_step);
}

//************************************************************
//******************** OTHER UI BLOCK ************************
//************************************************************
void MainWindow::on_checkBoxOrtho_stateChanged(int arg1) {
  openGLWidget->ortho = arg1;
  openGLWidget->update();
}

//************************************************************
//******************* RADIO/CHECK BLOCK **********************
//************************************************************
void MainWindow::on_radioButtonSolid_toggled(bool checked) {
  openGLWidget->draw_style = 0;
  if (checked) ui->statusBar->showMessage("SOLID", 3000);
  openGLWidget->update();
}

void MainWindow::on_radioButtonStripple_toggled(bool checked) {
  openGLWidget->draw_style = 1;
  if (checked) ui->statusBar->showMessage("STRIPPLE", 3000);
  openGLWidget->update();
}

void MainWindow::on_radioButtonPoint_toggled(bool checked) {
  openGLWidget->draw_style = 2;
  if (checked) ui->statusBar->showMessage("POINTS", 3000);
  openGLWidget->update();
}

void MainWindow::on_checkBoxRound_stateChanged(int arg1) {
  openGLWidget->point_round = arg1;
  ui->statusBar->showMessage("Point rounded: " + QString::number(arg1), 3000);
  openGLWidget->update();
}

void MainWindow::on_radioButtonMove_toggled(bool checked) {
  if (checked) {
    joyMove();
  }
}

void MainWindow::on_radioButtonRotate_toggled(bool checked) {
  if (checked) {
    joyRotate();
  }
}

void MainWindow::on_radioButtonZoom_toggled(bool checked) {
  if (checked) {
    joyZoom();
  }
}

void MainWindow::joyMove() {
  ui->buttonUp->setEnabled(true);
  ui->buttonDown->setEnabled(true);
  ui->buttonRight->setEnabled(true);
  ui->buttonLeft->setEnabled(true);
  ui->buttonZ1->setEnabled(true);
  ui->buttonZ2->setEnabled(true);
  ui->buttonUp->setText("↑");
  ui->buttonDown->setText("↓");
  ui->buttonRight->setText("→");
  ui->buttonLeft->setText("←");
  ui->buttonZ1->setText("+");
  ui->buttonZ2->setText("-");
}

void MainWindow::joyRotate() {
  ui->buttonUp->setEnabled(true);
  ui->buttonDown->setEnabled(true);
  ui->buttonRight->setEnabled(true);
  ui->buttonLeft->setEnabled(true);
  ui->buttonZ1->setEnabled(true);
  ui->buttonZ2->setEnabled(true);
  ui->buttonUp->setText("↑");
  ui->buttonDown->setText("↓");
  ui->buttonRight->setText("→");
  ui->buttonLeft->setText("←");
  ui->buttonZ1->setText("↻");
  ui->buttonZ2->setText("↺");
}

void MainWindow::joyZoom() {
  ui->buttonLeft->setEnabled(false);
  ui->buttonRight->setEnabled(false);
  ui->buttonZ1->setEnabled(false);
  ui->buttonZ2->setEnabled(false);
  ui->buttonUp->setText("+");
  ui->buttonDown->setText("-");
  ui->buttonRight->setText("");
  ui->buttonLeft->setText("");
  ui->buttonZ1->setText("");
  ui->buttonZ2->setText("");
}

//************************************************************
//********************* BUTTON BLOCK *************************
//************************************************************
void MainWindow::on_buttonOpen_clicked() {
  QString filename = QFileDialog::getOpenFileName();
  if (!filename.isEmpty()) {
    QByteArray ba = filename.toLocal8Bit();
    openGLWidget->open_file(ba.data());
    ui->statusBar->showMessage(
        filename +
        " [vertex: " + QString::number(openGLWidget->vertex.amount_vert) +
        " poligons: " + QString::number(openGLWidget->polygons_size) + "]");
  }
}

void MainWindow::on_buttonBGColor_clicked() {
  QColor color = QColorDialog::getColor(Qt::white, this);
  if (color.isValid()) {
    openGLWidget->background_color = color.name();
    setBGColor(openGLWidget->background_color);
    ui->statusBar->showMessage(
        "Background color: " + openGLWidget->background_color, 3000);
    ui->frameBFColor->setStyleSheet(
        "background-color: " + openGLWidget->background_color + ";");
  }
}

void MainWindow::on_buttonLineColor_clicked() {
  QColor color = QColorDialog::getColor(Qt::white, this);
  if (color.isValid()) {
    openGLWidget->line_color = color.name();
    setLineColor(openGLWidget->line_color);
    ui->statusBar->showMessage("Line color: " + openGLWidget->line_color, 3000);
    ui->frameLineColor->setStyleSheet(
        "background-color: " + openGLWidget->line_color + ";");
  }
}

void MainWindow::on_buttonPointColor_clicked() {
  QColor color = QColorDialog::getColor(Qt::white, this);
  if (color.isValid()) {
    openGLWidget->point_color = color.name();
    setPointColor(openGLWidget->point_color);
    ui->statusBar->showMessage("Point color: " + openGLWidget->point_color,
                               3000);
    ui->framePointColor->setStyleSheet(
        "background-color: " + openGLWidget->point_color + ";");
  }
}

void MainWindow::on_buttonBmp_clicked() {
  if (!choiceScreenFolder()) {
    QPixmap pixmap = openGLWidget->grab();
    QFile file(screenFolder + "/screenshot_" + screenTime + ".bmp");
    file.open(QIODevice::WriteOnly);
    pixmap.save(&file, "BMP");
  }
}

void MainWindow::on_buttonJpg_clicked() {
  if (!choiceScreenFolder()) {
    QPixmap pixmap = openGLWidget->grab();
    QFile file(screenFolder + "/screenshot_" + screenTime + ".jpeg");
    file.open(QIODevice::WriteOnly);
    pixmap.save(&file, "JPEG");
  }
}

void MainWindow::on_buttonGif_clicked() {
  if (!choiceScreenFolder()) {
    count = 0;
    timer = new QTimer();
    timer->start(100);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateFrames()));
  }
}

void MainWindow::on_buttonUp_clicked() {
  if (openGLWidget->polygons_size != 0) {
    if (ui->radioButtonMove->isChecked()) {
      moveOY(&(openGLWidget->vertex), K * openGLWidget->joy_step);
      openGLWidget->update();
    } else if (ui->radioButtonRotate->isChecked()) {
      rotateOX(&(openGLWidget->vertex), -K * openGLWidget->joy_step);
      openGLWidget->update();
    } else if (ui->radioButtonZoom->isChecked()) {
      scale(&(openGLWidget->vertex), 1 + K * openGLWidget->joy_step);
      openGLWidget->update();
    }
  }
}

void MainWindow::on_buttonDown_clicked() {
  if (openGLWidget->polygons_size != 0) {
    if (ui->radioButtonMove->isChecked()) {
      moveOY(&(openGLWidget->vertex), -K * openGLWidget->joy_step);
      openGLWidget->update();
    } else if (ui->radioButtonRotate->isChecked()) {
      rotateOX(&(openGLWidget->vertex), K * openGLWidget->joy_step);
      openGLWidget->update();
    } else if (ui->radioButtonZoom->isChecked()) {
      scale(&(openGLWidget->vertex), 1 - K * openGLWidget->joy_step);
      openGLWidget->update();
    }
  }
}

void MainWindow::on_buttonLeft_clicked() {
  if (openGLWidget->polygons_size != 0) {
    if (ui->radioButtonMove->isChecked()) {
      moveOX(&(openGLWidget->vertex), -K * openGLWidget->joy_step);
      openGLWidget->update();
    } else if (ui->radioButtonRotate->isChecked()) {
      rotateOY(&(openGLWidget->vertex), K * openGLWidget->joy_step);
      openGLWidget->update();
    }
  }
}

void MainWindow::on_buttonRight_clicked() {
  if (openGLWidget->polygons_size != 0) {
    if (ui->radioButtonMove->isChecked()) {
      moveOX(&(openGLWidget->vertex), K * openGLWidget->joy_step);
      openGLWidget->update();
    } else if (ui->radioButtonRotate->isChecked()) {
      rotateOY(&(openGLWidget->vertex), -K * openGLWidget->joy_step);
      openGLWidget->update();
    }
  }
}

void MainWindow::on_buttonZ1_clicked() {
  if (openGLWidget->polygons_size != 0) {
    if (ui->radioButtonMove->isChecked()) {
      moveOZ(&(openGLWidget->vertex), K * openGLWidget->joy_step);
      openGLWidget->update();
    } else if (ui->radioButtonRotate->isChecked()) {
      rotateOZ(&(openGLWidget->vertex), -K * openGLWidget->joy_step);
      openGLWidget->update();
    }
  }
}

void MainWindow::on_buttonZ2_clicked() {
  if (openGLWidget->polygons_size != 0) {
    if (ui->radioButtonMove->isChecked()) {
      moveOZ(&(openGLWidget->vertex), -K * openGLWidget->joy_step);
      openGLWidget->update();
    } else if (ui->radioButtonRotate->isChecked()) {
      rotateOZ(&(openGLWidget->vertex), K * openGLWidget->joy_step);
      openGLWidget->update();
    }
  }
}

//************************************************************
//********************* SPIN BOX BLOCK ***********************
//************************************************************
void MainWindow::on_spinBoxLineWidth_valueChanged(int arg1) {
  openGLWidget->line_width = arg1;
  ui->statusBar->showMessage("Line width: " + QString::number(arg1), 3000);
  openGLWidget->update();
}

void MainWindow::on_spinBoxPointWidth_valueChanged(int arg1) {
  openGLWidget->point_width = arg1;
  ui->statusBar->showMessage("Point width: " + QString::number(arg1), 3000);
  openGLWidget->update();
}
void MainWindow::on_spinBoxStep_valueChanged(int arg1) {
  openGLWidget->joy_step = arg1;
  ui->statusBar->showMessage("Step: " + QString::number(arg1), 3000);
  openGLWidget->update();
}
