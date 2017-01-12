#ifndef VTKIGTLIOTRANSLATOR_H
#define VTKIGTLIOTRANSLATOR_H

#include "vtkObject.h"

#include "igtlioToolsExport.h"

namespace igtlio
{

class OPENIGTLINKIO_TOOLS_EXPORT vtkIGTLIOTranslator
{
public:
    vtkIGTLIOTranslator();

    std::string GetToolNameFromDeviceName(std::string device_name);
    std::string DetermineTypeBasedOnToolName(std::string tool_name);

};

} //namespace igtlio

#endif // VTKIGTLIOTRANSLATOR_H
