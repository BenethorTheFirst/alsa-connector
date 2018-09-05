#include "IOConnectorGraph.hpp"
#include <QPainter>
#include <QRect>

IOConnectorGraph::IOConnectorGraph(QWidget* parent = 0)
  : QWidget(parent)
{
  setStyleSheet("QWidget { background: #f5f5ee; } ");
  if (parent)
    setGeometry(parent->geometry());
}

IOConnectorGraph::~IOConnectorGraph()
{
  if (m_expand_icon) {
    delete m_expand_icon;
  }
  if (m_collapse_icon) {
    delete m_collapse_icon;
  }
}

void IOConnectorGraph::appendInputNodeGroup(const IOConnectorGraph::NodeGroup nodeGroup)
{
  m_input_node_groups.push_back(nodeGroup);
}

void IOConnectorGraph::appendOutputNodeGroup(const IOConnectorGraph::NodeGroup nodeGroup)
{
  m_output_node_groups.push_back(nodeGroup);
}

void IOConnectorGraph::setIcons(QImage* expandIcon, QImage* collapseIcon)
{
  m_expand_icon = expandIcon;
  m_collapse_icon = collapseIcon;
}

void IOConnectorGraph::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);
  const int font_size = 16;
  const int row_font_size = 12;
  const int row_space = 12;
  const int indent_width = 2 * font_size;
  painter.setFont(QFont("Arial", font_size, QFont::Bold));
  painter.drawText(m_left_rect, Qt::AlignLeft, "Inputs");
  painter.drawText(m_right_rect, Qt::AlignLeft, "Outputs");
  //draw input nodegroups and children
  using NodeGroupV = vector<IOConnectorGraph::NodeGroup>;
  NodeGroupV::const_iterator iter;
  QRect node_rect = m_left_rect;
  node_rect.setLeft(node_rect.left() + indent_width);
  painter.setFont(QFont("Arial", row_font_size));
  for (iter = m_input_node_groups.cbegin();
       iter != m_input_node_groups.cend(); iter++)
  {
    node_rect.moveTop(node_rect.y() + font_size + row_space);
    painter.drawText(node_rect, Qt::AlignLeft, iter->name());
    painter.drawImage(node_rect.left() - indent_width, node_rect.y(), *m_expand_icon);
    //draw expander
    if (iter->status() == IOConnectorGraph::NodeGroup::Status::NORMAL) 
    {
      if (!m_expand_icon) 
      {
        painter.drawImage(node_rect.left() - indent_width, node_rect.y(), *m_expand_icon);
      }
      else
      {
        node_rect.moveLeft(node_rect.left() - indent_width);
        painter.drawText(node_rect, Qt::AlignLeft, "+");
        node_rect.moveLeft(node_rect.left() + indent_width);
      }
    }
    else if (iter->status() == IOConnectorGraph::NodeGroup::Status::EXPANDED)
    {
      if (!m_collapse_icon)
      {
        painter.drawImage(node_rect.left() - indent_width, node_rect.y(), *m_collapse_icon);
      }
      else
      {
        node_rect.moveLeft(node_rect.left() - indent_width);
        painter.drawText(node_rect, Qt::AlignLeft, "-");
        node_rect.moveLeft(node_rect.left() + indent_width);
      }
    }
    
  }
  node_rect = m_right_rect;
  node_rect.setLeft(node_rect.left() + indent_width);
  //draw output nodegroups and children
  for (iter = m_output_node_groups.cbegin();
       iter != m_output_node_groups.cend(); iter++)
  {
    node_rect.moveTop(node_rect.y() + font_size + row_space);
    painter.drawText(node_rect, Qt::AlignLeft, iter->name());
  }
}

void IOConnectorGraph::resizeEvent(QResizeEvent* event)
{
  m_left_rect.setRect(0,0,width()/2,height());
  m_right_rect.setRect(width()/2,0,width()/2,height());
}

IOConnectorGraph::NodeChild::NodeChild(const QString& name)
{
  setName(name);
}

void IOConnectorGraph::NodeChild::setName(const QString& name)
{
  m_name = name;
}

const int IOConnectorGraph::NodeChild::displayY() const
{
  return m_display_y;
}

const QString IOConnectorGraph::NodeGroup::name() const
{
  return m_name;
}

IOConnectorGraph::NodeGroup::NodeGroup(const QString name)
{
  setName(name);
}

void IOConnectorGraph::NodeGroup::setName(const QString& name)
{
  m_name = name;
}

void IOConnectorGraph::NodeGroup::addChild(IOConnectorGraph::NodeChild childNode)
{
  m_child_nodes.push_back(childNode);
}

const int IOConnectorGraph::NodeGroup::status() const
{
  return m_status;
}

