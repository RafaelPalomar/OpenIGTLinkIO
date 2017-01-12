#ifndef IGTLIOFIXTURE_H
#define IGTLIOFIXTURE_H


#include <string>
#include "vtkIGTLIOLogic.h"
#include "vtkIGTLIOConnector.h"
#include "vtkTimerLog.h"
#include "vtkIGTLIOImageDevice.h"
#include "vtkIGTLIOCommandDevice.h"
#include "vtkIGTLIOTranslator.h"

class vtkImageData;

struct LogicFixture
{
  LogicFixture();

  void startClient();
  void startServer();

  igtlio::vtkIGTLIOLogicPointer Logic;
  igtlio::vtkIGTLIOConnectorPointer Connector;
  igtlio::vtkIGTLIOSessionPointer Session;

  vtkSmartPointer<class vtkCallbackCommand> LogicEventCallback;
  std::vector<int> ReceivedEvents;
};

struct ClientServerFixture
{
  LogicFixture Server;
  LogicFixture Client;
  igtlio::vtkIGTLIOTranslator Translator;

  bool ConnectClientToServer();
  bool LoopUntilEventDetected(LogicFixture *logic, int eventId, int count=1);

public:
  vtkSmartPointer<vtkMatrix4x4> CreateTestTransform();
  vtkSmartPointer<vtkImageData> CreateTestImage();
};


#endif // IGTLIOFIXTURE_H
