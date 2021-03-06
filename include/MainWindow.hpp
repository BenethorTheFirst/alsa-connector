#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "IOConnectorGraph.hpp"

class MainWindow : public QMainWindow
{
public:
  MainWindow();
  IOConnectorGraph* m_connector_graph;
protected:
  void resizeEvent(QResizeEvent* event) override;
};
