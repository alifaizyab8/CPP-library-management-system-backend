#pragma once;
#include "../domain/User.h"
class AuthorizationService
{
public:
    void ensureLibrarian(const User &user) const;
    
};