
#include "../include/VDVInterpreter.h"

#include "lib/utility/include/Logging.h"

namespace uic918::detail
{

  VDVInterpreter::VDVInterpreter(::utility::LoggerFactory &lf)
      : logger(CREATE_LOGGER(lf))
  {
  }

  Context VDVInterpreter::interpret(Context &&context)
  {
    // Documentation: https://www.kcd-nrw.de/fileadmin/03_KC_Seiten/KCD/Downloads/Technische_Dokumente/Archiv/2010_02_12_kompendiumvrrfa2dvdv_1_4.pdf
    // Reference-Impl: https://sourceforge.net/projects/dbuic2vdvbc/

    LOG_WARN(logger) << "Unsupported VDV barcode detected";
    return std::move(context);
  }
}
