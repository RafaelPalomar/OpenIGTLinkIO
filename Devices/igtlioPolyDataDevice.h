/*==========================================================================

  Portions (c) Copyright 2008-2009 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See Doc/copyright/copyright.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Program:   3D Slicer
  Module:    $HeadURL: https://github.com/openigtlink/OpenIGTLinkIF/blob/master/MRML/vtkIGTLToMRMLPolyData.h $
  Date:      $Date: 2010-11-23 00:58:13 -0500 (Tue, 23 Nov 2010) $
  Version:   $Revision: 15552 $

==========================================================================*/


#ifndef IGTLIOPOLYDATADEVICE_H
#define IGTLIOPOLYDATADEVICE_H

#include "igtlioDevicesExport.h"

#include "igtlioPolyDataConverter.h"
#include "igtlioDevice.h"

namespace igtlio
{
class vtkPolyDataData;
typedef vtkSmartPointer<class PolyDataDevice> PolyDataDevicePointer;

class OPENIGTLINKIO_DEVICES_EXPORT PolyDataDevice : public Device
{
public:

  enum {
    PolyDataModifiedEvent         = 118958,
  };
  
 virtual vtkIntArray* GetDeviceContentModifiedEvent() const;
 virtual std::string GetDeviceType() const;
 virtual int ReceiveIGTLMessage(igtl::MessageBase::Pointer buffer, bool checkCRC);
 virtual igtl::MessageBase::Pointer GetIGTLMessage();
 virtual igtl::MessageBase::Pointer GetIGTLMessage(MESSAGE_PREFIX prefix);
 virtual std::set<MESSAGE_PREFIX> GetSupportedMessagePrefixes() const;

  void SetContent(PolyDataConverter::ContentData content);
  PolyDataConverter::ContentData GetContent();

 public:
  static PolyDataDevice *New();
  vtkTypeMacro(PolyDataDevice,Device);

  void PrintSelf(ostream& os, vtkIndent indent);

 protected:
  PolyDataDevice();
  ~PolyDataDevice();

 protected:
  igtl::PolyDataMessage::Pointer OutMessage;
  igtl::GetPolyDataMessage::Pointer GetMessage;

  PolyDataConverter::ContentData Content;
};


//---------------------------------------------------------------------------
class OPENIGTLINKIO_DEVICES_EXPORT PolyDataDeviceCreator : public DeviceCreator
{
public:
  virtual DevicePointer Create(std::string device_name);
  virtual std::string GetDeviceType() const;

  static PolyDataDeviceCreator *New();
  vtkTypeMacro(PolyDataDeviceCreator,vtkObject);

};

} // namespace igtlio

#endif // IGTLIOPOLYDATADEVICE_H
