/*==============================================================================

  Program: 3D Slicer

  Copyright (c) 2010 Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Julien Finet, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// Qt includes
#include <QDebug>
#include <QMap>
#include <QMimeData>
#include <QSharedPointer>
#include <QStack>
#include <QStringList>
#include <QVector>

// OpenIGTLinkIF GUI includes
#include <qIGTLIOConnectorModel.h>

#include "vtkIGTLIOLogic.h"
#include "vtkIGTLIOConnector.h"

// OpenIGTLinkIF MRML includes
//#include "vtkMRMLIGTLConnectorNode.h"

// MRML includes
//#include <vtkMRMLScene.h>
//#include <vtkMRMLNode.h>
//#include <vtkMRMLDisplayableHierarchyNode.h>
//#include <vtkMRMLDisplayableNode.h>

// VTK includes
#include <vtkVariantArray.h>
#include "qIGTLIOConnectorModel.h"
#include <vtkEventQtSlotConnect.h>

//------------------------------------------------------------------------------
qIGTLIOConnectorModel::qIGTLIOConnectorModel(QObject *vparent)
  :QAbstractItemModel(vparent)
{
  this->Connections = vtkSmartPointer<vtkEventQtSlotConnect>::New();
  HeaderLabels = QStringList() << "Name" << "Type" << "Status" << "Hostname" << "Port";
}

//------------------------------------------------------------------------------
qIGTLIOConnectorModel::~qIGTLIOConnectorModel()
{
}

//-----------------------------------------------------------------------------
int qIGTLIOConnectorModel::columnCount(const QModelIndex& parent) const
{
  return HeaderLabels.size();
}

//-----------------------------------------------------------------------------
int qIGTLIOConnectorModel::rowCount(const QModelIndex& parent) const
{
  if (parent.column() > 0)
    return 0;

  // only topnode has children
  if (!parent.isValid())
    return Logic->GetNumberOfConnectors();

  return 0;
}

//------------------------------------------------------------------------------
QVariant qIGTLIOConnectorModel::data(const QModelIndex &index, int role) const
{
  if (role!=Qt::DisplayRole)
    return QVariant();

  vtkIGTLIOConnector* cnode = this->getNodeFromIndex(index);

  if (!cnode)
    {
    return QVariant();
    }
  switch (index.column())
    {
    case qIGTLIOConnectorModel::NameColumn:
      {
      return QString::fromStdString(cnode->GetName());
      break;
      }
    case qIGTLIOConnectorModel::TypeColumn:
      {
      Q_ASSERT(cnode->GetType() < vtkIGTLIOConnector::NUM_TYPE);
      return QString::fromStdString(vtkIGTLIOConnector::ConnectorTypeStr[cnode->GetType()]);
      break;
      }
    case qIGTLIOConnectorModel::StatusColumn:
      {
      Q_ASSERT(cnode->GetState() < vtkIGTLIOConnector::NUM_STATE);
      return QString::fromStdString(vtkIGTLIOConnector::ConnectorStateStr[cnode->GetState()]);
      break;
      }
    case qIGTLIOConnectorModel::HostnameColumn:
      {
      if (cnode->GetType() == vtkIGTLIOConnector::TYPE_CLIENT)
        {
        return QString::fromStdString(cnode->GetServerHostname());
        }
      else
        {
        return QString("--");
        }
      break;
      }
    case qIGTLIOConnectorModel::PortColumn:
      {
      return QString("%1").arg(cnode->GetServerPort());
      break;
      }
    default:
      break;
    }
  return QVariant();
}


//-----------------------------------------------------------------------------
Qt::ItemFlags qIGTLIOConnectorModel::flags(const QModelIndex &index) const
{
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

//-----------------------------------------------------------------------------
QVariant qIGTLIOConnectorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
      return HeaderLabels[section];
    }

  return QVariant();
}

//-----------------------------------------------------------------------------
QModelIndex qIGTLIOConnectorModel::index(int row, int column, const QModelIndex &parent) const
{
  if (row < Logic->GetNumberOfConnectors())
    {
      return createIndex(row, column, Logic->GetConnector(row));
    }

  return QModelIndex();
}

//-----------------------------------------------------------------------------
QModelIndex qIGTLIOConnectorModel::parent(const QModelIndex &index) const
{
  return QModelIndex();
}

//-----------------------------------------------------------------------------
void qIGTLIOConnectorModel::resetModel()
{
  this->beginResetModel();
  this->endResetModel();
}


//QStandardItem* qIGTLIOConnectorModel::insertNode(vtkMRMLNode* node, QStandardItem* parent, int row)
//{
//  QStandardItem* insertedItem = this->Superclass::insertNode(node, parent, row);
//  if (this->listenNodeModifiedEvent() &&
//      node->IsA("vtkMRMLIGTLConnectorNode"))
//    {
//    qvtkConnect(node, vtkMRMLIGTLConnectorNode::ConnectedEvent,
//                this, SLOT(onMRMLNodeModified(vtkObject*)));
//    qvtkConnect(node, vtkMRMLIGTLConnectorNode::DisconnectedEvent,
//                this, SLOT(onMRMLNodeModified(vtkObject*)));
//    qvtkConnect(node, vtkMRMLIGTLConnectorNode::ActivatedEvent,
//                this, SLOT(onMRMLNodeModified(vtkObject*)));
//    qvtkConnect(node, vtkMRMLIGTLConnectorNode::DeactivatedEvent,
//                this, SLOT(onMRMLNodeModified(vtkObject*)));
//    }
//  return insertedItem;
//}

////------------------------------------------------------------------------------
//void qIGTLIOConnectorModel::updateNodeFromItemData(vtkMRMLNode* node, QStandardItem* item)
//{
//  //int oldChecked = node->GetSelected();
//  vtkMRMLIGTLConnectorNode* cnode = vtkMRMLIGTLConnectorNode::SafeDownCast(node);
//  if (!cnode)
//    {
//    return;
//    }
//  this->qMRMLSceneDisplayableModel::updateNodeFromItemData(node, item);

//  switch (item->column())
//    {
//    case qIGTLIOConnectorModel::NameColumn:
//      {
//      cnode->SetName(item->text().toLatin1());
//      break;
//      }
//    case qIGTLIOConnectorModel::TypeColumn:
//      {
//      }
//    case qIGTLIOConnectorModel::StatusColumn:
//      {
//      }
//    case qIGTLIOConnectorModel::HostnameColumn:
//      {
//      }
//    case qIGTLIOConnectorModel::PortColumn:
//      {
//      }
//    default:
//      break;
//    }
//}

////------------------------------------------------------------------------------
//void qIGTLIOConnectorModel::updateItemDataFromNode(QStandardItem* item, vtkMRMLNode* node, int column)
//{
//  vtkMRMLIGTLConnectorNode* cnode = vtkMRMLIGTLConnectorNode::SafeDownCast(node);
//  if (!cnode)
//    {
//    return;
//    }
//  switch (column)
//    {
//    case qIGTLIOConnectorModel::NameColumn:
//      {
//      item->setText(cnode->GetName());
//      break;
//      }
//    case qIGTLIOConnectorModel::TypeColumn:
//      {
//      Q_ASSERT(cnode->GetType() < vtkMRMLIGTLConnectorNode::NUM_TYPE);
//      item->setText(QString(vtkMRMLIGTLConnectorNode::ConnectorTypeStr[cnode->GetType()]));
//      break;
//      }
//    case qIGTLIOConnectorModel::StatusColumn:
//      {
//      Q_ASSERT(cnode->GetState() < vtkMRMLIGTLConnectorNode::NUM_STATE);
//      item->setText(QString(vtkMRMLIGTLConnectorNode::ConnectorStateStr[cnode->GetState()]));
//      break;
//      }
//    case qIGTLIOConnectorModel::HostnameColumn:
//      {
//      if (cnode->GetType() == vtkMRMLIGTLConnectorNode::TYPE_CLIENT)
//        {
//        item->setText(QString(cnode->GetServerHostname()));
//        }
//      else
//        {
//        item->setText(QString("--"));
//        }
//      break;
//      }
//    case qIGTLIOConnectorModel::PortColumn:
//      {
//      item->setText(QString("%1").arg(cnode->GetServerPort()));
//      break;
//      }
//    default:
//      break;
//    }
//}

////------------------------------------------------------------------------------
//QFlags<Qt::ItemFlag> qIGTLIOConnectorModel::nodeFlags(vtkMRMLNode* node, int column)const
//{
//  QFlags<Qt::ItemFlag> flags = this->qMRMLSceneDisplayableModel::nodeFlags(node, column);
//  // remove the ItemIsEditable flag from any possible item (typically at column 0)
//  flags = flags & ~Qt::ItemIsEditable;
//  // and set it to the right column
//  switch(column)
//    {
//    //case qIGTLIOConnectorModel::TextColumn:
//    //  flags = flags | Qt::ItemIsEditable;
//    //  break;
//    default:
//      break;
//    }
//  return flags;
//}

////------------------------------------------------------------------------------
//vtkMRMLNode* qIGTLIOConnectorModel::parentNode(vtkMRMLNode* node)const
//{
//  if (node == NULL)
//    {
//    return 0;
//    }

//  // MRML Displayable nodes (inherits from transformable)
//  vtkMRMLDisplayableNode *displayableNode = vtkMRMLDisplayableNode::SafeDownCast(node);
//  vtkMRMLDisplayableHierarchyNode * displayableHierarchyNode = NULL;
//  if (displayableNode &&
//      displayableNode->GetScene() &&
//      displayableNode->GetID())
//    {
//    // get the displayable hierarchy node associated with this displayable node
//    displayableHierarchyNode = vtkMRMLDisplayableHierarchyNode::GetDisplayableHierarchyNode(displayableNode->GetScene(), displayableNode->GetID());

//    if (displayableHierarchyNode)
//      {
//      if (displayableHierarchyNode->GetHideFromEditors())
//        {
//        // this is a hidden hierarchy node, so we do not want to display it
//        // instead, we will return the parent of the hidden hierarchy node
//        // to be used as the parent for the displayableNode
//        return displayableHierarchyNode->GetParentNode();
//        }
//      return displayableHierarchyNode;
//      }
//    }
//  if (displayableHierarchyNode == NULL)
//    {
//    // the passed in node might have been a hierarchy node instead, try to
//    // cast it
//    displayableHierarchyNode = vtkMRMLDisplayableHierarchyNode::SafeDownCast(node);
//    }
//  if (displayableHierarchyNode)
//    {
//    // return it's parent
//    return displayableHierarchyNode->GetParentNode();
//    }
//  return 0;
//}


//-----------------------------------------------------------------------------
void qIGTLIOConnectorModel::setLogic(vtkIGTLIOLogicPointer logic)
{
  this->Logic = logic;

  this->Connections->Connect(Logic,
                             vtkIGTLIOLogic::ConnectionAddedEvent,
                             this,
                             SLOT(onConnectionEvent(vtkObject*, unsigned long, void*, void*)));
  this->Connections->Connect(Logic,
                             vtkIGTLIOLogic::ConnectionAboutToBeRemovedEvent,
                             this,
                             SLOT(onConnectionEvent(vtkObject*, unsigned long, void*, void*)));
}

//-----------------------------------------------------------------------------
void qIGTLIOConnectorModel::onConnectionEvent(vtkObject* caller, unsigned long event , void*, void*)
{
  if (event==vtkIGTLIOLogic::ConnectionAddedEvent)
    {
      std::cout << "on dd connected event" << std::endl;
      this->resetModel();
    }
  if (event==vtkIGTLIOLogic::ConnectionAboutToBeRemovedEvent)
    {
      std::cout << "on remove connected event" << std::endl;
      this->resetModel();
    }
}

//-----------------------------------------------------------------------------
vtkIGTLIOConnector* qIGTLIOConnectorModel::getNodeFromIndex(const QModelIndex &index) const
{
  if (!index.isValid())
    return NULL;
  return static_cast<vtkIGTLIOConnector*>(index.internalPointer());
}
