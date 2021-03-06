//---------------------------------------------------------------
//   GLOBAL VARIABLES
//---------------------------------------------------------------

const int buttonsMax = 304;
const byte commandWaitFrames = 2;

/* User Mode Selection */
byte userMode = 1;          // 0 = edit, 1 = live link, 2 = play mode
int  userModeNode = 299;

/* Modifier Selection */
byte modifierMode = 7;
int  modifierModeNode = 999;

/* Com CTRL Selection and Pot Vals */
byte comCtrlMode = 4;
int  comCtrlModeNode = 999;
byte lastccVal[4]; 

int             velAdjust[4]     {127, 127, 127, 127};
unsigned int    lengthAdjust[4]  {3000, 3000, 3000, 3000};
int             pitchAdjust[4]   {  0,   0,   0,   0};


//---------------------------------------------------------------
//   NODES
//---------------------------------------------------------------

struct node {
    const byte ledx;
    const byte ledy;
    const byte type;        // 0 = not patchable, 1 = input, 2 = special input, 3 = output

    bool intState;
    bool lastIntState;
    bool extState; 
    bool lastExtState;

    bool mute;
    bool toggle;
    bool togLock;
    bool invert;


    bool intHasChanged()  {
      if (intState != lastIntState) 
        return 1;
      else 
        return 0;
    }

    bool intRisingEdge()  {
      if (intState != lastIntState) 
        if (intState == 1) 
          return 1;
        else 
          return 0;
      else
          return 0;
      // return abs(intState - lastIntState) * intState
      // return !(intState && lastIntState) && intState
    }

    bool intFallingEdge()  {
      if (intState != lastIntState) 
        if (intState == 0) 
          return 1;
        else 
          return 0;
      else
        return 0;
    }

    bool extHasChanged()  {
      if (extState != lastExtState) 
        return 1;
      else 
        return 0;
    }

    bool extRisingEdge()  {
      if (extState != lastExtState) 
        if (extState == 1) 
          return 1;
        else 
          return 0;
      else
          return 0;
    }

    bool extFallingEdge()  {
      if (extState != lastExtState) 
        if (extState == 0) 
          return 1;
        else 
          return 0;
      else
        return 0;
    }
};

node nodes[] {
    { 19, 6, 1}, { 41, 2, 1}, { 43, 3, 1}, { 40, 3, 1}, { 23, 6, 3}, { 26, 7, 3}, { 24, 7, 3}, { 41, 3, 1}, { 23, 5, 3}, { 40, 2, 1}, 
    { 42, 2, 1}, { 43, 2, 1}, { 26, 5, 3}, { 24, 5, 3}, { 24, 6, 3}, { 42, 3, 1}, { 25, 7, 3}, { 44, 0, 1}, { 42, 0, 0}, { 46, 0, 1}, 
    { 25, 6, 3}, { 22, 7, 1}, { 26, 6, 3}, { 44, 3, 1}, { 22, 6, 1}, { 40, 0, 0}, { 43, 0, 1}, { 38, 4, 0}, { 25, 5, 3}, { 20, 6, 1}, 
    { 21, 7, 1}, { 47, 0, 1}, { 23, 7, 3}, { 99, 9, 0}, { 99, 9, 0}, { 45, 0, 0}, { 20, 7, 1}, { 21, 6, 1}, { 19, 7, 1}, { 41, 0, 0}, 
    { 42, 1, 1}, { 33, 7, 1}, { 39, 1, 0}, { 38, 0, 1}, { 47, 1, 1}, { 44, 1, 1}, { 37, 5, 1}, { 39, 3, 1}, { 36, 4, 1}, { 32, 5, 1}, 
    { 38, 3, 1}, { 34, 7, 1}, { 46, 1, 1}, { 43, 1, 1}, { 45, 1, 1}, { 37, 2, 1}, { 40, 1, 1}, { 39, 2, 1}, { 33, 5, 1}, { 35, 7, 1}, 
    { 36, 5, 1}, { 35, 4, 1}, { 37, 4, 1}, { 34, 6, 1}, { 35, 5, 1}, { 32, 6, 1}, { 33, 4, 1}, { 34, 5, 1}, { 35, 6, 1}, { 38, 6, 1}, 
    { 36, 6, 1}, { 34, 4, 1}, { 36, 7, 1}, { 39, 0, 0}, { 32, 7, 1}, { 32, 4, 1}, { 41, 1, 1}, { 37, 7, 1}, { 38, 7, 1}, { 38, 1, 1}, 
    {  5, 6, 1}, { 36, 3, 1}, { 33, 2, 1}, { 36, 2, 1}, {  4, 7, 3}, { 32, 3, 1}, { 33, 3, 1}, { 35, 3, 1}, {  4, 5, 3}, { 37, 1, 1}, 
    { 35, 2, 1}, { 34, 3, 1}, {  6, 6, 1}, {  0, 7, 3}, {  7, 7, 3}, { 32, 2, 1}, {  5, 5, 3}, { 32, 0, 0}, { 35, 1, 0}, { 32, 1, 0}, 
    {  2, 7, 3}, {  6, 5, 3}, {  7, 5, 3}, { 33, 0, 0}, {  3, 7, 3}, { 37, 3, 1}, { 36, 1, 1}, { 33, 1, 0}, {  4, 6, 1}, {  7, 6, 1}, 
    {  0, 6, 2}, { 34, 1, 0}, {  1, 7, 3}, { 36, 0, 1}, { 34, 0, 0}, { 37, 0, 1}, {  1, 6, 2}, {  5, 7, 3}, {  6, 7, 3}, { 35, 0, 0}, 
    {  1, 3, 2}, {  3, 2, 3}, {  2, 2, 3}, {  1, 1, 3}, {  1, 5, 3}, {  0, 2, 3}, {  1, 2, 3}, {  0, 1, 3}, {  3, 4, 3}, {  6, 2, 3}, 
    {  5, 2, 3}, {  4, 1, 3}, {  4, 4, 3}, {  4, 3, 1}, {  0, 3, 2}, {  6, 0, 1}, {  5, 3, 1}, {  2, 1, 3}, {  0, 0, 2}, {  6, 1, 3}, 
    {  6, 3, 1}, {  7, 4, 3}, {  6, 4, 3}, {  3, 1, 3}, {  2, 5, 3}, {  7, 0, 1}, {  5, 0, 1}, {  1, 0, 2}, {  3, 5, 3}, {  1, 4, 3}, 
    {  2, 4, 3}, {  5, 1, 3}, {  7, 3, 1}, {  7, 1, 3}, {  7, 2, 3}, {  4, 2, 3}, {  0, 4, 3}, {  0, 5, 3}, {  5, 4, 3}, {  4, 0, 1}, 
    { 15, 5, 1}, { 18, 1, 3}, { 13, 5, 3}, {  8, 7, 1}, {  8, 4, 1}, {  9, 7, 1}, {  8, 6, 1}, {  9, 6, 1}, { 14, 4, 3}, { 17, 5, 0}, 
    { 18, 7, 3}, { 18, 2, 3}, { 12, 4, 3}, { 12, 5, 3}, {  9, 4, 1}, {  8, 5, 1}, { 14, 5, 3}, { 18, 4, 3}, { 17, 4, 0}, { 18, 3, 3}, 
    { 15, 7, 1}, { 13, 4, 3}, { 15, 4, 1}, { 17, 6, 0}, { 12, 7, 3}, { 16, 4, 1}, { 18, 6, 3}, { 18, 5, 3}, { 13, 6, 3}, { 12, 6, 3}, 
    {  9, 5, 1}, { 16, 5, 1}, { 14, 7, 3}, { 16, 6, 1}, { 18, 0, 3}, { 16, 7, 1}, { 14, 6, 3}, { 13, 7, 3}, { 15, 6, 1}, { 17, 7, 0}, 
    { 14, 1, 1}, {  9, 1, 1}, { 16, 1, 3}, {  9, 0, 1}, { 15, 3, 1}, { 12, 3, 1}, { 17, 3, 1}, { 10, 3, 1}, { 16, 2, 1}, {  9, 2, 1}, 
    {  8, 0, 0}, { 10, 0, 1}, { 16, 3, 1}, { 11, 2, 1}, { 16, 0, 3}, {  8, 1, 0}, { 14, 2, 1}, { 17, 1, 3}, { 11, 0, 1}, { 15, 0, 3}, 
    { 13, 3, 1}, { 13, 2, 1}, { 13, 1, 1}, { 15, 1, 3}, { 14, 0, 1}, { 13, 0, 1}, { 10, 1, 1}, {  9, 3, 1}, { 11, 3, 1}, { 17, 2, 1}, 
    { 14, 3, 1}, {  8, 3, 0}, { 15, 2, 1}, {  8, 2, 0}, { 11, 1, 1}, { 12, 0, 1}, { 12, 2, 1}, { 12, 1, 1}, { 17, 0, 3}, { 10, 2, 1}, 
    { 22, 0, 3}, { 25, 3, 1}, { 25, 4, 3}, { 24, 1, 3}, { 22, 2, 3}, { 21, 2, 3}, { 22, 4, 1}, { 26, 3, 1}, { 22, 1, 3}, { 23, 4, 3}, 
    { 23, 3, 1}, { 24, 4, 3}, { 21, 4, 1}, { 21, 0, 1}, { 19, 1, 1}, { 24, 3, 1}, { 21, 1, 3}, { 23, 2, 3}, { 26, 0, 3}, { 26, 2, 3}, 
    { 20, 2, 0}, { 19, 4, 1}, { 19, 2, 0}, { 25, 0, 3}, { 19, 3, 1}, { 25, 2, 3}, { 25, 1, 1}, { 26, 1, 3}, { 20, 4, 1}, { 20, 3, 1}, 
    { 22, 3, 1}, { 24, 0, 1}, { 20, 0, 3}, { 23, 1, 1}, { 26, 4, 3}, { 23, 0, 3}, { 19, 0, 1}, { 20, 1, 3}, { 21, 3, 1}, { 24, 2, 3}, 
    { 28, 5, 0}, { 28, 3, 0}, { 28, 4, 0}, { 27, 4, 0}, { 27, 5, 0}, { 27, 7, 0}, { 27, 6, 0}, { 30, 2, 0}, { 30, 5, 0}, { 29, 4, 0}, 
    { 27, 3, 0}, { 29, 3, 0}, { 29, 5, 0}, { 29, 6, 0}, { 28, 6, 0}, { 99, 9, 0}, { 30, 7, 0}, { 99, 9, 0}, { 28, 2, 0}, { 29, 2, 0}, 
    { 30, 6, 0}, { 28, 7, 0}, { 29, 7, 0}, { 27, 2, 0}, { 2, 0, 0} 
};


//---------------------------------------------------------------
//   FUNCTIONS
//---------------------------------------------------------------

void nodesUpdateLastStates() {
    for (int i = 0; i < 304; i++) {
        nodes[i].lastIntState = nodes[i].intState;
        nodes[i].lastExtState = nodes[i].extState;
    }
}