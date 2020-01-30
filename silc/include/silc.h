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
    _IF = 262,
    _WHILE = 263,
    _THEN = 264,
    _ELSE = 265,
    _ENDIF = 266,
    _ENDWHILE = 267,
    _DO = 268,
    _BREAK = 269,
    _CONT = 270,
    _LT = 271,
    _GT = 272,
    _EQ = 273,
    _NE = 274,
    _LE = 275,
    _GE = 276,
    _PLUS = 277,
    _MINUS = 278,
    _MUL = 279,
    _DIV = 280,
    _END = 281,
    _BEGIN = 282,
    _READ = 283,
    _WRITE = 284,
    _SEMI = 285,
    _EQUALS = 286,
    _Q = 287,
    _COMMA = 288,
    _MOD = 289,
    _ID = 290,
    _NUM = 291
  };
#endif
/* Tokens.  */
#define _DECL 258
#define _ENDDECL 259
#define _INT 260
#define _STR 261
#define _IF 262
#define _WHILE 263
#define _THEN 264
#define _ELSE 265
#define _ENDIF 266
#define _ENDWHILE 267
#define _DO 268
#define _BREAK 269
#define _CONT 270
#define _LT 271
#define _GT 272
#define _EQ 273
#define _NE 274
#define _LE 275
#define _GE 276
#define _PLUS 277
#define _MINUS 278
#define _MUL 279
#define _DIV 280
#define _END 281
#define _BEGIN 282
#define _READ 283
#define _WRITE 284
#define _SEMI 285
#define _EQUALS 286
#define _Q 287
#define _COMMA 288
#define _MOD 289
#define _ID 290
#define _NUM 291

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 11 "./src/silc.y"

	struct tnode *no;
    char *name;
    int type;
    struct varList *var;
	

#line 137 "./include/silc.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_INCLUDE_SILC_H_INCLUDED  */
