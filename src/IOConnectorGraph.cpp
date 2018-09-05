#include "IOConnectorGraph.hpp"
#include <QPainter>
#include <QRect>
#include <QDebug>

IOConnectorGraph::IOConnectorGraph(QWidget* parent = 0)
  : QWidget(parent)
{
  if (parent)
    setGeometry(parent->geometry());
  m_connectionManager.initialize();
  m_connectionManager.populateInputsList();
  m_connectionManager.populateOutputsList();
  setupUi();
  setStyleSheet("QWidget { background: #f5f5ee; } ");
  m_treeInputs->setFont(QFont("Arial",15));
  m_treeInputs->setStyleSheet("QTreeWidget::item, QTreeWidgetItem { font-size: 15px !important; padding: 5px; }");
  m_treeOutputs->setFont(QFont("Arial",15));
  m_treeOutputs->setStyleSheet("QTreeWidget::item { font-size: 15px; padding: 5px; }");
}

IOConnectorGraph::~IOConnectorGraph()
{
  
}

void IOConnectorGraph::paintEvent(QPaintEvent* event)
{
  Q_UNUSED(event)
  QPainter painter(this);
  
}

void IOConnectorGraph::setupUi()
{
  IOConnectorGraph* Form = this;
  if (Form->objectName().isEmpty())
    Form->setObjectName(QStringLiteral("Form"));
  Form->resize(616, 462);
  m_gridLayout = new QGridLayout(Form);
  m_gridLayout->setObjectName(QStringLiteral("gridLayout"));
  m_treeInputs = new QTreeWidget(Form);
  m_treeInputs->setHeaderItem(new QTreeWidgetItem(QStringList("Inputs")));
  m_treeInputs->headerItem()->setFont(0, QFont("Arial",15,QFont::Bold));
  m_treeInputs->insertTopLevelItems(0,m_connectionManager.treeInputsWidgetList());
  m_treeInputs->setObjectName(QStringLiteral("m_treeInputs"));
  m_gridLayout->addWidget(m_treeInputs, 0, 0, 1, 1);
  
  m_horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
  m_gridLayout->addItem(m_horizontalSpacer, 0, 1, 1, 1);
  
  m_treeOutputs = new QTreeWidget(Form);
  m_treeOutputs->setHeaderItem(new QTreeWidgetItem(QStringList("Outputs")));
  m_treeOutputs->headerItem()->setFont(0, QFont("Arial",15,QFont::Bold));
  m_treeOutputs->insertTopLevelItems(0,m_connectionManager.treeOutputsWidgetList());
  m_treeOutputs->setObjectName(QStringLiteral("m_treeOutputs"));
  m_gridLayout->addWidget(m_treeOutputs, 0, 2, 1, 1);
  
  m_buttonConnectDisconnect = new QPushButton(Form);
  m_buttonConnectDisconnect->setObjectName(QStringLiteral("m_buttonConnectDisconnect"));
  m_buttonConnectDisconnect->setText("Connect/Disconnect");
  m_buttonConnectDisconnect->setFont(QFont("Arial",15));
  m_gridLayout->addWidget(m_buttonConnectDisconnect, 1, 0, 1, 3);
  QMetaObject::connectSlotsByName(Form);
} // setupUi
