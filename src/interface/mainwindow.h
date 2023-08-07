#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "glview.h"
#include <QColorDialog>
#include <QFileDialog>
#include <QMainWindow>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void saveSettings();
  void loadSettings();

private:
  Ui::MainWindow *ui;
  GlView *openGLWidget;
  QSettings *settings;
  QColor color;

  QTimer *timer;
  QImage frames[50];
  QString screenFolder;
  QString screenTime;
  int count;

  bool choiceScreenFolder();
  void joyMove();
  void joyRotate();
  void joyZoom();

public slots:
  void slotTextFromGlView(const QString &message);
  void updateFrames();

private slots:
  void setBGColor(QString color_hex_string);
  void setLineColor(QString color_hex_string);
  void setPointColor(QString color_hex_string);

  void on_buttonOpen_clicked();
  void on_buttonBmp_clicked();
  void on_buttonJpg_clicked();
  void on_buttonGif_clicked();

  void on_checkBoxOrtho_stateChanged(int arg1);
  void on_radioButtonSolid_toggled(bool checked);
  void on_radioButtonStripple_toggled(bool checked);
  void on_spinBoxLineWidth_valueChanged(int arg1);
  void on_buttonBGColor_clicked();
  void on_buttonLineColor_clicked();
  void on_buttonPointColor_clicked();
  void on_spinBoxPointWidth_valueChanged(int arg1);
  void on_radioButtonPoint_toggled(bool checked);
  void on_checkBoxRound_stateChanged(int arg1);
  void on_radioButtonMove_toggled(bool checked);
  void on_radioButtonRotate_toggled(bool checked);
  void on_radioButtonZoom_toggled(bool checked);

  void on_buttonUp_clicked();
  void on_buttonDown_clicked();
  void on_buttonLeft_clicked();
  void on_buttonRight_clicked();
  void on_spinBoxStep_valueChanged(int arg1);
  void on_buttonZ1_clicked();
  void on_buttonZ2_clicked();
};

#endif // MAINWINDOW_H
