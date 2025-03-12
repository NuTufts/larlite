#include "DataFormatConstants.h"

/// Namespace of everything in this framework
namespace larlite{
  
  /// Defines constants for data structure definition (system utility)
  namespace data{

    const std::string& GetProductName(const larlite::data::DataType_t t)
    {
      return larlite::data::kDATA_TREE_NAME[t];
    }
    
  }
}
