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
    _AND = 274,
    _LT = 275,
    _GT = 276,
    _EQ = 277,
    _NE = 278,
    _LE = 279,
    _GE = 280,
    _PLUS = 281,
    _MINUS = 282,
    _MUL = 283,
    _DIV = 284,
    _END = 285,
    _BEGIN = 286,
    _READ = 287,
    _WRITE = 288,
    _SEMI = 289,
    _EQUALS = 290,
    _Q = 291,
    _COMMA = 292,
    _MOD = 293,
    _ID = 294,
    _NUM = 295
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
#define _AND 274
#define _LT 275
#define _GT 276
#define _EQ 277
#define _NE 278
#define _LE 279
#define _GE 280
#define _PLUS 281
#define _MINUS 282
#define _MUL 283
#define _DIV 284
#define _END 285
#define _BEGIN 286
#define _READ 287
#define _WRITE 288
#define _SEMI 289
#define _EQUALS 290
#define _Q 291
#define _COMMA 292
#define _MOD 293
#define _ID 294
#define _NUM 295

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 29 "./src/silc.y"

	tnode *no;
    char *name;
    int type;
    LinkedList* list;
	

#line 145 "./include/silc.h"

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
