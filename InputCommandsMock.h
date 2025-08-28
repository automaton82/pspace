#ifndef _INPUT_COMMANDS_MOCK_H_
#define _INPUT_COMMANDS_MOCK_H_

#include "InputEvent.h"

namespace InputCommands {
    
    struct InputData {
        InputEventType type;
        bool shift;
        bool ctrl; 
        bool alt;
        
        InputData(InputEventType t, bool s = false, bool c = false, bool a = false) 
            : type(t), shift(s), ctrl(c), alt(a) {}
    };
    
    class InputCommand {
    public:
        virtual ~InputCommand() = default;
        virtual void execute() = 0;
        virtual const char* getTypeName() const = 0;
        
        InputData input;
        
    protected:
        InputCommand(const InputData& data) : input(data) {}
    };
    
    class InputActive : public InputCommand {
    public:
        InputActive(const InputData& data) : InputCommand(data) {}
        void execute() override;
        const char* getTypeName() const override { return "InputActive"; }
    };
    
    class InputActivated : public InputCommand {
    public:
        InputActivated(const InputData& data) : InputCommand(data) {}
        void execute() override;
        const char* getTypeName() const override { return "InputActivated"; }
    };
    
    class InputUnactivated : public InputCommand {
    public:
        InputUnactivated(const InputData& data) : InputCommand(data) {}
        void execute() override;
        const char* getTypeName() const override { return "InputUnactivated"; }
    };
}

#endif // _INPUT_COMMANDS_MOCK_H_
