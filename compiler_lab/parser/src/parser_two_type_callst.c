#include <stdlib.h>

#include "error.h"
#include "parser.h"
#include "reader.h"
#include "scanner.h"

Token *currentToken;
Token *lookAhead;

void scan(void) {
  Token *tmp = currentToken;
  currentToken = lookAhead;
  lookAhead = getValidToken();
  free(tmp);
}

void eat(TokenType tokenType) {
  if (lookAhead->tokenType == tokenType) {
    printToken(lookAhead);
    scan();
  } else
    missingToken(tokenType, lookAhead->lineNo, lookAhead->colNo);
}

void compileProgram(void) {
  assert("Parsing a Program ....");
  eat(KW_PROGRAM);
  eat(TK_IDENT);
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_PERIOD);
  assert("Program parsed!");
}

void compileBlock(void) {
  assert("Parsing a Block ....");
  if (lookAhead->tokenType == KW_CONST) {
    eat(KW_CONST);
    compileConstDecl();
    compileConstDecls();
    compileBlock2();
  } else
    compileBlock2();
  assert("Block parsed!");
}

void compileBlock2(void) {
  if (lookAhead->tokenType == KW_TYPE) {
    eat(KW_TYPE);
    compileTypeDecl();
    compileTypeDecls();
    compileBlock3();
  } else
    compileBlock3();
}

void compileBlock3(void) {
  if (lookAhead->tokenType == KW_VAR) {
    eat(KW_VAR);
    compileVarDecl();
    compileVarDecls();
    compileBlock4();
  } else
    compileBlock4();
}

void compileBlock4(void) {
  compileSubDecls();
  compileBlock5();
}

void compileBlock5(void) {
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
}

void compileConstDecls(void) {
  while (lookAhead->tokenType == TK_IDENT)
    compileConstDecl();
}

void compileConstDecl(void) {
  eat(TK_IDENT);
  eat(SB_EQ);
  compileConstant();
  eat(SB_SEMICOLON);
}

void compileTypeDecls(void) {
  if (lookAhead->tokenType == TK_IDENT) {
    compileTypeDecl();
    compileTypeDecls();
  }
}

void compileTypeDecl(void) {
  eat(TK_IDENT);
  eat(SB_EQ);
  compileType();
  eat(SB_SEMICOLON);
}

void compileVarDecls(void) {
  if (lookAhead->tokenType == TK_IDENT) {
    compileVarDecl();
    compileVarDecls();
  }
}

void compileVarDecl(void) {
  eat(TK_IDENT);
  eat(SB_COLON);
  compileType();
  eat(SB_SEMICOLON);
}

void compileSubDecls(void) {
  assert("Parsing subtoutines ....");
  while (lookAhead->tokenType == KW_FUNCTION ||
         lookAhead->tokenType == KW_PROCEDURE) {
    if (lookAhead->tokenType == KW_FUNCTION) {
      compileFuncDecl();
    }
    if (lookAhead->tokenType == KW_PROCEDURE) {
      compileProcDecl();
    }
  }
  assert("Subtoutines parsed ....");
}

void compileFuncDecl(void) {
  assert("Parsing a function ....");
  eat(KW_FUNCTION);
  eat(TK_IDENT);
  compileParams();
  eat(SB_COLON);
  compileBasicType();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Function parsed ....");
}

void compileProcDecl(void) {
  assert("Parsing a procedure ....");
  eat(KW_PROCEDURE);
  eat(TK_IDENT);
  compileParams();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Procedure parsed ....");
}

void compileUnsignedConstant(void) {
  switch (lookAhead->tokenType) {
    case TK_NUMBER:
    case TK_IDENT:
    case TK_CHAR:
      eat(lookAhead->tokenType);
      break;
    default:
      error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileConstant(void) {
  switch (lookAhead->tokenType) {
    case SB_MINUS:
    case SB_PLUS:
      eat(lookAhead->tokenType);
      compileConstant2();
      break;
    case TK_CHAR:
      eat(TK_CHAR);
      break;
    default:
      compileConstant2();
      break;
  }
}

void compileConstant2(void) {
  switch (lookAhead->tokenType) {
    case TK_IDENT:
      eat(TK_IDENT);
      break;
    case TK_NUMBER:
      eat(TK_NUMBER);
      break;
    default:
      error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
  }
}

void compileType(void) {
  switch (lookAhead->tokenType) {
    case KW_INTEGER:
    case KW_CHAR:
    case TK_IDENT:
      eat(lookAhead->tokenType);
      break;
    case KW_ARRAY:
      eat(KW_ARRAY);
      eat(SB_LSEL);
      eat(TK_NUMBER);
      eat(SB_RSEL);
      eat(KW_OF);
      compileType();
      break;
    default:
      error(ERR_INVALIDTYPE, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileBasicType(void) {
  switch (lookAhead->tokenType) {
    case KW_INTEGER:
    case KW_CHAR:
      eat(lookAhead->tokenType);
      break;
    default:
      error(ERR_INVALIDBASICTYPE, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileParams(void) {
  if (lookAhead->tokenType == SB_LPAR) {
    eat(SB_LPAR);
    compileParam();
    compileParams2();
    eat(SB_RPAR);
  }
}

void compileParams2(void) {
  if (lookAhead->tokenType == SB_SEMICOLON) {
    eat(SB_SEMICOLON);
    compileParam();
    compileParams2();
  }
}

void compileParam(void) {
  switch (lookAhead->tokenType) {
    case TK_IDENT:
      eat(TK_IDENT);
      eat(SB_COLON);
      compileBasicType();
      break;
    case KW_VAR:
      eat(KW_VAR);
      eat(TK_IDENT);
      eat(SB_COLON);
      compileBasicType();
      break;
    default:
      error(ERR_INVALIDPARAM, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileStatements(void) {
  compileStatement();
  compileStatements2();
}

void compileStatements2(void) {
  switch (lookAhead->tokenType) {
    case SB_SEMICOLON:
      eat(SB_SEMICOLON);
      compileStatement();
      compileStatements2();
      break;
    case KW_END:
      break;
    default:
      error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileStatement(void) {
  switch (lookAhead->tokenType) {
    case TK_IDENT:
      compileAssignSt();
      break;
    case KW_CALL:
      compileCallSt();
      break;
    case KW_BEGIN:
      compileGroupSt();
      break;
    case KW_IF:
      compileIfSt();
      break;
    case KW_WHILE:
      compileWhileSt();
      break;
    case KW_FOR:
      compileForSt();
      break;
    case SB_SEMICOLON:
    case KW_END:
    case KW_ELSE:
      break;
    default:
      error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}
/*-----------------------------------------------------*/

void compileAssignSt(void) {
  assert("Parsing an assign statement ....");
  eat(TK_IDENT);

  switch (lookAhead->tokenType) {
    case SB_ASSIGN:
      eat(SB_ASSIGN);
      compileExpression();
      break;
    case SB_LSEL:
      compileIndexes();
      break;
    default:
      compileArguments();
      break;
  }
  assert("Assign statement parsed ....");
}

void compileCallSt(void) {
  assert("Parsing a call statement ....");
  eat(KW_CALL);
  eat(TK_IDENT);
  compileArguments3();
  assert("Call statement parsed ....");
}

void compileArguments(void) {
  compileExpression();
  compileArguments2();
}

void compileArguments2(void) {
  if (lookAhead->tokenType == SB_COMMA) {
    eat(SB_COMMA);
    compileExpression();
    compileArguments2();
  }
}

void compileArguments3(void) {
  eat(SB_LPAR);
  compileArguments4();
  eat(SB_RPAR);
}

void compileArguments4(void) {
  compileExpression();
  compileArguments5();
}

void compileArguments5(void) {
  if (lookAhead->tokenType == SB_COMMA) {
    eat(SB_COMMA);
    compileExpression();
    compileArguments5();
  }
}

/*-----------------------------------------------------*/

void compileGroupSt(void) {
  assert("Parsing a group statement ....");
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
  assert("Group statement parsed ....");
}

void compileIfSt(void) {
  assert("Parsing an if statement ....");
  eat(KW_IF);
  compileCondition();
  eat(KW_THEN);
  compileStatement();
  if (lookAhead->tokenType == KW_ELSE)
    compileElseSt();
  assert("If statement parsed ....");
}

void compileElseSt(void) {
  eat(KW_ELSE);
  compileStatement();
}

void compileWhileSt(void) {
  assert("Parsing a while statement ....");
  eat(KW_WHILE);
  compileCondition();
  eat(KW_DO);
  compileStatement();
  assert("While statement pased ....");
}

void compileForSt(void) {
  assert("Parsing a for statement ....");
  eat(KW_FOR);
  eat(TK_IDENT);
  eat(SB_ASSIGN);
  compileExpression();
  eat(KW_TO);
  compileExpression();
  eat(KW_DO);
  compileStatement();
  assert("For statement parsed ....");
}

// void compileArguments(void) {
//   switch (lookAhead->tokenType) {
//     case SB_LPAR:
//       eat(SB_LPAR);
//       compileExpression();
//       compileArguments2();
//       eat(SB_RPAR);
//       break;
//     case SB_TIMES:
//     case SB_SLASH:
//     case SB_PLUS:
//     case SB_MINUS:
//     case KW_TO:
//     case KW_DO:
//     case SB_RPAR:
//     case SB_COMMA:
//     case SB_EQ:
//     case SB_NEQ:
//     case SB_LE:
//     case SB_LT:
//     case SB_GE:
//     case SB_GT:
//     case SB_RSEL:
//     case SB_SEMICOLON:
//     case KW_END:
//     case KW_ELSE:
//     case KW_THEN:
//       break;
//     default:
//       error(ERR_INVALIDARGUMENTS, lookAhead->lineNo, lookAhead->colNo);
//   }
// }

// void compileArguments2(void) {
//   if (lookAhead->tokenType == SB_COMMA) {
//     eat(SB_COMMA);
//     compileExpression();
//     compileArguments2();
//   }
// }

void compileCondition(void) {
  compileExpression();
  compileCondition2();
}

void compileCondition2(void) {
  switch (lookAhead->tokenType) {
    case SB_EQ:
    case SB_NEQ:
    case SB_LT:
    case SB_LE:
    case SB_GT:
    case SB_GE:
      eat(lookAhead->tokenType);
      compileExpression();
      break;
    default:
      error(ERR_INVALIDCOMPARATOR, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileExpression(void) {
  assert("Parsing an expression");
  switch (lookAhead->tokenType) {
    case SB_PLUS:
    case SB_MINUS:
      eat(lookAhead->tokenType);
      compileExpression2();
      break;
    default:
      compileExpression2();
      break;
  }
  assert("Expression parsed");
}

void compileExpression2(void) {
  compileTerm();
  compileExpression3();
}

void compileExpression3(void) {
  switch (lookAhead->tokenType) {
    case SB_PLUS:
      eat(SB_PLUS);
      compileTerm();
      compileExpression3();
      break;
    case SB_MINUS:
      eat(SB_MINUS);
      compileTerm();
      compileExpression3();
      break;
    case KW_TO:
    case KW_DO:
    case SB_RPAR:
    case SB_COMMA:
    case SB_EQ:
    case SB_NEQ:
    case SB_LE:
    case SB_LT:
    case SB_GE:
    case SB_GT:
    case SB_RSEL:
    case SB_SEMICOLON:
    case KW_END:
    case KW_ELSE:
    case KW_THEN:
      break;
    default:
      error(ERR_INVALIDEXPRESSION, lookAhead->lineNo, lookAhead->colNo);
  }
}

void compileTerm(void) {
  compileFactor();
  compileTerm2();
}

void compileTerm2(void) {
  switch (lookAhead->tokenType) {
    case SB_TIMES:
      eat(SB_TIMES);
      compileFactor();
      compileTerm2();
      break;
    case SB_SLASH:
      eat(SB_SLASH);
      compileFactor();
      compileTerm2();
      break;
    case SB_PLUS:
    case SB_MINUS:
    case KW_TO:
    case KW_DO:
    case SB_RPAR:
    case SB_COMMA:
    case SB_EQ:
    case SB_NEQ:
    case SB_LE:
    case SB_LT:
    case SB_GE:
    case SB_GT:
    case SB_RSEL:
    case SB_SEMICOLON:
    case KW_END:
    case KW_ELSE:
    case KW_THEN:
    case SB_ASSIGN:
      break;
    default:
      error(ERR_INVALIDTERM, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileFactor(void) {
  switch (lookAhead->tokenType) {
    case TK_NUMBER:
    case TK_CHAR:
      compileUnsignedConstant();
      break;
    case SB_LPAR:
      eat(SB_LPAR);
      compileExpression();
      eat(SB_RPAR);
      break;
    case TK_IDENT:
      eat(TK_IDENT);
      switch (lookAhead->tokenType) {
        case SB_LSEL:
          compileIndexes();
          break;
        case SB_LPAR:
          compileArguments3();
          break;
        default:
          break;
      }
      break;
    default:
      error(ERR_INVALIDFACTOR, lookAhead->lineNo, lookAhead->colNo);
      break;
  }
}

void compileIndexes(void) {
  if (lookAhead->tokenType == SB_LSEL) {
    eat(SB_LSEL);
    compileExpression();
    eat(SB_RSEL);
    compileIndexes();
  }
}

int compile(char *fileName) {
  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  currentToken = NULL;
  lookAhead = getValidToken();

  compileProgram();

  free(currentToken);
  free(lookAhead);
  closeInputStream();
  return IO_SUCCESS;
}
