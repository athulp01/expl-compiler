/* A Bison parser, made by GNU Bison 3.5.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "./src/silc.y"

	#include <stdlib.h>
	#include <stdio.h>
	#include "evaluators.h"
    #include <string.h>
    #include "datastructures.h"

	int yylex(void);
  char virtual[1000];
    int yyerror(char*);
    int classno = 0, mid =0;
    FILE *out, *yyin;
    LinkedList *GSymList, *LSymList, *LVarList, *GVarList, *TypeList, *curLvar, *ClassList, *curClassField, *curClassMethod;
    Frame *curFrame;
    char *curClassName;
    int curMemory, curClassFieldMem;

    void checkArg(tnode* sym, tnode* arg) {
        if(sym == NULL) {
            if(arg == NULL) return;
            else {
                yyerror("Argument mismatch");
            }
        }
        if(arg == NULL) yyerror("Argument mismatch");
        if(arg->vartype != sym->vartype) yyerror("Argument mismatch");
        if(arg->type == OP || arg->type == FUNC) return;
        checkArg(sym->left, arg->left);
        checkArg(sym->right, arg->right);
    }

    Type* getTypeOfSymbol(char *oname) {
        char *sname = strdup(oname);
        char *name = strtok(sname, ".");
        GSymbol* sym = searchSymbol(name, curLvar);
        if (sym) {  // if it is a local variable
            Field* field = NULL;
            if(!strcmp(name, "self")) { // search in the fields of current class if begin with "self"
                char *tok;
                LinkedList *lst = curClassField;
                while((tok = strtok(NULL, "."))) {
                    LOG("field", tok)
                    field = getField(tok, lst);
                    if(!field) yyerror("Undefined field");
                    lst = field->type?field->type->fields:field->class->fields;
                } 
            }else {
                char *tok;
                LinkedList *lst = sym->type?sym->type->fields:sym->class->fields;
                while((tok = strtok(NULL, "."))) {
                    field = getField(tok, lst);
                    if(!field) yyerror("Undefined field");
                    lst = field->type?field->type->fields:field->class->fields;
                    
                }
            }
            return field?field->type:sym->type;            
        } else {
            sym = searchSymbol(name, GSymList);
            if (sym) {  // global variable
                Field* field = NULL;
                char *tok;
                LinkedList *lst = sym->type?sym->type->fields:sym->class->fields;
                while((tok = strtok(NULL, "."))) {
                    field = getField(tok, lst);
                    if(!field) yyerror("Undefined field");
                    lst = field->type?field->type->fields:field->class->fields;
                }
                return field?field->type:sym->type; 
            } else {
                printf("%s", sname);
                yyerror("Undefined variable");
                return NULL;
            }
        } 
    }

    ClassDef* getClassOfSymbol(char *oname) {
        char *sname = strdup(oname);
        char *name = strtok(sname, ".");
        GSymbol* sym = searchSymbol(name, curLvar);
        if (sym) {  // local variable
            Field* field = NULL;
            if(!strcmp(name, "self")) {
                char *tok;
                LinkedList *lst = curClassField;
                while((tok = strtok(NULL, "."))) {
                    field = getField(tok, lst);
                    if(!field) yyerror("Undefined field");
                    lst = field->type?field->type->fields:field->class->fields;
                } 
            }else {
                char *tok;
                LinkedList *lst = sym->type?sym->type->fields:sym->class->fields;
                while((tok = strtok(NULL, "."))) {
                    field = getField(tok, lst);
                    if(!field) yyerror("Undefined field");
                    lst = field->type?field->type->fields:field->class->fields;
                }
            }
            return field?field->class:sym->class;            
        } else {
            sym = searchSymbol(name, GSymList);
            if (sym) {  // global variable
                Field* field = NULL;
                char *tok;
                LinkedList *lst = sym->type?sym->type->fields:sym->class->fields;
                while((tok = strtok(NULL, "."))) {
                    field = getField(tok, lst);
                    if(!field) yyerror("Undefined field");
                    lst = field->type?field->type->fields:field->class->fields;
                }
                return field?field->class:sym->class; 
            } else {
                printf("%s", sname);
                yyerror("Undefined variable");
                return NULL;
            }
        } 
    }

    LinkedList* addParam(tnode *root) {
        if(root == NULL) return NULL;
        LinkedList *res = NULL;
        if(root->type != CONN) {
            res = (LinkedList*)malloc(sizeof(LinkedList));
            LSymbol* tmp = malloc(sizeof(LSymbol));
            *tmp = (LSymbol){.name = root->varname, .type = root->vartype};
            res->data = (void*)tmp;
            res->next = NULL;
            return res;
        }else {
            LinkedList *right = addParam(root->right);
            LinkedList *left = addParam(root->left);
            if(right) {
                right->next = left;
                return right;
            }
            return left;
        }

    }

#line 214 "./src/y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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
#line 147 "./src/silc.y"

	tnode *no;
    char *name;
    char* type;
    LinkedList* list;
    Field* field;
	

#line 377 "./src/y.tab.c"

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



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  20
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   638

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  59
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  116
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  289

#define YYUNDEFTOK  2
#define YYMAXUTOK   306


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      52,    53,     2,     2,     2,     2,    56,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    57,     2,    58,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    54,     2,    55,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   180,   180,   181,   182,   183,   184,   185,   186,   194,
     209,   210,   213,   242,   299,   300,   302,   316,   332,   335,
     336,   338,   369,   395,   396,   399,   406,   410,   413,   433,
     435,   442,   445,   448,   451,   454,   457,   460,   463,   466,
     469,   472,   475,   478,   479,   480,   481,   482,   483,   484,
     485,   488,   492,   494,   496,   500,   503,   506,   508,   512,
     515,   517,   519,   523,   528,   534,   537,   540,   544,   545,
     546,   547,   548,   549,   550,   551,   552,   555,   556,   560,
     561,   564,   565,   567,   571,   572,   573,   574,   576,   584,
     591,   592,   595,   596,   600,   616,   617,   625,   626,   629,
     630,   634,   654,   659,   664,   669,   674,   679,   689,   693,
     698,   713,   730,   737,   738,   739,   742
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "_DECL", "_ENDDECL", "_INT", "_STR",
  "_TEXT", "_MAIN", "_RET", "_TYPE", "_ENDTYPE", "_NULL", "_EXTENDS",
  "_IF", "_WHILE", "_THEN", "_ELSE", "_ENDIF", "_ENDWHILE", "_DO",
  "_BREAK", "_CONT", "_AND", "_INIT", "_ALLOC", "_FREE", "_CLASS", "_SELF",
  "_ENDCLASS", "_NEW", "_LT", "_GT", "_EQ", "_NE", "_LE", "_GE", "_PLUS",
  "_MINUS", "_MUL", "_DIV", "_END", "_BEGIN", "_READ", "_WRITE", "_SEMI",
  "_EQUALS", "_Q", "_COMMA", "_MOD", "_ID", "_NUM", "'('", "')'", "'{'",
  "'}'", "'.'", "'['", "']'", "$accept", "program", "mainblock",
  "classdefblock", "classdef", "methodlst", "methoddef",
  "classmemberblock", "classmemberlst", "classmember", "methodcall",
  "typedefblock", "fielddef", "fieldlst", "typedef", "field", "expr",
  "return", "read", "init", "alloc", "new", "free", "write", "assgn",
  "ifstmt", "break", "cont", "whilestmt", "stmt", "stmtList", "arg",
  "args", "funccall", "funcstmt", "type", "ldeclblock", "ldecllist",
  "ldecl", "lvarlist", "gdeclblock", "gdecllist", "gdecl", "gvarlist",
  "fdefblock", "fdef", "paramlist", "params", "param", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,    40,    41,   123,   125,    46,    91,    93
};
# endif

#define YYPACT_NINF (-153)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       0,    -3,   -35,   -22,    26,    56,  -153,    89,  -153,     4,
    -153,     3,  -153,  -153,    46,     5,  -153,    17,    -8,    48,
    -153,  -153,    72,  -153,    89,  -153,    26,  -153,    94,     3,
    -153,    -9,    78,  -153,  -153,   107,   131,   155,   115,  -153,
       3,   138,  -153,  -153,   107,   140,  -153,   152,  -153,    57,
     153,   158,   107,   107,   160,  -153,     3,   107,   166,   144,
     169,  -153,   161,    68,   207,  -153,   175,   155,     7,  -153,
      46,    92,  -153,   173,   218,  -153,   172,  -153,  -153,   107,
    -153,   107,   176,  -153,  -153,   107,  -153,  -153,    90,   195,
    -153,   174,     8,   189,   181,  -153,   185,   184,    96,  -153,
    -153,   107,  -153,   193,     9,  -153,   296,    18,  -153,  -153,
     217,   191,  -153,   147,  -153,  -153,   196,   197,   202,   205,
     199,   200,   201,   203,   208,   210,   148,  -153,    59,  -153,
    -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,
    -153,   163,   209,  -153,   296,   227,  -153,   216,  -153,   221,
      37,    37,  -153,  -153,   220,   222,   224,   226,   228,    37,
      37,    37,   229,    37,    37,    37,   230,    37,   236,  -153,
    -153,   163,   296,    21,  -153,  -153,  -153,    52,  -153,    37,
     -34,   402,  -153,   425,  -153,  -153,   235,   243,   118,   143,
     448,   494,   570,  -153,   102,  -153,   318,   513,   108,  -153,
     532,   234,   240,   163,   296,   255,    37,   471,    37,    37,
      37,    37,    37,    37,    37,    37,    37,    37,    37,    37,
     266,   278,   256,  -153,   257,    37,   258,   259,  -153,    37,
    -153,   254,  -153,  -153,  -153,  -153,   250,   265,   163,   296,
     346,  -153,   589,   589,   589,   589,   589,   589,   589,     1,
       1,  -153,  -153,   171,   296,   296,  -153,  -153,   374,  -153,
    -153,  -153,    37,  -153,   252,   267,   163,  -153,   215,   242,
     261,   551,  -153,   260,   268,   296,   271,   279,   280,  -153,
    -153,   272,   269,  -153,  -153,  -153,  -153,   283,  -153
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     8,     0,    11,     0,
      24,     0,    98,    89,     0,     0,   100,     0,     0,     0,
       1,    10,     0,     5,     0,    23,    89,     6,     0,     0,
     109,   106,     0,    97,    99,     0,     0,     0,     0,     7,
       0,     0,     4,   108,   115,     0,   101,     0,    27,     0,
       0,     0,     0,     0,     0,     3,     0,   115,     0,     0,
     112,   114,     0,   102,     0,    26,     0,     0,     0,    20,
       0,     0,    15,     0,     0,     2,     0,   116,   107,     0,
     105,   115,     0,    28,    25,     0,    18,    19,     0,     0,
      14,     0,     0,     0,     0,   113,     0,     0,     0,    21,
      12,   115,    91,     0,     0,    93,     0,     0,   104,   103,
       0,     0,    96,     0,    90,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    86,     0,    68,
      85,    84,    87,    76,    69,    70,    71,    73,    74,    72,
      78,     0,     0,    75,     0,     0,    13,     0,    94,     0,
       0,     0,    65,    66,     0,     0,     0,     0,     0,     0,
       0,    80,     0,     0,     0,    80,     0,     0,     0,    77,
      88,     0,     0,     0,    95,    47,    50,    45,    44,     0,
      49,     0,    48,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    79,    82,     0,    29,     0,     0,     0,    30,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,    60,    80,
      83,     0,    62,    22,    51,     9,     0,     0,     0,     0,
       0,    43,    41,    38,    39,    37,    40,    35,    36,    31,
      32,    33,    34,    42,     0,     0,    58,    52,     0,    54,
      59,    81,     0,   111,     0,     0,     0,    46,     0,     0,
       0,     0,   110,     0,     0,     0,     0,     0,     0,    61,
      17,     0,     0,    64,    67,    53,    16,     0,    63
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -153,  -153,    84,   314,    -1,   244,    19,   263,  -153,   264,
    -153,  -153,   282,  -153,   324,  -106,  -134,  -152,  -153,  -153,
    -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -139,
    -136,   105,   170,  -102,  -153,   130,  -100,  -153,   232,  -153,
      13,  -153,   322,   273,   298,    65,    53,  -153,   281
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,     6,     7,     8,    71,    72,    53,    68,    69,
     127,     9,    48,    49,    10,   180,   192,   168,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   193,   194,   182,   143,    58,    93,   104,   105,   113,
      11,    15,    16,    32,    29,    30,    59,    60,    61
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     128,    12,   169,     1,   142,    36,    21,   145,   171,    33,
       2,    86,   102,   114,     2,    17,   181,   183,   165,   202,
      22,    92,   166,    21,    92,   190,   191,     3,    18,   196,
     197,     3,   169,   200,    19,   128,   203,    40,   128,   142,
     217,   218,   142,    44,   175,   207,    37,    13,    45,   176,
       4,   237,   189,    26,     4,    13,    20,    13,    13,    13,
     144,   120,   121,   204,   169,   128,   128,   123,   238,   142,
     142,    35,   240,   205,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   265,   177,   178,   179,
      90,   258,     1,    23,    43,    27,    31,   128,   128,   169,
      38,   142,   142,   266,   161,   164,    39,    13,   162,   206,
      76,   165,    64,    42,   274,   166,     3,    90,   268,   269,
      81,    43,     4,    46,    55,    82,    47,   169,   271,   169,
     169,    14,   128,   128,    96,    99,   142,   142,    47,   282,
      75,    28,    13,   169,    41,    14,    13,    89,   128,   128,
     229,   110,   142,   142,   111,   230,   229,    13,    52,    28,
     128,   233,   128,   128,   142,    50,   142,   142,    54,   128,
      28,   224,   167,   142,   162,   225,   128,   116,   117,    50,
     142,    51,    70,    73,   118,   119,    28,   120,   121,   122,
      57,    62,   148,   123,   160,   149,   226,    78,    70,   166,
     161,    73,    63,    66,   162,   163,   124,   125,   215,   216,
     217,   218,    67,   126,    74,    73,    77,    79,    83,    80,
      84,    92,   103,    91,   100,    94,   101,    97,    73,   116,
     117,   106,   275,   276,   103,   107,   118,   119,   108,   120,
     121,   122,   109,   112,   147,   123,   146,   152,   150,   151,
     153,   154,   155,   156,   170,   157,   116,   117,   124,   125,
     158,   277,   159,   118,   119,   126,   120,   121,   122,   172,
     173,   174,   123,   184,   186,   185,   187,   201,   188,   195,
     199,   236,   254,   116,   117,   124,   125,   287,   222,   235,
     118,   119,   126,   120,   121,   122,   223,   239,   255,   123,
     262,   256,   257,   259,   260,   263,   264,   272,   273,   281,
     116,   117,   124,   125,   278,   280,   283,   118,   119,   126,
     120,   121,   122,    24,   284,   285,   123,   286,   288,    98,
      85,    65,    87,    25,   261,   198,   115,    34,    56,   124,
     125,   208,     0,    88,     0,     0,   126,     0,     0,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,     0,
      95,     0,     0,     0,     0,     0,     0,   219,     0,   208,
       0,     0,     0,     0,     0,     0,   231,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,     0,     0,     0,
       0,     0,     0,     0,     0,   219,     0,   208,     0,     0,
       0,     0,     0,     0,   267,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,     0,     0,     0,     0,     0,
       0,     0,     0,   219,     0,   208,     0,     0,     0,     0,
       0,     0,   270,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,     0,     0,     0,     0,     0,   208,     0,
       0,   219,     0,     0,     0,   220,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,     0,     0,     0,     0,
       0,   208,     0,     0,   219,     0,     0,     0,   221,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,     0,
       0,     0,     0,     0,   208,     0,     0,   219,     0,     0,
       0,   227,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,     0,     0,     0,     0,     0,   208,     0,     0,
     219,     0,     0,     0,   241,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,     0,   208,     0,     0,   228,
       0,     0,     0,   219,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,     0,   208,     0,     0,   232,     0,
       0,     0,   219,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,     0,   208,     0,     0,   234,     0,     0,
       0,   219,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,     0,   208,     0,     0,   279,     0,     0,     0,
     219,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,     0,    -1,     0,     0,     0,     0,     0,     0,   219,
      -1,    -1,    -1,    -1,    -1,    -1,   215,   216,   217,   218,
       0,     0,     0,     0,     0,     0,     0,     0,   219
};

static const yytype_int16 yycheck[] =
{
     106,     4,   141,     3,   106,    13,     7,   107,   144,     4,
      10,     4,     4,     4,    10,    50,   150,   151,    52,   171,
       7,     3,    56,    24,     3,   159,   160,    27,    50,   163,
     164,    27,   171,   167,     8,   141,   172,    24,   144,   141,
      39,    40,   144,    52,     7,   179,    54,    50,    57,    12,
      50,   203,   158,    50,    50,    50,     0,    50,    50,    50,
      42,    24,    25,    42,   203,   171,   172,    30,   204,   171,
     172,    54,   206,   173,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   238,    50,    51,    52,
      71,   225,     3,     9,    29,    11,    50,   203,   204,   238,
      52,   203,   204,   239,    52,    46,    22,    50,    56,    57,
      57,    52,    55,    29,   266,    56,    27,    98,   254,   255,
      52,    56,    50,    45,    40,    57,    48,   266,   262,   268,
     269,     1,   238,   239,    81,    45,   238,   239,    48,   275,
      56,    11,    50,   282,    50,    15,    50,    55,   254,   255,
      48,    55,   254,   255,   101,    53,    48,    50,     3,    29,
     266,    53,   268,   269,   266,    35,   268,   269,    53,   275,
      40,    53,     9,   275,    56,    57,   282,    14,    15,    49,
     282,    50,    52,    53,    21,    22,    56,    24,    25,    26,
      52,    51,    45,    30,    46,    48,    53,    53,    68,    56,
      52,    71,    50,    50,    56,    57,    43,    44,    37,    38,
      39,    40,    54,    50,    54,    85,    50,    48,    11,    58,
      45,     3,    92,    50,    29,    53,    52,    51,    98,    14,
      15,    42,    17,    18,   104,    54,    21,    22,    53,    24,
      25,    26,    58,    50,    53,    30,    29,    45,    52,    52,
      45,    52,    52,    52,    45,    52,    14,    15,    43,    44,
      52,    19,    52,    21,    22,    50,    24,    25,    26,    42,
      54,    50,    30,    53,    50,    53,    50,    41,    50,    50,
      50,    41,    16,    14,    15,    43,    44,    18,    53,    55,
      21,    22,    50,    24,    25,    26,    53,    42,    20,    30,
      46,    45,    45,    45,    45,    55,    41,    55,    41,    41,
      14,    15,    43,    44,    53,    55,    45,    21,    22,    50,
      24,    25,    26,     9,    45,    45,    30,    55,    45,    85,
      67,    49,    68,     9,   229,   165,   104,    15,    40,    43,
      44,    23,    -1,    70,    -1,    -1,    50,    -1,    -1,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    58,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    49,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    58,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    49,    -1,    -1,    -1,    53,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    49,    -1,    -1,    -1,    53,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    49,    -1,    -1,
      -1,    53,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      49,    -1,    -1,    -1,    53,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    -1,    23,    -1,    -1,    45,
      -1,    -1,    -1,    49,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    23,    -1,    -1,    45,    -1,
      -1,    -1,    49,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    -1,    23,    -1,    -1,    45,    -1,    -1,
      -1,    49,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    23,    -1,    -1,    45,    -1,    -1,    -1,
      49,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    10,    27,    50,    60,    61,    62,    63,    70,
      73,    99,     4,    50,    94,   100,   101,    50,    50,     8,
       0,    63,    99,    61,    62,    73,    50,    61,    94,   103,
     104,    50,   102,     4,   101,    54,    13,    54,    52,    61,
      99,    50,    61,   104,    52,    57,    45,    48,    71,    72,
      94,    50,     3,    66,    53,    61,   103,    52,    94,   105,
     106,   107,    51,    50,    55,    71,    50,    54,    67,    68,
      94,    64,    65,    94,    54,    61,   105,    50,    53,    48,
      58,    52,    57,    11,    45,    66,     4,    68,   102,    55,
      65,    50,     3,    95,    53,   107,   105,    51,    64,    45,
      29,    52,     4,    94,    96,    97,    42,    54,    53,    58,
      55,   105,    50,    98,     4,    97,    14,    15,    21,    22,
      24,    25,    26,    30,    43,    44,    50,    69,    74,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    92,    93,    42,    95,    29,    53,    45,    48,
      52,    52,    45,    45,    52,    52,    52,    52,    52,    52,
      46,    52,    56,    57,    46,    52,    56,     9,    76,    88,
      45,    89,    42,    54,    50,     7,    12,    50,    51,    52,
      74,    75,    92,    75,    53,    53,    50,    50,    50,    74,
      75,    75,    75,    90,    91,    50,    75,    75,    91,    50,
      75,    41,    76,    89,    42,    95,    57,    75,    23,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    49,
      53,    53,    53,    53,    53,    57,    53,    53,    45,    48,
      53,    58,    45,    53,    45,    55,    41,    76,    89,    42,
      75,    53,    75,    75,    75,    75,    75,    75,    75,    75,
      75,    75,    75,    75,    16,    20,    45,    45,    75,    45,
      45,    90,    46,    55,    41,    76,    89,    58,    89,    89,
      58,    75,    55,    41,    76,    17,    18,    19,    53,    45,
      55,    41,    89,    45,    45,    45,    55,    18,    45
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    59,    60,    60,    60,    60,    60,    60,    60,    61,
      62,    62,    63,    63,    64,    64,    65,    65,    66,    67,
      67,    68,    69,    70,    70,    71,    72,    72,    73,    74,
      74,    75,    75,    75,    75,    75,    75,    75,    75,    75,
      75,    75,    75,    75,    75,    75,    75,    75,    75,    75,
      75,    76,    77,    77,    77,    78,    79,    80,    81,    82,
      83,    83,    83,    84,    84,    85,    86,    87,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    89,    89,    90,
      90,    91,    91,    92,    92,    92,    92,    92,    93,    94,
      95,    95,    96,    96,    97,    98,    98,    99,    99,   100,
     100,   101,   102,   102,   102,   102,   102,   102,   103,   103,
     104,   104,   105,   106,   106,   106,   107
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     5,     4,     3,     2,     2,     3,     1,    11,
       2,     1,     7,     9,     2,     1,    12,    11,     3,     2,
       1,     3,     4,     2,     1,     3,     2,     1,     6,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     1,     4,     1,     1,     1,
       1,     3,     5,     8,     5,     3,     3,     4,     5,     5,
       4,     7,     4,    10,     8,     2,     2,     8,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     1,
       0,     3,     1,     4,     1,     1,     1,     1,     2,     1,
       3,     2,     2,     1,     3,     3,     1,     3,     2,     2,
       1,     3,     3,     6,     6,     4,     1,     4,     2,     1,
      12,    11,     1,     3,     1,     0,     2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 9:
#line 194 "./src/silc.y"
                                                                                    {
                                                             tnode *node = createNode(FUNC, "main", -1, connect((yyvsp[-3].no),(yyvsp[-2].no)), NULL); (yyval.no)->vartype = 0;
                                                             GSymbol* tmp = malloc(sizeof(GSymbol));
                                                             Frame *frame = (Frame*)malloc(sizeof(Frame));
                                                             frame->Lvars = (yyvsp[-5].list);
                                                             *tmp = (GSymbol){.name="main", .params=NULL, .frame=frame};
                                                             GSymList = addNode(tmp, sizeof(GSymbol), GSymList);
                                                             fprintf(out ,"main:\nBRKP\n");
                                                             fprintf(out, "%s", virtual);
                                                             eval_func(node, out);
                                                            }
#line 1936 "./src/y.tab.c"
    break;

  case 12:
#line 213 "./src/silc.y"
                                                                          { 
                                                                 if(searchClass((yyvsp[-5].name), ClassList)) yyerror("Duplicate class");
                                                                ClassDef *class = (ClassDef*)malloc(sizeof(ClassDef));
                                                                 class->name = (yyvsp[-5].name);
                                                                 curClassName = (yyvsp[-5].name);
                                                                 class->idx = classno++;
                                                                 class->fields = curClassField;
                                                                 class->methods = curClassMethod;
                                                                 LinkedList *func = (yyvsp[-2].list);
                                                                 ClassList = addNode(class, sizeof(ClassDef), ClassList);
                                                                 tnode *tmp;
                                                                 while(func) {
                                                                     tmp = (tnode*)func->data;
                                                                     char *s = malloc(sizeof((yyvsp[-5].name))+sizeof(tmp->varname)+2);
                                                                     strcpy(s, (yyvsp[-5].name));
                                                                     strcat(s, ".");
                                                                     strcat(s, tmp->varname);
                                                                     char t[50];
                                                                     sprintf(t, "MOV R19, %s\nMOV [%d], R19\n", s, class->idx*8 + 4096+searchMethod(tmp->varname, curClassMethod)->idx);
                                                                     strcat(virtual, t);
                                                                     fprintf(out, "%s:\n",s);
                                                                     eval_method(tmp, curClassMethod, out);
                                                                     func = func->next;
                                                                 }
                                                                 curClassMethod = NULL;
                                                                 curClassField = NULL;
                                                                  curMemory = (classno + 1)*8 + 4096;
                                                                  mid=0;
                                                                }
#line 1970 "./src/y.tab.c"
    break;

  case 13:
#line 242 "./src/silc.y"
                                                                                       { 
                                                                 if(searchClass((yyvsp[-7].name), ClassList)) yyerror("Duplicate class");
                                                                 ClassDef *parent = searchClass((yyvsp[-5].name), ClassList);
                                                                 if(!parent) yyerror("Parent class is not defines");
                                                                 ClassDef *class = (ClassDef*)malloc(sizeof(ClassDef));
                                                                 class->name = (yyvsp[-7].name);
                                                                 curClassName = (yyvsp[-7].name);
                                                                 class->idx = classno++;
                                                                 class->fields = curClassField;
                                                                 class->parent = parent;
                                                                 int nummethods = 0;
                                                                 LinkedList *mtds = curClassMethod;
                                                                 while(mtds) {
                                                                    nummethods++;
                                                                     char s[100];
                                                                     strcpy(s, (yyvsp[-7].name));
                                                                     strcat(s, ".");
                                                                     strcat(s, ((Method*)mtds->data)->name);
                                                                     char t[50];
                                                                    sprintf(t, "MOV R19, %s\nMOV [%d], R19\n",s, class->idx*8+4096+((Method*)mtds->data)->idx);
                                                                     strcat(virtual, t);
                                                                    mtds = mtds->next;
                                                                  }
                                                                 mtds = copyList(parent->methods, sizeof(Method));
                                                                 LinkedList* parentm = mtds;
                                                                 while(mtds) {
                                                                    ((Method*)mtds->data)->idx += nummethods;
                                                                     char s[100];
                                                                     strcpy(s, class->parent->name);
                                                                     strcat(s, ".");
                                                                     strcat(s, ((Method*)mtds->data)->name);
                                                                     char t[50];
                                                                    sprintf(t, "MOV R19, %s\nMOV [%d], R19\n",s, class->idx*8+4096+((Method*)mtds->data)->idx);
                                                                    strcat(virtual, t);
                                                                    mtds = mtds->next;
                                                                  }
                                                                 class->methods = connectList(curClassMethod, parentm, sizeof(Method));
                                                                 class->parent = parent;
                                                                 LinkedList *func = (yyvsp[-2].list);
                                                                 ClassList = addNode(class, sizeof(ClassDef), ClassList);
                                                                 tnode *tmp;
                                                                 while(func) {
                                                                     tmp = (tnode*)func->data;
                                                                     char *s = malloc(sizeof((yyvsp[-7].name))+sizeof(tmp->varname)+2);
                                                                     strcpy(s, (yyvsp[-7].name));
                                                                     strcat(s, ".");
                                                                     strcat(s, tmp->varname);
                                                                     fprintf(out, "%s:\n",s);
                                                                     eval_method(tmp, curClassMethod, out);
                                                                     func = func->next;
                                                                 }
                                                                 curClassMethod = NULL;
                                                                 mid = 0;
                                                                 curClassField = NULL;
                                                                  curMemory = (classno + 1)*8 + 4096;
                                                                }
#line 2031 "./src/y.tab.c"
    break;

  case 14:
#line 299 "./src/silc.y"
                                                                {(yyval.list) = connectList((yyvsp[-1].list), (yyvsp[0].list), sizeof(tnode));}
#line 2037 "./src/y.tab.c"
    break;

  case 15:
#line 300 "./src/silc.y"
                                                                {(yyval.list) = (yyvsp[0].list);}
#line 2043 "./src/y.tab.c"
    break;

  case 16:
#line 302 "./src/silc.y"
                                                                                          {
                                                                    Method* tmp = searchMethod((yyvsp[-10].name), curClassMethod);
                                                                    if(!tmp) yyerror("Method not declared");
                                                                    Frame *frame = (Frame*)malloc(sizeof(Frame));
                                                                    frame->Lvars = (yyvsp[-5].list);
                                                                    tmp->frame = frame;
                                                                    tmp->type = searchType((yyvsp[-11].name), TypeList); 
                                                                    if((yyvsp[-2].no)->left->type == VAR) {  
                                                                        if(strcmp((yyvsp[-11].name), getTypeOfSymbol((yyvsp[-2].no)->left->varname)->name)) yyerror("Return type is not correct");
                                                                    } else if(strcmp((yyvsp[-11].name), (yyvsp[-2].no)->left->vartype->name)) yyerror("Return type is not correct");
                                                                    LinkedList *lst = (LinkedList*)malloc(sizeof(LinkedList));
                                                                    lst->data = createNode(METHOD, (yyvsp[-10].name), -1, connect((yyvsp[-3].no), (yyvsp[-2].no)), (yyvsp[-8].no)); ((tnode*)lst->data)->vartype = searchType((yyvsp[-11].name), TypeList);
                                                                    (yyval.list) = lst;
                                                                    }
#line 2062 "./src/y.tab.c"
    break;

  case 17:
#line 316 "./src/silc.y"
                                                                                {
                                                                    Method* tmp = searchMethod((yyvsp[-9].name), curClassMethod);
                                                                    Frame *frame = (Frame*)malloc(sizeof(Frame));
                                                                    frame->Lvars = NULL;
                                                                    tmp->frame = frame;
                                                                    tmp->type = searchType((yyvsp[-10].name), TypeList); 
                                                                    if((yyvsp[-2].no)->left->type == VAR) {  
                                                                        if(strcmp((yyvsp[-10].name), getTypeOfSymbol((yyvsp[-2].no)->left->varname)->name)) yyerror("Return type is not correct");
                                                                    } else if(strcmp((yyvsp[-10].name), (yyvsp[-2].no)->left->vartype->name)) yyerror("Return type is not correct");
                                                                    LinkedList *lst = (LinkedList*)malloc(sizeof(LinkedList));
                                                                    lst->data = createNode(METHOD, (yyvsp[-9].name), -1, connect((yyvsp[-3].no), (yyvsp[-2].no)), (yyvsp[-7].no)); ((tnode*)lst->data)->vartype = searchType((yyvsp[-10].name), TypeList);
                                                                    (yyval.list) = lst;
                                                                    }
#line 2080 "./src/y.tab.c"
    break;

  case 18:
#line 332 "./src/silc.y"
                                                               {(yyval.list) = (yyvsp[-1].list);}
#line 2086 "./src/y.tab.c"
    break;

  case 19:
#line 335 "./src/silc.y"
                                                                {(yyval.list) = connectList((yyvsp[-1].list), (yyvsp[0].list), sizeof(Field));}
#line 2092 "./src/y.tab.c"
    break;

  case 20:
#line 336 "./src/silc.y"
                                                                    {(yyval.list) = (yyvsp[0].list);}
#line 2098 "./src/y.tab.c"
    break;

  case 21:
#line 338 "./src/silc.y"
                                                              {
                                                                LinkedList *fields= NULL, *methods = NULL;
                                                                LinkedList* gvars = (yyvsp[-1].list);
                                                                while(gvars) {
                                                                    GVariable *var = (GVariable*)(gvars->data);
                                                                    if(var->isfunc) {
                                                                        if(searchMethod(var->name, curClassMethod)) yyerror("Duplicate method");
                                                                        Method* tmp = (Method*)malloc(sizeof(Method));
                                                                        ClassDef *class = searchClass((yyvsp[-2].name), ClassList);
                                                                        Type *type = (Type*)searchType((yyvsp[-2].name), TypeList);
                                                                        if(type==NULL && class==NULL) yyerror("Undefined type");
                                                                        *tmp = (Method){.name=var->name, .type=type, .class=class, .params=var->params};
                                                                        tmp->idx = mid++;
                                                                        methods = addNode(tmp, sizeof(Method), methods);
                                                                    } else {
                                                                        if(getField(var->name, curClassField)) yyerror("Duplicate field");
                                                                        Field* tmp = (Field*)malloc(sizeof(Field));
                                                                        ClassDef *class = searchClass((yyvsp[-2].name), ClassList);
                                                                        Type *type = (Type*)searchType((yyvsp[-2].name), TypeList);
                                                                        if(type==NULL && class==NULL) yyerror("Undefined type");
                                                                        *tmp = (Field){.name=var->name, .type=type, .class=class, .idx = curClassFieldMem++};
                                                                        if(class) curClassFieldMem++;
                                                                        fields = addNode(tmp, sizeof(Field), fields);
                                                                    }
                                                                    gvars = gvars->next;
                                                                }
                                                                curClassMethod = connectList(curClassMethod, methods, sizeof(Method));
                                                                curClassField = connectList(curClassField, fields, sizeof(Field));
                                                            }
#line 2132 "./src/y.tab.c"
    break;

  case 22:
#line 369 "./src/silc.y"
                                                    {   char *dup = strdup((yyvsp[-3].name));
                                                        char *prev;
                                                
                                                        LOG("methodname", dup)char *s = strtok(dup, ".");
                                                        GSymbol* sym = (GSymbol*)searchSymbol(s, GSymList);
                                                        if(sym==NULL) sym = (GSymbol*)searchSymbol(s, curLvar);
                                                        if(sym== NULL) yyerror("Function is not declared");
                                                        LinkedList *lst = sym->class->fields, *methods = sym->class->methods;
                                                        Field *field;
                                                        prev = strdup(s);
                                                        while((s=strtok(NULL, "."))) {
                                                            prev = strdup(s);
                                                            field = getField(s, lst);
                                                            if(field) {
                                                                methods = field->class->methods;
                                                                lst = field->class->fields;
                                                            }
                                                        }
                                                        (yyval.no) = createNode(METHOD, (yyvsp[-3].name), -1, (yyvsp[-1].no), NULL);
                                                        Method* method = searchMethod(prev, methods);
                                                        if(!method) yyerror("Method not found");
                                                        (yyval.no)->vartype = method->type;
                                                        }
#line 2160 "./src/y.tab.c"
    break;

  case 23:
#line 395 "./src/silc.y"
                                                          {}
#line 2166 "./src/y.tab.c"
    break;

  case 24:
#line 396 "./src/silc.y"
                                                          {}
#line 2172 "./src/y.tab.c"
    break;

  case 25:
#line 399 "./src/silc.y"
                                                          { Field *field = (Field*)malloc(sizeof(Field));
                                                            Type *type = searchType((yyvsp[-2].name), TypeList);
                                                            *field = type?(Field){.name=(yyvsp[-1].name), .type=type}:(Field){.name=(yyvsp[-1].name), .ndef=(yyvsp[-2].name)};
                                                            (yyval.field) = field;
                                                          }
#line 2182 "./src/y.tab.c"
    break;

  case 26:
#line 406 "./src/silc.y"
                                                           { (yyvsp[0].field)->idx = ((Field*)(yyvsp[-1].list)->data)->idx + 1;
                                                             if(getField((yyvsp[0].field)->name, (yyvsp[-1].list)))
                                                                yyerror("Duplicate field entry");
                                                             (yyval.list) = addNode((yyvsp[0].field), sizeof(Field), (yyvsp[-1].list));}
#line 2191 "./src/y.tab.c"
    break;

  case 27:
#line 410 "./src/silc.y"
                                                           { (yyvsp[0].field)->idx = 1; (yyval.list) = addNode((yyvsp[0].field), sizeof(Field), NULL); }
#line 2197 "./src/y.tab.c"
    break;

  case 28:
#line 413 "./src/silc.y"
                                                          { if(searchType((yyvsp[-4].name), TypeList)!=NULL) yyerror("Type is already defined");
                                                            Type *type = (Type*)malloc(sizeof(Type));
                                                            *type = (Type){.name=(yyvsp[-4].name), .fields=(yyvsp[-2].list)};
                                                            LinkedList *field = type->fields;
                                                            int size = 0;
                                                            while(field) {
                                                                size++;
                                                                if(!((Field*)field->data)->type) {
                                                                    if(!strcmp(((Field*)field->data)->ndef, (yyvsp[-4].name))) 
                                                                        ((Field*)field->data)->type = type;
                                                                    else yyerror("Undefined Type");
                                                                }
                                                                field = field->next;
                                                            }
                                                            if(size > 7) yyerror("Size of user defined type cannot be greater than 8");
                                                            type->size = size;
                                                             TypeList = addNode(type, sizeof(Type), TypeList);
                                                           }
#line 2220 "./src/y.tab.c"
    break;

  case 29:
#line 433 "./src/silc.y"
                                                            {char *tmp = (char*)malloc(strlen((yyvsp[-2].name)) + strlen((yyvsp[0].name)) + 1);
                                                            strcpy(tmp, (yyvsp[-2].name)); strcat(tmp, ".");strcat(tmp, (yyvsp[0].name)); (yyval.name) = tmp;}
#line 2227 "./src/y.tab.c"
    break;

  case 30:
#line 435 "./src/silc.y"
                                                            {char *tmp = (char*)malloc(strlen((yyvsp[-2].name)) + strlen((yyvsp[0].name)) + 1);
                                                            strcpy(tmp, (yyvsp[-2].name)); strcat(tmp, ".");strcat(tmp, (yyvsp[0].name)); (yyval.name) = tmp;}
#line 2234 "./src/y.tab.c"
    break;

  case 31:
#line 442 "./src/silc.y"
                                                            {(yyval.no) = createNode(OP, "+", -1, (yyvsp[-2].no), (yyvsp[0].no)); 
                                                        (yyval.no)->vartype = searchType("int", TypeList);}
#line 2241 "./src/y.tab.c"
    break;

  case 32:
#line 445 "./src/silc.y"
                                                            {(yyval.no) = createNode(OP, "-", -1, (yyvsp[-2].no), (yyvsp[0].no)); 
                                                        (yyval.no)->vartype = searchType("int", TypeList);}
#line 2248 "./src/y.tab.c"
    break;

  case 33:
#line 448 "./src/silc.y"
                                                                {(yyval.no) = createNode(OP, "*", -1, (yyvsp[-2].no), (yyvsp[0].no)); 
                                                        (yyval.no)->vartype = searchType("int", TypeList);}
#line 2255 "./src/y.tab.c"
    break;

  case 34:
#line 451 "./src/silc.y"
                                                                {(yyval.no) = createNode(OP, "/", -1, (yyvsp[-2].no), (yyvsp[0].no)); 
                                                        (yyval.no)->vartype = searchType("int", TypeList);}
#line 2262 "./src/y.tab.c"
    break;

  case 35:
#line 454 "./src/silc.y"
                                                        {(yyval.no) = createNode(OP, "<=", -1, (yyvsp[-2].no), (yyvsp[0].no)); 
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2269 "./src/y.tab.c"
    break;

  case 36:
#line 457 "./src/silc.y"
                                                        {(yyval.no) = createNode(OP, ">=", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2276 "./src/y.tab.c"
    break;

  case 37:
#line 460 "./src/silc.y"
                                                        {(yyval.no) = createNode(OP, "==", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2283 "./src/y.tab.c"
    break;

  case 38:
#line 463 "./src/silc.y"
                                                        {(yyval.no) = createNode(OP, "<", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2290 "./src/y.tab.c"
    break;

  case 39:
#line 466 "./src/silc.y"
                                                        {(yyval.no) = createNode(OP, ">", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2297 "./src/y.tab.c"
    break;

  case 40:
#line 469 "./src/silc.y"
                                                        {(yyval.no) = createNode(OP, "!=", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2304 "./src/y.tab.c"
    break;

  case 41:
#line 472 "./src/silc.y"
                                                        {(yyval.no) = createNode(OP, "AND", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2311 "./src/y.tab.c"
    break;

  case 42:
#line 475 "./src/silc.y"
                                                        {(yyval.no) = createNode(OP, "%", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("int", TypeList);}
#line 2318 "./src/y.tab.c"
    break;

  case 43:
#line 478 "./src/silc.y"
                                                                {(yyval.no) = (yyvsp[-1].no);}
#line 2324 "./src/y.tab.c"
    break;

  case 44:
#line 479 "./src/silc.y"
                                                                    {(yyval.no) = (yyvsp[0].no); (yyval.no)->vartype = searchType("int", TypeList);}
#line 2330 "./src/y.tab.c"
    break;

  case 45:
#line 480 "./src/silc.y"
                                                        {(yyval.no) = createNode(VAR, (yyvsp[0].name), -1, NULL, NULL); (yyval.no)->vartype = getTypeOfSymbol((yyvsp[0].name)); (yyval.no)->varclass = getClassOfSymbol((yyvsp[0].name));}
#line 2336 "./src/y.tab.c"
    break;

  case 46:
#line 481 "./src/silc.y"
                                                        {(yyval.no) = createNode(VAR, (yyvsp[-3].name), -1, (yyvsp[-1].no), NULL);(yyval.no)->vartype = getTypeOfSymbol((yyvsp[-3].name));}
#line 2342 "./src/y.tab.c"
    break;

  case 47:
#line 482 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no); (yyval.no)->vartype = searchType("str", TypeList);}
#line 2348 "./src/y.tab.c"
    break;

  case 48:
#line 483 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2354 "./src/y.tab.c"
    break;

  case 49:
#line 484 "./src/silc.y"
                                                        {(yyval.no) = createNode(VAR, (yyvsp[0].name), -1, NULL, NULL);(yyval.no)->vartype = getTypeOfSymbol((yyvsp[0].name)); (yyval.no)->varclass = getClassOfSymbol((yyvsp[0].name));}
#line 2360 "./src/y.tab.c"
    break;

  case 50:
#line 485 "./src/silc.y"
                                                        {(yyval.no) = createNode(CONST, "\0", 0, NULL, NULL); (yyval.no)->vartype = searchType("null", TypeList);}
#line 2366 "./src/y.tab.c"
    break;

  case 51:
#line 488 "./src/silc.y"
                                                        {(yyval.no) = createNode(RET, "\0", -1, (yyvsp[-1].no), NULL);}
#line 2372 "./src/y.tab.c"
    break;

  case 52:
#line 492 "./src/silc.y"
                                                        {tnode *tmp = createNode(VAR, (yyvsp[-2].name), -1, NULL, NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-2].name));
                                                        (yyval.no) = createNode(READ, "", -1, tmp, NULL);}
#line 2379 "./src/y.tab.c"
    break;

  case 53:
#line 494 "./src/silc.y"
                                                        {tnode *tmp = createNode(VAR, (yyvsp[-5].name), -1, (yyvsp[-3].no), NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-5].name));
                                                        (yyval.no) = createNode(READ, "", -1, tmp, NULL);}
#line 2386 "./src/y.tab.c"
    break;

  case 54:
#line 496 "./src/silc.y"
                                                        {tnode *tmp = createNode(VAR, (yyvsp[-2].name), -1, NULL, NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-2].name));
                                                        (yyval.no) = createNode(READ, "", -1, tmp, NULL);}
#line 2393 "./src/y.tab.c"
    break;

  case 55:
#line 500 "./src/silc.y"
                                                          {(yyval.no) = createNode(INIT, "", -1, NULL, NULL);(yyval.no)->vartype = searchType("int", TypeList);}
#line 2399 "./src/y.tab.c"
    break;

  case 56:
#line 503 "./src/silc.y"
                                                       {(yyval.no) = createNode(ALLOC, "", -1, NULL, NULL); (yyval.no)->vartype = searchType("int", TypeList);}
#line 2405 "./src/y.tab.c"
    break;

  case 57:
#line 506 "./src/silc.y"
                                                        {(yyval.no) = createNode(NEW, (yyvsp[-1].name), -1, NULL, NULL); (yyval.no)->vartype = searchType("int", TypeList);}
#line 2411 "./src/y.tab.c"
    break;

  case 58:
#line 508 "./src/silc.y"
                                                        {tnode *tmp = createNode(VAR, (yyvsp[-2].name), -1, NULL, NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-2].name));tmp->varclass = getClassOfSymbol((yyvsp[-2].name));
                                                        (yyval.no) = createNode(FFREE, "", -1, tmp, NULL);}
#line 2418 "./src/y.tab.c"
    break;

  case 59:
#line 512 "./src/silc.y"
                                                        {(yyval.no) = createNode(WRITE, "", -1, (yyvsp[-2].no), NULL);}
#line 2424 "./src/y.tab.c"
    break;

  case 60:
#line 515 "./src/silc.y"
                                                        { tnode *tmp = createNode(VAR, (yyvsp[-3].name), -1, NULL, NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-3].name)); tmp->varclass = getClassOfSymbol((yyvsp[-3].name));
                                                         (yyval.no) = createNode(ASSN, "", -1, tmp, (yyvsp[-1].no));}
#line 2431 "./src/y.tab.c"
    break;

  case 61:
#line 517 "./src/silc.y"
                                                        {tnode *tmp = createNode(VAR, (yyvsp[-6].name), -1, (yyvsp[-4].no), NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-6].name)); tmp->varclass = getClassOfSymbol((yyvsp[-6].name));
                                                         (yyval.no) = createNode(ASSN, "", -1, tmp, (yyvsp[-1].no));}
#line 2438 "./src/y.tab.c"
    break;

  case 62:
#line 519 "./src/silc.y"
                                                        { tnode *tmp = createNode(VAR, (yyvsp[-3].name), -1, NULL, NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-3].name)); tmp->varclass = getClassOfSymbol((yyvsp[-3].name));
                                                        (yyval.no) = createNode(ASSN, "", -1, tmp, (yyvsp[-1].no));}
#line 2445 "./src/y.tab.c"
    break;

  case 63:
#line 524 "./src/silc.y"
                                                        {
                                                            tnode *tmp = createNode(IF_BODY, "", -1, (yyvsp[-4].no), (yyvsp[-2].no));
                                                            (yyval.no) = createNode(IF, "", -1, (yyvsp[-7].no), tmp);
                                                        }
#line 2454 "./src/y.tab.c"
    break;

  case 64:
#line 528 "./src/silc.y"
                                                        {
                                                            tnode *tmp = createNode(IF_BODY, "", -1, (yyvsp[-2].no), NULL);
                                                            (yyval.no) = createNode(IF, "", -1, (yyvsp[-5].no), tmp);
                                                        }
#line 2463 "./src/y.tab.c"
    break;

  case 65:
#line 534 "./src/silc.y"
                                                        {(yyval.no) = createNode(BREAK, "", -1, NULL, NULL);}
#line 2469 "./src/y.tab.c"
    break;

  case 66:
#line 537 "./src/silc.y"
                                                        {(yyval.no) = createNode(CONT, "", -1, NULL, NULL);}
#line 2475 "./src/y.tab.c"
    break;

  case 67:
#line 541 "./src/silc.y"
                                                        {(yyval.no) = createNode(WHILE, "", -1, (yyvsp[-5].no), (yyvsp[-2].no));}
#line 2481 "./src/y.tab.c"
    break;

  case 68:
#line 544 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2487 "./src/y.tab.c"
    break;

  case 69:
#line 545 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2493 "./src/y.tab.c"
    break;

  case 70:
#line 546 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2499 "./src/y.tab.c"
    break;

  case 71:
#line 547 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2505 "./src/y.tab.c"
    break;

  case 72:
#line 548 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2511 "./src/y.tab.c"
    break;

  case 73:
#line 549 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2517 "./src/y.tab.c"
    break;

  case 74:
#line 550 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2523 "./src/y.tab.c"
    break;

  case 75:
#line 551 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2529 "./src/y.tab.c"
    break;

  case 76:
#line 552 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2535 "./src/y.tab.c"
    break;

  case 77:
#line 555 "./src/silc.y"
                                                        {(yyval.no) = connect((yyvsp[-1].no), (yyvsp[0].no));}
#line 2541 "./src/y.tab.c"
    break;

  case 78:
#line 556 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2547 "./src/y.tab.c"
    break;

  case 79:
#line 560 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2553 "./src/y.tab.c"
    break;

  case 80:
#line 561 "./src/silc.y"
                                                        {(yyval.no) = NULL;}
#line 2559 "./src/y.tab.c"
    break;

  case 81:
#line 564 "./src/silc.y"
                                                        {(yyval.no) = connect((yyvsp[0].no), (yyvsp[-2].no));}
#line 2565 "./src/y.tab.c"
    break;

  case 82:
#line 565 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2571 "./src/y.tab.c"
    break;

  case 83:
#line 567 "./src/silc.y"
                                                 {  GSymbol* sym = (GSymbol*)searchSymbol((yyvsp[-3].name), GSymList);
                                                    if(sym== NULL) yyerror("Function is not declared");
                                                    (yyval.no) = createNode(FUNC, (yyvsp[-3].name), -1, (yyvsp[-1].no), NULL);
                                                    (yyval.no)->vartype = sym->type;}
#line 2580 "./src/y.tab.c"
    break;

  case 84:
#line 571 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2586 "./src/y.tab.c"
    break;

  case 85:
#line 572 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2592 "./src/y.tab.c"
    break;

  case 86:
#line 573 "./src/silc.y"
                                                         {(yyval.no) = (yyvsp[0].no);}
#line 2598 "./src/y.tab.c"
    break;

  case 87:
#line 574 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2604 "./src/y.tab.c"
    break;

  case 88:
#line 576 "./src/silc.y"
                                                        { (yyval.no) = (yyvsp[-1].no);}
#line 2610 "./src/y.tab.c"
    break;

  case 89:
#line 584 "./src/silc.y"
                                                       {(yyval.name) = (yyvsp[0].name);}
#line 2616 "./src/y.tab.c"
    break;

  case 90:
#line 591 "./src/silc.y"
                                                        { (yyval.list) = (yyvsp[-1].list);}
#line 2622 "./src/y.tab.c"
    break;

  case 91:
#line 592 "./src/silc.y"
                                                        { (yyval.list) = NULL;}
#line 2628 "./src/y.tab.c"
    break;

  case 92:
#line 595 "./src/silc.y"
                                                        { (yyval.list) = connectList((yyvsp[-1].list), (yyvsp[0].list), sizeof(LSymbol));}
#line 2634 "./src/y.tab.c"
    break;

  case 93:
#line 596 "./src/silc.y"
                                                        {(yyval.list) = (yyvsp[0].list);}
#line 2640 "./src/y.tab.c"
    break;

  case 94:
#line 600 "./src/silc.y"
                                                        {   (yyval.list) = NULL;
                                                            LinkedList* gvars = (yyvsp[-1].list);
                                                            while(gvars) {
                                                                char *var = (char*)(gvars->data);
                                                                LSymbol* tmp = (LSymbol*)malloc(sizeof(LSymbol));
                                                                Type *type = (Type*)searchType((yyvsp[-2].name), TypeList);
                                                                ClassDef* class = searchClass((yyvsp[-2].name), ClassList);
                                                                if(type==NULL && class==NULL) yyerror("Undefined type");
                                                                *tmp = (LSymbol){.name=var, .type=type,.class=class, .size=1};
                                                                (yyval.list) = addNode(tmp, sizeof(LSymbol), (yyval.list));
                                                                gvars = gvars->next;
                                                            }
                                                            LinkedList *copy = copyList((yyval.list), sizeof(LSymbol));
                                                            curLvar = connectList(curLvar, copy, sizeof(LSymbol));
                                                        }
#line 2660 "./src/y.tab.c"
    break;

  case 95:
#line 616 "./src/silc.y"
                                                        {(yyval.list) = addNode(strdup((yyvsp[0].name)), sizeof((yyvsp[0].name)), (yyvsp[-2].list));}
#line 2666 "./src/y.tab.c"
    break;

  case 96:
#line 617 "./src/silc.y"
                                                        {(yyval.list) = addNode(strdup((yyvsp[0].name)), sizeof((yyvsp[0].name)), NULL);}
#line 2672 "./src/y.tab.c"
    break;

  case 97:
#line 625 "./src/silc.y"
                                                        {}
#line 2678 "./src/y.tab.c"
    break;

  case 98:
#line 626 "./src/silc.y"
                                                        {}
#line 2684 "./src/y.tab.c"
    break;

  case 99:
#line 629 "./src/silc.y"
                                                        {}
#line 2690 "./src/y.tab.c"
    break;

  case 100:
#line 630 "./src/silc.y"
                                                        {}
#line 2696 "./src/y.tab.c"
    break;

  case 101:
#line 634 "./src/silc.y"
                                                        {
                                                            LinkedList* gvars = (yyvsp[-1].list);
                                                            while(gvars) {
                                                                GVariable *var = (GVariable*)(gvars->data);
                                                                if(searchSymbol(var->name, GSymList)) {
                                                                    yyerror("Variable is already declared");
                                                                }
                                                                GSymbol* tmp = (GSymbol*)malloc(sizeof(GSymbol));
                                                                Type *type = (Type*)searchType((yyvsp[-2].name), TypeList);
                                                                ClassDef* class = searchClass((yyvsp[-2].name), ClassList);
                                                                if(type==NULL && class==NULL)
                                                                    yyerror("Undefined type");
                                                                *tmp = (GSymbol){.name=var->name, .type=type, .class=class, .size=var->size, .params=var->params, .binding=curMemory};
                                                                if(class) curMemory++;
                                                                GSymList = addNode(tmp, sizeof(GSymbol), GSymList);
                                                                gvars = gvars->next;
                                                                curMemory+=var->size;
                                                            }
                                                        }
#line 2720 "./src/y.tab.c"
    break;

  case 102:
#line 654 "./src/silc.y"
                                                        {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[0].name), .size=1};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), (yyvsp[-2].list));
                                                        }
#line 2729 "./src/y.tab.c"
    break;

  case 103:
#line 659 "./src/silc.y"
                                                        {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[-3].name), .size=(yyvsp[-1].no)->val};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), (yyvsp[-5].list));
                                                        }
#line 2738 "./src/y.tab.c"
    break;

  case 104:
#line 664 "./src/silc.y"
                                                        {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[-3].name), .params=(yyvsp[-1].no), .size=0};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), (yyvsp[-5].list));
                                                        }
#line 2747 "./src/y.tab.c"
    break;

  case 105:
#line 669 "./src/silc.y"
                                                        {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[-3].name), .size=(yyvsp[-1].no)->val};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), NULL);
                                                        }
#line 2756 "./src/y.tab.c"
    break;

  case 106:
#line 674 "./src/silc.y"
                                                        {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[0].name), .size=1};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), NULL);
                                                        }
#line 2765 "./src/y.tab.c"
    break;

  case 107:
#line 679 "./src/silc.y"
                                                        {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[-3].name), .params=(yyvsp[-1].no), .size=0, .isfunc=1};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), NULL);
                                                        }
#line 2774 "./src/y.tab.c"
    break;

  case 108:
#line 689 "./src/silc.y"
                                                        {
                                                         fprintf(out, "%s:\n", (yyvsp[0].no)->varname);
                                                         eval_func((yyvsp[0].no), out);}
#line 2782 "./src/y.tab.c"
    break;

  case 109:
#line 693 "./src/silc.y"
                                                        {
                                                         fprintf(out, "%s:\n", (yyvsp[0].no)->varname);
                                                         eval_func((yyvsp[0].no), out);}
#line 2790 "./src/y.tab.c"
    break;

  case 110:
#line 698 "./src/silc.y"
                                                                                   {
                                                                    GSymbol* tmp = searchSymbol((yyvsp[-10].name), GSymList);                                                                    
                                                                    if(tmp == NULL) yyerror("Function is not declared");
                                                                    if(tmp->frame != NULL) yyerror("Function is already defined");
                                                                    checkArg(tmp->params, (yyvsp[-8].no));
                                                                    Frame *frame = (Frame*)malloc(sizeof(Frame));
                                                                    frame->Lvars = (yyvsp[-5].list);
                                                                    tmp->frame = frame;
                                                                    tmp->type = searchType((yyvsp[-11].name), TypeList); 
                                                                    if((yyvsp[-2].no)->left->type == VAR) {  
                                                                        if(strcmp((yyvsp[-11].name), getTypeOfSymbol((yyvsp[-2].no)->left->varname)->name)) yyerror("Return type is not correct");
                                                                    } else if(strcmp((yyvsp[-11].name), (yyvsp[-2].no)->left->vartype->name)) yyerror("Return type is not correct");
                                                                    (yyval.no) = createNode(FUNC, (yyvsp[-10].name), -1, connect((yyvsp[-3].no), (yyvsp[-2].no)), (yyvsp[-8].no)); (yyval.no)->vartype = searchType((yyvsp[-11].name), TypeList);
                                                                    curLvar = NULL;
                                                                    }
#line 2810 "./src/y.tab.c"
    break;

  case 111:
#line 713 "./src/silc.y"
                                                                       {
                                                                    GSymbol* tmp = searchSymbol((yyvsp[-9].name), GSymList);                                                                    
                                                                    if(tmp == NULL) yyerror("Function is not declared");
                                                                    if(tmp->frame != NULL) yyerror("Function is already defined");
                                                                    checkArg(tmp->params, (yyvsp[-7].no));
                                                                    Frame *frame = (Frame*)malloc(sizeof(Frame));
                                                                    frame->Lvars = NULL;
                                                                    tmp->frame = frame;
                                                                    tmp->type = searchType((yyvsp[-10].name), TypeList); 
                                                                    if((yyvsp[-2].no)->left->type == VAR) {  
                                                                        if(strcmp((yyvsp[-10].name), getTypeOfSymbol((yyvsp[-2].no)->left->varname)->name)) yyerror("Return type is not correct");
                                                                    } else if(strcmp((yyvsp[-10].name), (yyvsp[-2].no)->left->vartype->name)) yyerror("Return type is not correct");
                                                                    (yyval.no) = createNode(FUNC, (yyvsp[-9].name), -1, connect((yyvsp[-3].no), (yyvsp[-2].no)), (yyvsp[-7].no)); (yyval.no)->vartype = searchType((yyvsp[-10].name), TypeList);
                                                                    curLvar = NULL;
                                                                    }
#line 2830 "./src/y.tab.c"
    break;

  case 112:
#line 730 "./src/silc.y"
                                                        {curLvar = addParam((yyvsp[0].no)); (yyval.no) = (yyvsp[0].no); 
                                                        LSymbol* tmp = malloc(sizeof(LSymbol));
                                                        ClassDef *type = malloc(sizeof(ClassDef));;
                                                        *type = (ClassDef){.fields=curClassField, .methods=curClassMethod};
                                                        *tmp = (LSymbol){.name = "self", .class=type };
                                                        curLvar = addNode(tmp, sizeof(LSymbol), curLvar);}
#line 2841 "./src/y.tab.c"
    break;

  case 113:
#line 737 "./src/silc.y"
                                                        { (yyval.no) = connect((yyvsp[0].no), (yyvsp[-2].no));}
#line 2847 "./src/y.tab.c"
    break;

  case 114:
#line 738 "./src/silc.y"
                                                        { (yyval.no) = (yyvsp[0].no);}
#line 2853 "./src/y.tab.c"
    break;

  case 115:
#line 739 "./src/silc.y"
                                                        { (yyval.no) = NULL;}
#line 2859 "./src/y.tab.c"
    break;

  case 116:
#line 742 "./src/silc.y"
                                                        {(yyval.no) = createNode(VAR, (yyvsp[0].name), -1, NULL, NULL); (yyval.no)->vartype = searchType((yyvsp[-1].name), TypeList);}
#line 2865 "./src/y.tab.c"
    break;


#line 2869 "./src/y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 747 "./src/silc.y"





int main(int argc, char **argv) {

    if(argc < 2) {
        printf("Usage: ./silc <source_file>\n");
        exit(1);
    }
    // TODO: change curMemory after vft
    curClassFieldMem = 1;
    mid = 0;
    GSymList = NULL;
    LSymList = NULL;
    LVarList = NULL;
    GVarList = NULL;
    curLvar = NULL;
    curMemory = 4096;
    Type *type = (Type*)malloc(sizeof(Type));
    *type = (Type){.name="int", .size=1, .fields=NULL};
    TypeList = addNode(type, sizeof(Type), TypeList);
    *type = (Type){.name="str", .size=1, .fields=NULL};
    TypeList = addNode(type, sizeof(Type), TypeList);
    *type = (Type){.name="bool", .size=1, .fields=NULL};
    TypeList = addNode(type, sizeof(Type), TypeList);     
    *type = (Type){.name="void", .size=0, .fields=NULL};
    TypeList = addNode(type, sizeof(Type), TypeList);
    *type = (Type){.name="null", .size=0, .fields=NULL};
    TypeList = addNode(type, sizeof(Type), TypeList);     
    yyin = fopen(argv[1], "r");
    if(yyin == NULL) yyerror("Input file not found");
	out = fopen("./out.xsm", "w");
    fprintf(out, "0\n2056\n0\n0\n0\n0\n0\n0\nMOV SP, 4300\nCALL main\n");
    fprintf(out, "MOV R19, \"Exit\"\nPUSH R19\nPUSH R19\nPUSH R19\nPUSH R19\nPUSH R19\nCALL 0\n");
    if(out == NULL) yyerror("file");
	yyparse();
	return 0;
}
