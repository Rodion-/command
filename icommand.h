#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <iostream>

class ICommand
{
    public:

    virtual void execute ( void ) = 0;
};

#endif // ICOMMAND_H
