/* A Bison parser, made by GNU Bison 3.4.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_INCLUDE_SILC_H_INCLUDED
# define YY_YY_INCLUDE_SILC_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    _DECL = 258,
    _ENDDECL = 259,
    _INT = 260,
    _STR = 261,
    _TEXT = 262,
    _MAIN = 263,
    _RET = 264,
    _IF = 265,
    _WHILE = 266,
    _THEN = 267,
    _ELSE = 268,
    _ENDIF = 269,
    _ENDWHILE = 270,
    _DO = 271,
    _BREAK = 272,
    _CONT = 273,
    _LT = 274,
    _GT = 275,
    _EQ = 276,
    _NE = 277,
    _LE = 278,
    _GE = 279,
    _PLUS = 280,
    _MINUS = 281,
    _MUL = 282,
    _DIV = 283,
    _END = 284,
    _BEGIN = 285,
    _READ = 286,
    _WRITE = 287,
    _SEMI = 288,
    _EQUALS = 289,
    _Q = 290,
    _COMMA = 291,
    _MOD = 292,
    _ID = 293,
    _NUM = 294
  };
#endif
/* Tokens.  */
#define _DECL 258
#define _ENDDECL 259
#define _INT 260
#define _STR 261
#define _TEXT 262
#define _MAIN 263
#define _RET 264
#define _IF 265
#define _WHILE 266
#define _THEN 267
#define _ELSE 268
#define _ENDIF 269
#define _ENDWHILE 270
#define _DO 271
#define _BREAK 272
#define _CONT 273
#define _LT 274
#define _GT 275
#define _EQ 276
#define _NE 277
#define _LE 278
#define _GE 279
#define _PLUS 280
#define _MINUS 281
#define _MUL 282
#define _DIV 283
#define _END 284
#define _BEGIN 285
#define _READ 286
#define _WRITE 287
#define _SEMI 288
#define _EQUALS 289
#define _Q 290
#define _COMMA 291
#define _MOD 292
#define _ID 293
#define _NUM 294

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 29 "./src/silc.y"

	tnode *no;
    char *name;
    int type;
    LinkedList* list;
	

#line 143 "./include/silc.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_INCLUDE_SILC_H_INCLUDED  */
