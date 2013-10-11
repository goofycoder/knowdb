#include <iostream>

class RemoteControl 
{
public:
    Command slot;

    RemoteControl() { }

    void setCommand(Command command) { 
        slot = command;
    } 

    void buttonWasPressed {
        slot.execute();
    }
};

class Light 
{
public:
    Light(int _id) { id=_id; }
private:
    int id;
};

class Command 
{   
    void execute() { }
};

class LightOnCommand : Command 
{
    void execute() { }
};

void TEST_RemoteControl() 
{
    RemoteControl remote;
    
    int light_id = 23;
    Light light(light_id);

    LightOnCommand lightOn(light);

    remote.setCommand(lightOn);
    
    // could be extended to add other commands
    // e.g.: remote.setCommand(lightOff);
    remote.buttonWasPressed();   
}

int main()
{
    TEST_RemoteControl();

    return 0;
}
