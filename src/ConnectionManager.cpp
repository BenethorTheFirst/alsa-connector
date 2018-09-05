#include "ConnectionManager.hpp"

ConnectionManager::ConnectionManager()
{
  
}

ConnectionManager::~ConnectionManager()
{
  qDeleteAll(m_tree_inputs_widget_list.begin(), m_tree_inputs_widget_list.end());
}

void ConnectionManager::initialize()
{
  if (aconnect_enumerate_all_clients())
  {
    m_connection_input_list = aconnect_get_input_connection_list();
    m_connection_output_list = aconnect_get_output_connection_list();
  }
  else
  {
    QMessageBox messageBox;
    messageBox.setText("Failed to enumerate devices");
    messageBox.exec();
  }
}

bool ConnectionManager::populateInputsList()
{
  //list clients
  for (auto &c : m_connection_input_list->clients)
  {
    QTreeWidgetItem *parent = new QTreeWidgetItem(
        (QTreeWidget*)0, QStringList(QString(c.name.c_str()))
      );
    for (auto &p : c.ports)
    {
      QTreeWidgetItem* child = new QTreeWidgetItem(
        QStringList(QString("%1 (%2, %3)").arg(p.name.c_str()).arg(p.num).arg(p.type == SND_SEQ_USER_CLIENT ? "user" : "kernel"))
      );
      parent->addChild(child);
    }
    m_tree_inputs_widget_list.append(parent);
  }
  return true;
}

bool ConnectionManager::populateOutputsList()
{
  //list clients
  for (auto &c : m_connection_output_list->clients)
  {
    QTreeWidgetItem *parent = new QTreeWidgetItem(
        (QTreeWidget*)0, QStringList(QString(c.name.c_str()))
      );
    for (auto &p : c.ports)
    {
      QTreeWidgetItem* child = new QTreeWidgetItem(
        QStringList(QString("%1 (%2, %3)").arg(p.name.c_str()).arg(p.num).arg(p.type == SND_SEQ_USER_CLIENT ? "user" : "kernel"))
      );
      parent->addChild(child);
    }
    m_tree_outputs_widget_list.append(parent);
  }
  return true;
}

QList<QTreeWidgetItem*> ConnectionManager::treeInputsWidgetList()
{
  return m_tree_inputs_widget_list;
}

QList<QTreeWidgetItem*> ConnectionManager::treeOutputsWidgetList()
{
  return m_tree_outputs_widget_list;
}
