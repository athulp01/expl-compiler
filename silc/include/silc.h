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
    _IF = 263,
    _WHILE = 264,
    _THEN = 265,
    _ELSE = 266,
    _ENDIF = 267,
    _ENDWHILE = 268,
    _DO = 269,
    _BREAK = 270,
    _CONT = 271,
    _LT = 272,
    _GT = 273,
    _EQ = 274,
    _NE = 275,
    _LE = 276,
    _GE = 277,
    _PLUS = 278,
    _MINUS = 279,
    _MUL = 280,
    _DIV = 281,
    _END = 282,
    _BEGIN = 283,
    _READ = 284,
    _WRITE = 285,
    _SEMI = 286,
    _EQUALS = 287,
    _Q = 288,
    _COMMA = 289,
    _MOD = 290,
    _ID = 291,
    _NUM = 292
  };
#endif
/* Tokens.  */
#define _DECL 258
#define _ENDDECL 259
#define _INT 260
#define _STR 261
#define _TEXT 262
#define _IF 263
#define _WHILE 264
#define _THEN 265
#define _ELSE 266
#define _ENDIF 267
#define _ENDWHILE 268
#define _DO 269
#define _BREAK 270
#define _CONT 271
#define _LT 272
#define _GT 273
#define _EQ 274
#define _NE 275
#define _LE 276
#define _GE 277
#define _PLUS 278
#define _MINUS 279
#define _MUL 280
#define _DIV 281
#define _END 282
#define _BEGIN 283
#define _READ 284
#define _WRITE 285
#define _SEMI 286
#define _EQUALS 287
#define _Q 288
#define _COMMA 289
#define _MOD 290
#define _ID 291
#define _NUM 292

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 12 "./src/silc.y"

	struct tnode *no;
    char *name;
    int type;
    struct varList *var;
	

#line 139 "./include/silc.h"

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
