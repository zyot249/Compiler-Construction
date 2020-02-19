/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __ERROR_H__
#define __ERROR_H__

typedef enum {
  ERR_ENDOFCOMMENT,
  ERR_IDENTTOOLONG,
  ERR_STRINGTOOLONG,
  ERR_INVALIDCHARCONSTANT,
  ERR_INVALIDSTRINGCONSTANT,
  ERR_INVALIDSYMBOL

} ErrorCode;


#define ERM_ENDOFCOMMENT "End of comment expected!"
#define ERM_IDENTTOOLONG "Identification too long!"
#define ERM_STRINGTOOLONG "String too long! (Max of length of String is 16)"
#define ERM_INVALIDCHARCONSTANT "Invalid const char!"
#define ERM_INVALIDSTRINGCONSTANT "Invalid const string!"
#define ERM_INVALIDSYMBOL "Invalid symbol!"

void error(ErrorCode err, int lineNo, int colNo);

#endif
