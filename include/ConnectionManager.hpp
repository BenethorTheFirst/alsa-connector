#include <QtCore/QVariant>
#include "aconnect.hpp"
#include <QTreeWidgetItem>
#include <QMessageBox>

//Manages ALSA device connections and provides AbstractListModel's
class ConnectionManager
{
public:
  ConnectionManager();
  ~ConnectionManager();
  void initialize();
  bool populateInputsList();
  bool populateOutputsList();
  QList<QTreeWidgetItem*> treeInputsWidgetList();
  QList<QTreeWidgetItem*> treeOutputsWidgetList();
private:
  alsa_connection_list* m_connection_input_list;
  alsa_connection_list* m_connection_output_list;
  QList<QTreeWidgetItem*> m_tree_inputs_widget_list;
  QList<QTreeWidgetItem*> m_tree_outputs_widget_list;
};
