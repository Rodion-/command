#include <gtest/gtest.h>
#include "command.h"
#include <queue>

TEST( test1 , test )
{
    std::shared_ptr<UObject> spaceship;
    CheckFuelAdapter CheckFuelCommand( spaceship );

    CheckFuelCommand.set_fuel_for_test( 100 );

    EXPECT_NO_THROW (  CheckFuelCommand.execute() );

    CheckFuelCommand.set_fuel_for_test( 0 );

    EXPECT_THROW( CheckFuelCommand.execute() , Exception );
}

TEST( test2 , test )
{
    std::shared_ptr<UObject> spaceship;
    BurnFuelAdapter BurnFuelCommand( spaceship , 100 );

    BurnFuelCommand.execute();
    int fuel = BurnFuelCommand.get_fuel_for_test();

    EXPECT_EQ ( fuel , 99 );
}

TEST( test3 , test )
{
    std::shared_ptr<UObject> spaceship;
    velocity vel = { 1 , 2 };
    VelocityAdapter ChangeVelocityCommand( spaceship , vel );

    ChangeVelocityCommand.set_velocity_for_test( velocity{ 3 , 4 } );
    ChangeVelocityCommand.execute();

    ChangeVelocityCommand.get_velocity_for_test( &vel );

    EXPECT_EQ ( vel.x , 4 );
    EXPECT_EQ ( vel.y , 6 );
}

TEST( test4 , test )
{
    std::shared_ptr<UObject> spaceship;
    ExceptionHandler exceptionhandler;
    ExceptionStaus ex_status = ExceptionStaus::NOTPROCESSED;

    try
    {
        exceptionhandler.Setup( "Rotation",
                         ExceptionType::ArgumentException,
                         [](){ std::cout<<"Exception Rotation" <<  std::endl; }
                       );

        exceptionhandler.Setup( "CommandException",
                         ExceptionType::TYPE2Exception,
                         [](){ std::cout<<"CommandException" <<  std::endl; }
                       );



        macroCommandMove MCommandMove( spaceship , ( position ){ 1 , 2 } , (velocity){ 1 , 1 } , 100 );
        MCommandMove.execute();

        macroCommandRotate MCommandRotate( spaceship , 0 , 100 , { 1 , -1 } );
        MCommandRotate.execute();
    }
    catch ( Exception ex )
    {
        ex_status = exceptionhandler.Handler( ex.cmd_type , ex.ex_type );
    }

    EXPECT_EQ( (int)ex_status , ( int ) ExceptionStaus::PROCESSED );
}
