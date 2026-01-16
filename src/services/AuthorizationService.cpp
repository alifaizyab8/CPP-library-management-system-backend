#include "AuthorizationService.h"
#include <stdexcept>
void AuthorizationService::ensureLibrarian(const User &user) const
{
    if (user.getRole() != Role::Librarian)
    {
        throw std::runtime_error("Access denied. Not a Librarian");
    }
}