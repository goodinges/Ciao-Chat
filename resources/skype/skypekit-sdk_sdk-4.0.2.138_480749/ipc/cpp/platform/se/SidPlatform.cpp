#include "SidPlatform.hpp"

namespace Sid
{       
        namespace Platform
        {        
                void list_remove(List_int* list, int i) { list->remove_pos(i); }
                void list_remove(List_String* list, int i) { list->remove_pos(i); }   
                void list_remove(List_Filename* list, int i) { list->remove_pos(i); }   
                void list_remove(List_ref* list, int i) { list->remove_pos(i); }                    
        }
}
