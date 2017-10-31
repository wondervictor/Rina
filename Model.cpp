/**
 * @brief Model
 * @author Vic Chan
 * @date 2017-10-30
 * */

#include "Model.h"

namespace Rina {

MessageType Message::getType() {
  std::string content = this->content;
  if (content == LOGIN) {
    return login;
  } else if (content == LOGOUT) {
    return logout;
  } else if (content == GET_ALL) {
    return getall;
  } else {
    return normal;
  }
}

}