/* A Bison parser, made by GNU Bison 3.5.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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
    _TYPE = 265,
    _ENDTYPE = 266,
    _NULL = 267,
    _EXTENDS = 268,
    _IF = 269,
    _WHILE = 270,
    _THEN = 271,
    _ELSE = 272,
    _ENDIF = 273,
    _ENDWHILE = 274,
    _DO = 275,
    _BREAK = 276,
    _CONT = 277,
    _AND = 278,
    _INIT = 279,
    _ALLOC = 280,
    _FREE = 281,
    _CLASS = 282,
    _SELF = 283,
    _ENDCLASS = 284,
    _NEW = 285,
    _LT = 286,
    _GT = 287,
    _EQ = 288,
    _NE = 289,
    _LE = 290,
    _GE = 291,
    _PLUS = 292,
    _MINUS = 293,
    _MUL = 294,
    _DIV = 295,
    _END = 296,
    _BEGIN = 297,
    _READ = 298,
    _WRITE = 299,
    _SEMI = 300,
    _EQUALS = 301,
    _Q = 302,
    _COMMA = 303,
    _MOD = 304,
    _ID = 305,
    _NUM = 306
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
#define _TYPE 265
#define _ENDTYPE 266
#define _NULL 267
#define _EXTENDS 268
#define _IF 269
#define _WHILE 270
#define _THEN 271
#define _ELSE 272
#define _ENDIF 273
#define _ENDWHILE 274
#define _DO 275
#define _BREAK 276
#define _CONT 277
#define _AND 278
#define _INIT 279
#define _ALLOC 280
#define _FREE 281
#define _CLASS 282
#define _SELF 283
#define _ENDCLASS 284
#define _NEW 285
#define _LT 286
#define _GT 287
#define _EQ 288
#define _NE 289
#define _LE 290
#define _GE 291
#define _PLUS 292
#define _MINUS 293
#define _MUL 294
#define _DIV 295
#define _END 296
#define _BEGIN 297
#define _READ 298
#define _WRITE 299
#define _SEMI 300
#define _EQUALS 301
#define _Q 302
#define _COMMA 303
#define _MOD 304
#define _ID 305
#define _NUM 306

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 143 "./src/silc.y"

	tnode *no;
    char *name;
    char* type;
    LinkedList* list;
    Field* field;
	

#line 168 "./include/silc.h"

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
