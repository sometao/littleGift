#include "httplib.h"

namespace httplib {

class HttpUtils {
public:

  using FrontHandler = std::function<bool(const Request&, Response&)>;
  using Handler = std::function<void(const Request&, Response&)>;

}

}  // namespace httplib
