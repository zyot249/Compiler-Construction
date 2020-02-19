#ifndef __ERROR_H__
#define __ERROR_H__

typedef enum {
  ERR_ENDOFCOMMENT,
  ERR_IDENTTOOLONG,
  ERR_NUMBERTOOLARGE,
  ERR_NUMBERTOOLONG,
  ERR_INVALIDCHARCONSTANT,
  ERR_INVALIDSYMBOL,
} ErrorCode;

#define ERM_ENDOFCOMMENT "End of comment expected!"
#define ERM_IDENTTOOLONG "Identification too long!"
#define ERM_NUMBERTOOLONG "Number too long!"
#define ERM_NUMBERTOOLARGE "Number too large!"
#define ERM_INVALIDCHARCONSTANT "Invalid const char!"
#define ERM_INVALIDSYMBOL "Invalid symbol!"

void error(ErrorCode err, int lineNo, int colNo);

#endif
