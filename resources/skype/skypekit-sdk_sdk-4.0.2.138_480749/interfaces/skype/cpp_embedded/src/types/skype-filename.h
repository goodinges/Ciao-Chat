
#ifndef __SKYPE_FILENAME_H
#define __SKYPE_FILENAME_H

#include "skype-string.h"

/*! \class SEFilename skype-filename.h "skype-filename.h"
 *  \brief SEFilename Represents a file name. Currently not too much different from SEString.
 */
class SEFilename  : public SEString
{
public:
        SEFilename() : SEString() {}
        SEFilename(const SEString &s) : SEString(s) {}
        SEFilename(const SEFilename &s) : SEString(s) {}
        SEFilename(const char *s) : SEString(s) {}
        SEFilename(unsigned int buf) : SEString(buf) {}
};

#endif
