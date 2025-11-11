#ifndef IR_H
#define IR_H

/*
 * @function initPinIR()
 * init pin puts the pin into input mode
 * for reading IR sesnor.
 */
void initPinIR(int PIN);
/*
 * @function readPin
 * reads given pin
 * @brief if 1 on surface,
 * otherwise will be 0 hangning on edge or if
 */
int readPinIR(int PIN);

/*
 * @function readIRObj
 * reads if an object is in front of robot
 * @brief -1 if object present otherwise 0 if no object present.
 * tested to be able to find object within x cm.
 */
int readIRObj(int PIN);

#endif
