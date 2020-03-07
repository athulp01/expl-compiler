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
    _TYPE = 265,
    _ENDTYPE = 266,
    _NULL = 267,
    _IF = 268,
    _WHILE = 269,
    _THEN = 270,
    _ELSE = 271,
    _ENDIF = 272,
    _ENDWHILE = 273,
    _DO = 274,
    _BREAK = 275,
    _CONT = 276,
    _AND = 277,
    _INIT = 278,
    _ALLOC = 279,
    _FREE = 280,
    _LT = 281,
    _GT = 282,
    _EQ = 283,
    _NE = 284,
    _LE = 285,
    _GE = 286,
    _PLUS = 287,
    _MINUS = 288,
    _MUL = 289,
    _DIV = 290,
    _END = 291,
    _BEGIN = 292,
    _READ = 293,
    _WRITE = 294,
    _SEMI = 295,
    _EQUALS = 296,
    _Q = 297,
    _COMMA = 298,
    _MOD = 299,
    _ID = 300,
    _NUM = 301
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
#define _IF 268
#define _WHILE 269
#define _THEN 270
#define _ELSE 271
#define _ENDIF 272
#define _ENDWHILE 273
#define _DO 274
#define _BREAK 275
#define _CONT 276
#define _AND 277
#define _INIT 278
#define _ALLOC 279
#define _FREE 280
#define _LT 281
#define _GT 282
#define _EQ 283
#define _NE 284
#define _LE 285
#define _GE 286
#define _PLUS 287
#define _MINUS 288
#define _MUL 289
#define _DIV 290
#define _END 291
#define _BEGIN 292
#define _READ 293
#define _WRITE 294
#define _SEMI 295
#define _EQUALS 296
#define _Q 297
#define _COMMA 298
#define _MOD 299
#define _ID 300
#define _NUM 301

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 84 "./src/silc.y"

	tnode *no;
    char *name;
    char* type;
    LinkedList* list;
    Field* field;
	

#line 158 "./include/silc.h"

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
