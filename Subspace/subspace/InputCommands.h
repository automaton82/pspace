#ifndef _INPUTCOMMANDS_H_
#define _INPUTCOMMANDS_H_

#include "InputEvent.h"

class InputCommandReceiver;

class InputCommand
{
public:
	virtual ~InputCommand();

	virtual void execute(InputCommandReceiver* listener) = 0;
};

namespace InputCommands
{
	struct InputData
	{
		/*InputData(const InputData& copy)
		{
			*this = copy;
		}*/

		InputData(InputEventType t, bool s = false, bool c = false, bool a = false) :
			type(t), shift(s), ctrl(c), alt(a)
		{}

		InputData() :
			type(INPUT_UNKNOWN), shift(false), ctrl(false), alt(false)
		{}

		friend bool operator<(const InputData& lhs, const InputData& rhs)
		{
			if(lhs.type != rhs.type)
				return lhs.type < rhs.type;
			else if(lhs.shift != rhs.shift)
				return lhs.shift < rhs.shift;
			else if(lhs.ctrl != rhs.ctrl)
				return lhs.ctrl < rhs.ctrl;
			else if(lhs.alt != rhs.alt)
				return lhs.alt < rhs.alt;
			else
				return false;
		}

		friend bool operator==(const InputData& lhs, const InputData& rhs)
		{
			return 
				(lhs.type == rhs.type) && 
				(lhs.shift == rhs.shift) && 
				(lhs.ctrl == rhs.ctrl) && 
				(lhs.alt == rhs.alt);
		}

		InputEventType type;
		bool shift;
		bool ctrl;
		bool alt;
	};

	class InputActive : public InputCommand
	{
	public:
		void execute(InputCommandReceiver* listener);

		InputData input;
	};

	class InputActivated : public InputCommand
	{
	public:
		void execute(InputCommandReceiver* listener);

		InputData input;
	};

	class InputUnactivated : public InputCommand
	{
	public:
		void execute(InputCommandReceiver* listener);

		InputData input;
	};
}

#endif