#include "MainWindow.hpp"
#include <QMessageBox>
#include <QDialog>

MainWindow::MainWindow() :
  QMainWindow(0)
{
  setGeometry(0,0,800,600);
  m_connector_graph = new IOConnectorGraph(this);
  setWindowTitle("ALSA Client Connections");
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
  Q_UNUSED(event)
  m_connector_graph->setGeometry(0,0,width(),height());
}
