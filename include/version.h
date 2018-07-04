/*
 * version.h
 *
 *  Created on: 21 Sep 2017
 *      Author: Menno
 */

#ifndef INCLUDE_FTB_VERSION_H_
#define INCLUDE_FTB_VERSION_H_


#include <string>

namespace application
{
  struct Version
  {
    static const std::string GIT_SHA1;
    static const std::string GIT_BRANCH;
    static const std::string GIT_DATE;
    static const std::string GIT_COMMIT_SUBJECT;
  };
}


#endif /* INCLUDE_FTB_VERSION_H_ */

