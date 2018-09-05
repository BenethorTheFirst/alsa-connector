#include <QtWidgets/QWidget>
#include <QPaintEvent>
#include <QImage>
#include <vector>

using std::vector;

class IOConnectorGraph : public QWidget
{
public:
  IOConnectorGraph(QWidget* parent);
  ~IOConnectorGraph();
  class NodeChild
  {
  public:
    NodeChild(const QString& name);
    void setName(const QString& name);
    const QString& name() const;
    QRect m_display_rect;
  private:
    QString m_name;
    
  };
  class NodeGroup
  {
  public:
    NodeGroup(const QString name);
    void setName(const QString& name);
    const QString name() const;
    void addChild(const IOConnectorGraph::NodeChild childNode);
    void setStatus(const int status);
    const int status() const;
    class Status
    {
    public:
      enum {
        NORMAL,
        EXPANDED
      };
    };
    QRect m_display_rect;
  private:
    QString m_name;
    int m_status;
    vector<IOConnectorGraph::NodeChild> m_child_nodes;
  };
  struct Connection
  {
    int input_group_id;
    int output_group_id;
    int input_id;
    int output_id;
  };
  void appendInputNodeGroup(const NodeGroup nodeGroup);
  void appendOutputNodeGroup(const NodeGroup nodeGroup);
  void setIcons(QImage* expandIcon, QImage* collapseIcon);
protected:
  void paintEvent(QPaintEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;
private:
  QRect m_left_rect, m_right_rect;
  vector<NodeGroup> m_input_node_groups;
  vector<NodeGroup> m_output_node_groups;
  vector<Connection> m_connections;
  QImage* m_expand_icon;
  QImage* m_collapse_icon;
};
