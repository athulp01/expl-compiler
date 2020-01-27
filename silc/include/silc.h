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
    _IF = 258,
    _WHILE = 259,
    _THEN = 260,
    _ELSE = 261,
    _ENDIF = 262,
    _ENDWHILE = 263,
    _DO = 264,
    _BREAK = 265,
    _CONT = 266,
    _LT = 267,
    _GT = 268,
    _EQ = 269,
    _NE = 270,
    _LE = 271,
    _GE = 272,
    _PLUS = 273,
    _MINUS = 274,
    _MUL = 275,
    _DIV = 276,
    _END = 277,
    _BEGIN = 278,
    _READ = 279,
    _WRITE = 280,
    _SEMI = 281,
    _EQUALS = 282,
    _Q = 283,
    _ID = 284,
    _NUM = 285
  };
#endif
/* Tokens.  */
#define _IF 258
#define _WHILE 259
#define _THEN 260
#define _ELSE 261
#define _ENDIF 262
#define _ENDWHILE 263
#define _DO 264
#define _BREAK 265
#define _CONT 266
#define _LT 267
#define _GT 268
#define _EQ 269
#define _NE 270
#define _LE 271
#define _GE 272
#define _PLUS 273
#define _MINUS 274
#define _MUL 275
#define _DIV 276
#define _END 277
#define _BEGIN 278
#define _READ 279
#define _WRITE 280
#define _SEMI 281
#define _EQUALS 282
#define _Q 283
#define _ID 284
#define _NUM 285

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 10 "./src/silc.y"

	struct tnode *no;
	

#line 122 "./include/silc.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_INCLUDE_SILC_H_INCLUDED  */
