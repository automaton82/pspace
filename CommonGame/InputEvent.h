#ifndef _INPUTEVENT_H_
#define _INPUTEVENT_H_


enum InputEventType
{
	// INPUT_Unknown = 0,
	INPUT_UNKNOWN = 0,
	INPUT_EVENT_UNKNOWN = 0,
							//ascii set unaffected

	INPUT_LAST_ASCII = 256,

	//KEY_UNKNOWN,		//keyboard events
	//KeyBegin,

	KEY_ENTER,

	KEY_UP, 
	KEY_DOWN, 
	KEY_LEFT, 
	KEY_RIGHT,

	//KEY_CONTROL,
	KEY_LCONTROL,			//toggles
	KEY_RCONTROL,
	KEY_LSHIFT,
	KEY_RSHIFT,

	KEY_TAB,

	KEY_INSERT,				//insert pad
	KEY_DELETE,				
	KEY_HOME,
	KEY_END,
	KEY_PAGEUP,
	KEY_PAGEDOWN,

	KEY_ESCAPE,
							//function keys
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,


	//MOUSE_UNKNOWN = 2000,			//mouse events
	MouseBegin = 300,

	MOUSE_BUTTON_LEFT = 300,
	MOUSE_BUTTON_RIGHT,
	MOUSE_BUTTON_MIDDLE,
	MOUSE_BUTTON_WHEEL,
	//MOUSE_NO_BUTTON
	MOUSE_PASSIVE,

	INPUT_NumInputs
};

enum InputEventState
{
	INPUT_STATE_UNKNOWN = 0,

	INPUT_STATE_UP,
	INPUT_STATE_DOWN
};

struct InputEvent
{
public:
	InputEvent();
	//InputEvent(const string& s);
	InputEvent(InputEventType type, InputEventState state, InputEventState previousState);

	InputEventType type;		//should be set through constructor, to update inputStates
	InputEventState state;
	InputEventState previousState;

	//static bool isAscii(InputEventType event) const;
	//static char getAscii(InputEventType event) const;

	bool isAscii() const;
	char getAscii() const;

	bool isActivated() const;	//just pressed
	bool isActive() const;		//still being pressed

	bool isUnactivated() const;	 //just released

	static bool inputStates[INPUT_NumInputs];

	
private:
	
	/*static void initStringInputs();
	static InputEventType getInputType(const string& s);
	
	typedef map<string, InputEventType> StringInputMap;
	static StringInputMap stringInputs_;
	static bool hasStringInputs_;*/
};

#endif