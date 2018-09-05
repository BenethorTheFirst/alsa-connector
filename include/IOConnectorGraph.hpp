#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>
#include <QPaintEvent>
#include "ConnectionManager.hpp"

class IOConnectorGraph : public QWidget
{
public:
  IOConnectorGraph(QWidget* parent);
  ~IOConnectorGraph();
protected:
  void paintEvent(QPaintEvent* event);
private:
  void setupUi();
  ConnectionManager m_connectionManager; 
  QGridLayout *m_gridLayout;
  QTreeWidget *m_treeInputs;
  QTreeWidget *m_treeOutputs;
  QPushButton *m_buttonConnectDisconnect;
  QSpacerItem *m_horizontalSpacer;
};
