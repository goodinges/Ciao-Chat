
#ifndef __SKYPE_FILENAME_LIST_H
#define __SKYPE_FILENAME_LIST_H

#include "skype-string-list.h"
#include "skype-filename.h"

/*! \class SEFilenameList skype-filename-list.h "skype-filename-list.h"
 *  \brief SEFilenameList Represents string list with file names in it.
 */
class SEFilenameList  : public SEStringList
{
public:
        SEFilenameList() : SEStringList() {}
        SEFilenameList(const SEFilenameList& fl) : SEStringList(fl) {}
        SEFilenameList(const SEStringList& sl) : SEStringList(sl) {}

        ~SEFilenameList() {}

        /** Returns a string reference specified by its index. */
        const SEFilename &operator[](size_t n) const {
                return (const SEFilename &) SEStringList::operator[](n);
        }

};

#endif
