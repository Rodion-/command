#ifndef COMMAND_H
#define COMMAND_H

#include "icommand.h"
#include "exception.h"
#include <iostream>
#include <memory>

typedef struct position
{
    int x;
    int y;
} position;

typedef struct velocity
{
    int x;
    int y;
} velocity;


class UObject   //
{

    position coordinates;
    velocity v;

    int angle;


    public:

    UObject(){}
    ~UObject(){}

    void set_property ( std::string property )
    {
    }

    void get_property ( std::string property )
    {
    }

};

class CheckFuelAdapter : public ICommand
{
    std::shared_ptr<UObject> ob;

    int fuel;

    std::string cmdtype;

    public:

    CheckFuelAdapter( std::shared_ptr<UObject> _ob ) : ob( _ob )
    {
        cmdtype = "CheckFuel" ;
    }
    ~CheckFuelAdapter(){}

    void set_fuel_for_test( int f )
    {
        fuel = f;
    }

     void execute(  void ) override
     {
        //  fuel = ob->get_property("fuel");

         if( fuel == 0 ) throw Exception{ cmdtype , ExceptionType::ArgumentException , ExceptionStaus::NOTPROCESSED };
//         std::cout<< "CheckFuelAdapter : " << std::endl;
     }
};

class BurnFuelAdapter : public ICommand
{
    std::shared_ptr<UObject> ob;

    int fuel;

    std::string cmdtype;

    public:

    BurnFuelAdapter( std::shared_ptr<UObject> _ob , int _fuel ) : ob( _ob ) , fuel( _fuel )
    {
        cmdtype = "BurnFuel" ;
    }
    ~BurnFuelAdapter(){}

    int get_fuel_for_test( void )
    {
        return fuel;
    }


     void execute(  void ) override
     {
         fuel -= 1;

         ob->set_property("fuel");
//         std::cout<< "BurnFuelAdapter : " <<  fuel << std::endl;
     }
};

class MoveAdapter : public ICommand
{
    std::shared_ptr<UObject> ob;

    std::string cmdtype;

    //----mock part
    position pos;
    velocity vel;
    //----

    public:

    MoveAdapter( std::shared_ptr<UObject> _ob , position _pos , velocity _vel) : ob( _ob ) , pos( _pos ) , vel( _vel )
    {
        cmdtype = "Move" ;
    }
    ~MoveAdapter(){}

    void execute(  void ) override
    {
        if( !pos.x && !pos.y ) throw -1;
        if( vel.x && !vel.y ) throw -1;
        if( !pos.x && !pos.y && vel.x && !vel.y ) throw -1;

        pos.x = pos.x + vel.x;
        pos.y = pos.y + vel.y;

        std::cout<< "MoveAdapter : " <<  pos.x << " " << pos.y << std::endl;

        //ob->set_property();
    }

    void set_position( position* xy )
    {
        pos = *xy;
    }

    void get_position( position* xy )
    {
        xy->x = pos.x;
        xy->y = pos.y;
    }

    void set_velocity( velocity* v )
    {
        vel = *v;
    }

    void get_velocity( velocity* v )
    {
        v->x = vel.x;
        v->y = vel.y;
    }
};

class VelocityAdapter : public ICommand
{
    std::shared_ptr<UObject> ob;

    std::string cmdtype;

    //----mock part
    velocity vel;
    velocity ob_vel;
    //----

    public:

    VelocityAdapter( std::shared_ptr<UObject> _ob , velocity _vel) : ob( _ob ) , vel( _vel )
    {
        cmdtype = "Velocity" ;
    }
    ~VelocityAdapter(){}

    void execute(  void ) override
    {
        if( vel.x && !vel.y ) throw Exception{ cmdtype , ExceptionType::ArgumentException , ExceptionStaus::NOTPROCESSED };

        ob->get_property("is_movable");

        ob->get_property("velocity");        

        ob_vel.x += vel.x;
        ob_vel.y += vel.y;

        ob->set_property("velocity");
    }

    void set_velocity_for_test( velocity v )
    {
        ob_vel = v;
    }

    void get_velocity_for_test( velocity* v )
    {
        v->x = ob_vel.x;
        v->y = ob_vel.y;
    }
};

class RotationAdapter : public ICommand
{
    std::shared_ptr<UObject> ob;

    std::string cmdtype;
    //---Mock
    int angle;
    //-------

    public:

    RotationAdapter( std::shared_ptr<UObject> _ob , int _angle) : ob( _ob ) , angle( _angle )
    {
        cmdtype = "Rotation" ;
    }
    ~RotationAdapter(){}

    void set_angle( int  a )
    {
        angle = a;
    }

    void get_angle( int* a )
    {
        *a = angle;
    }

    void execute() override
    {
        std::cout<< "RotationAdapter : " <<  angle << std::endl;
        //ob->set_property();
        if( angle == 0 ) throw Exception{ cmdtype , ExceptionType::ArgumentException , ExceptionStaus::NOTPROCESSED };
    }
};

class macroCommandMove
{
    std::shared_ptr<UObject> ob;
    position xy;
    velocity vel;

    int fuel_level;

    public:

    macroCommandMove( std::shared_ptr<UObject> _ob , position _xy , velocity _vel , int _fuel_level )
        : ob( _ob ) , xy( _xy ) , vel( _vel ) , fuel_level( _fuel_level )
    {

    }

    ~macroCommandMove(){}

    void execute ( void )
    {
        BurnFuelAdapter BurnFuelCommand( ob , fuel_level );
        CheckFuelAdapter CheckFuelCommand( ob );
        MoveAdapter MoveCommand( ob , xy , vel );

        CheckFuelCommand.execute();
        MoveCommand.execute();
        BurnFuelCommand.execute();
    }

};

class macroCommandRotate
{
    std::shared_ptr<UObject> ob;
    velocity vel;
    int angle;
    int fuel_level;

    public:

    macroCommandRotate( std::shared_ptr<UObject> _ob , int _angle , int _fuel_level , velocity _vel )
        : ob( _ob ) , angle( _angle ) , fuel_level( _fuel_level ) , vel( _vel )
    {

    }

    ~macroCommandRotate(){}

    void execute ( void )
    {
        BurnFuelAdapter BurnFuelCommand( ob , fuel_level );
        CheckFuelAdapter CheckFuelCommand( ob );
        VelocityAdapter VelocityCommand( ob , vel );
        RotationAdapter RotationCommand( ob , angle);


        try
        {
            CheckFuelCommand.execute();
            RotationCommand.execute();
            BurnFuelCommand.execute();
        }
        catch ( Exception ex )
        {
            throw Exception{ "CommandException" , ExceptionType::TYPE2Exception , ExceptionStaus::NOTPROCESSED };
        }
    }
};


#endif // COMMAND_H
