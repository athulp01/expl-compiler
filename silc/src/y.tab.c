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
            // printf("%s %s\n", root->vartype->name, root->varname);
            LSymbol* tmp = malloc(sizeof(LSymbol));
            *tmp = (LSymbol){.name = root->varname, .type = root->vartype};
            res->data = (void*)tmp;
            res->next = NULL;
            return res;
        }else {
            LinkedList *right = addParam(root->right);
            LinkedList *left = addParam(root->left);
            return connectList(right, left, sizeof(LSymbol));
            // if(right) {
            //     right->next = left;
            //     return right;
            // }
            // return left;
        }

    }

#line 216 "./src/y.tab.c"

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
#line 149 "./src/silc.y"

	tnode *no;
    char *name;
    char* type;
    LinkedList* list;
    Field* field;
	

#line 379 "./src/y.tab.c"

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
#define YYLAST   651

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  59
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  50
/* YYNRULES -- Number of rules.  */
#define YYNRULES  117
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  290

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
       0,   182,   182,   183,   184,   185,   186,   187,   188,   196,
     211,   212,   215,   260,   335,   341,   342,   344,   359,   376,
     379,   380,   382,   413,   439,   440,   443,   450,   454,   457,
     477,   479,   486,   489,   492,   495,   498,   501,   504,   507,
     510,   513,   516,   519,   522,   523,   524,   525,   526,   527,
     528,   529,   532,   536,   538,   540,   544,   547,   550,   552,
     556,   559,   561,   563,   567,   572,   578,   581,   584,   588,
     589,   590,   591,   592,   593,   594,   595,   596,   599,   600,
     604,   605,   608,   609,   611,   615,   616,   617,   618,   620,
     628,   635,   636,   639,   640,   644,   660,   661,   669,   670,
     673,   674,   678,   698,   703,   708,   714,   719,   724,   735,
     739,   744,   759,   776,   786,   787,   788,   791
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
  "classdefblock", "classdef", "inherit", "methodlst", "methoddef",
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

#define YYPACT_NINF (-155)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      43,    -3,   -17,    45,    37,    59,  -155,     9,  -155,    -7,
    -155,    53,  -155,  -155,    83,     2,  -155,    55,    -6,    89,
    -155,  -155,   107,  -155,     9,  -155,    37,  -155,   117,    53,
    -155,    73,   -34,  -155,  -155,   120,   127,   179,   130,   138,
    -155,    53,   131,  -155,  -155,   120,   141,  -155,   143,  -155,
      99,   144,  -155,   120,   120,   179,   142,  -155,    53,   120,
     147,   148,   152,  -155,   146,    98,   191,  -155,   160,     4,
    -155,    83,   103,  -155,   157,   120,   205,  -155,   156,  -155,
    -155,   120,  -155,   120,   159,  -155,  -155,  -155,  -155,    12,
     184,  -155,   163,   114,     5,   174,   170,  -155,   172,   168,
    -155,  -155,   120,   199,  -155,   180,     6,  -155,   309,    19,
    -155,  -155,   176,  -155,  -155,    44,  -155,  -155,   181,   182,
     186,   187,   183,   185,   192,   196,   203,   204,    62,  -155,
      64,  -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,
    -155,  -155,  -155,   197,   212,  -155,   309,   194,   206,  -155,
     209,    87,    87,  -155,  -155,   208,   210,   214,   215,   216,
      87,    87,    87,   217,    87,    87,    87,   218,    87,   221,
    -155,  -155,   197,   309,    20,  -155,  -155,  -155,    88,  -155,
      87,   129,   415,  -155,   438,  -155,  -155,   220,   222,    49,
      68,   461,   507,   583,  -155,   123,  -155,   331,   526,   125,
    -155,   545,   227,   242,   197,   309,   244,    87,   484,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,   268,   267,   243,  -155,   245,    87,   246,   247,  -155,
      87,  -155,   248,  -155,  -155,  -155,  -155,   234,   252,   197,
     309,   359,  -155,   602,   602,   602,   602,   602,   602,   602,
     -13,   -13,  -155,  -155,   150,   309,   309,  -155,  -155,   387,
    -155,  -155,  -155,    87,  -155,   240,   260,   197,  -155,   228,
     255,   249,   564,  -155,   254,   269,   309,   266,   270,   271,
    -155,  -155,   258,   282,  -155,  -155,  -155,  -155,   272,  -155
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     8,     0,    11,     0,
      25,     0,    99,    90,     0,     0,   101,     0,     0,     0,
       1,    10,     0,     5,     0,    24,    90,     6,     0,     0,
     110,   107,     0,    98,   100,     0,     0,     0,     0,     0,
       7,     0,     0,     4,   109,   116,     0,   102,     0,    28,
       0,     0,    14,     0,     0,     0,     0,     3,     0,   116,
       0,     0,   113,   115,     0,   103,     0,    27,     0,     0,
      21,     0,     0,    16,     0,     0,     0,     2,     0,   117,
     108,     0,   106,   116,     0,    29,    26,    19,    20,     0,
       0,    15,     0,     0,     0,     0,     0,   114,     0,     0,
      22,    12,   116,     0,    92,     0,     0,    94,     0,     0,
     105,   104,     0,    13,    97,     0,    91,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
       0,    69,    86,    85,    88,    77,    70,    71,    72,    74,
      75,    73,    79,     0,     0,    76,     0,     0,     0,    95,
       0,     0,     0,    66,    67,     0,     0,     0,     0,     0,
       0,     0,    81,     0,     0,     0,    81,     0,     0,     0,
      78,    89,     0,     0,     0,    96,    48,    51,    46,    45,
       0,    50,     0,    49,     0,    56,    57,     0,     0,     0,
       0,     0,     0,    80,    83,     0,    30,     0,     0,     0,
      31,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,    61,
      81,    84,     0,    63,    23,    52,     9,     0,     0,     0,
       0,     0,    44,    42,    39,    40,    38,    41,    36,    37,
      32,    33,    34,    35,    43,     0,     0,    59,    53,     0,
      55,    60,    82,     0,   112,     0,     0,     0,    47,     0,
       0,     0,     0,   111,     0,     0,     0,     0,     0,     0,
      62,    18,     0,     0,    65,    68,    54,    17,     0,    64
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -155,  -155,     8,   305,    34,  -155,   253,    -5,   263,  -155,
     250,  -155,  -155,   277,  -155,   311,  -108,  -136,  -154,  -155,
    -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,  -155,
    -141,  -133,    91,   171,  -104,  -155,   145,   -88,  -155,   223,
    -155,    80,  -155,   307,   265,   297,   -24,    32,  -155,   259
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,     6,     7,     8,    38,    72,    73,    54,    69,
      70,   129,     9,    49,    50,    10,   181,   193,   169,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   194,   195,   183,   145,    60,    95,   106,   107,
     115,    11,    15,    16,    32,    29,    30,    61,    62,    63
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     130,    12,   170,     2,   144,    44,    33,    36,    87,   104,
     116,    47,     1,   172,    48,   182,   184,    23,   203,    27,
       3,   147,    94,    94,   191,   192,   218,   219,   197,   198,
      40,   170,   201,    17,    44,   130,     3,    43,   130,   144,
     204,    21,   144,     4,   208,    19,     1,    13,    37,    57,
     238,   190,    13,     2,    13,    13,    13,   100,    21,    20,
      48,   146,   205,   170,   130,   130,    77,    91,   144,   144,
       3,   241,   239,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   266,   206,    22,    91,   149,
     259,    78,   150,     4,   176,    18,   130,   130,   170,   177,
     144,   144,   225,    26,    41,   163,   226,   267,   161,    35,
     165,   122,   123,   275,   162,    98,   166,   125,   163,   164,
     167,   227,   269,   270,   167,    45,   170,   272,   170,   170,
      46,   130,   130,    31,   112,   144,   144,   178,   179,   180,
     162,    39,   170,   283,   163,   207,    14,   130,   130,    13,
      83,   144,   144,    13,    66,    84,    28,     4,    90,   130,
      14,   130,   130,   144,    13,   144,   144,    42,   130,   103,
      13,   230,   144,   230,    28,   130,   231,    52,   234,   144,
      51,   166,    53,    59,    55,   167,    28,   216,   217,   218,
     219,    56,    64,    65,    68,    51,    76,    79,    71,    74,
      81,    80,    85,    28,    82,    86,   168,    92,    94,    96,
      99,   118,   119,   101,    71,   102,   108,    74,   120,   121,
      74,   122,   123,   124,   109,   110,   111,   125,   113,   148,
     114,   153,   154,   151,   152,   155,   173,   156,    74,   105,
     126,   127,   118,   119,   157,   276,   277,   128,   158,   120,
     121,   105,   122,   123,   124,   159,   160,   171,   125,   175,
     174,   185,   202,   186,   187,   188,   189,   196,   200,   118,
     119,   126,   127,   223,   278,   224,   120,   121,   128,   122,
     123,   124,   236,   237,   255,   125,   240,   256,   257,   264,
     258,   260,   261,   265,   263,   273,   118,   119,   126,   127,
     288,   274,   279,   120,   121,   128,   122,   123,   124,   281,
     282,   284,   125,   287,    24,   285,   286,   289,    75,    88,
      25,   262,    34,   118,   119,   126,   127,    67,    93,   117,
     120,   121,   128,   122,   123,   124,    89,   199,    58,   125,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   126,   127,   209,     0,     0,     0,     0,   128,
       0,     0,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,     0,     0,     0,     0,     0,     0,     0,     0,
     220,     0,   209,     0,     0,     0,     0,     0,     0,   232,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
       0,     0,     0,     0,     0,     0,     0,     0,   220,     0,
     209,     0,     0,     0,     0,     0,     0,   268,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,     0,     0,
       0,     0,     0,     0,     0,     0,   220,     0,   209,     0,
       0,     0,     0,     0,     0,   271,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,     0,     0,     0,     0,
       0,   209,     0,     0,   220,     0,     0,     0,   221,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,     0,
       0,     0,     0,     0,   209,     0,     0,   220,     0,     0,
       0,   222,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,     0,     0,     0,     0,     0,   209,     0,     0,
     220,     0,     0,     0,   228,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,     0,     0,     0,     0,     0,
     209,     0,     0,   220,     0,     0,     0,   242,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,     0,   209,
       0,     0,   229,     0,     0,     0,   220,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,     0,   209,     0,
       0,   233,     0,     0,     0,   220,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,     0,   209,     0,     0,
     235,     0,     0,     0,   220,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,     0,   209,     0,     0,   280,
       0,     0,     0,   220,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,     0,    -1,     0,     0,     0,     0,
       0,     0,   220,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,   218,   219,     0,     0,     0,     0,     0,     0,     0,
       0,   220
};

static const yytype_int16 yycheck[] =
{
     108,     4,   143,    10,   108,    29,     4,    13,     4,     4,
       4,    45,     3,   146,    48,   151,   152,     9,   172,    11,
      27,   109,     3,     3,   160,   161,    39,    40,   164,   165,
      22,   172,   168,    50,    58,   143,    27,    29,   146,   143,
     173,     7,   146,    50,   180,     8,     3,    50,    54,    41,
     204,   159,    50,    10,    50,    50,    50,    45,    24,     0,
      48,    42,    42,   204,   172,   173,    58,    72,   172,   173,
      27,   207,   205,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   239,   174,     7,    93,    45,
     226,    59,    48,    50,     7,    50,   204,   205,   239,    12,
     204,   205,    53,    50,    24,    56,    57,   240,    46,    54,
      46,    24,    25,   267,    52,    83,    52,    30,    56,    57,
      56,    53,   255,   256,    56,    52,   267,   263,   269,   270,
      57,   239,   240,    50,   102,   239,   240,    50,    51,    52,
      52,    52,   283,   276,    56,    57,     1,   255,   256,    50,
      52,   255,   256,    50,    55,    57,    11,    50,    55,   267,
      15,   269,   270,   267,    50,   269,   270,    50,   276,    55,
      50,    48,   276,    48,    29,   283,    53,    50,    53,   283,
      35,    52,     3,    52,    54,    56,    41,    37,    38,    39,
      40,    53,    51,    50,    50,    50,    54,    50,    53,    54,
      48,    53,    11,    58,    58,    45,     9,    50,     3,    53,
      51,    14,    15,    29,    69,    52,    42,    72,    21,    22,
      75,    24,    25,    26,    54,    53,    58,    30,    29,    53,
      50,    45,    45,    52,    52,    52,    42,    52,    93,    94,
      43,    44,    14,    15,    52,    17,    18,    50,    52,    21,
      22,   106,    24,    25,    26,    52,    52,    45,    30,    50,
      54,    53,    41,    53,    50,    50,    50,    50,    50,    14,
      15,    43,    44,    53,    19,    53,    21,    22,    50,    24,
      25,    26,    55,    41,    16,    30,    42,    20,    45,    55,
      45,    45,    45,    41,    46,    55,    14,    15,    43,    44,
      18,    41,    53,    21,    22,    50,    24,    25,    26,    55,
      41,    45,    30,    55,     9,    45,    45,    45,    55,    69,
       9,   230,    15,    14,    15,    43,    44,    50,    75,   106,
      21,    22,    50,    24,    25,    26,    71,   166,    41,    30,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    43,    44,    23,    -1,    -1,    -1,    -1,    50,
      -1,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      49,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    58,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    58,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    58,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    49,    -1,    -1,    -1,    53,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    49,    -1,    -1,
      -1,    53,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      49,    -1,    -1,    -1,    53,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    49,    -1,    -1,    -1,    53,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    -1,    23,
      -1,    -1,    45,    -1,    -1,    -1,    49,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    23,    -1,
      -1,    45,    -1,    -1,    -1,    49,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    -1,    23,    -1,    -1,
      45,    -1,    -1,    -1,    49,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    -1,    23,    -1,    -1,    45,
      -1,    -1,    -1,    49,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    10,    27,    50,    60,    61,    62,    63,    71,
      74,   100,     4,    50,    95,   101,   102,    50,    50,     8,
       0,    63,   100,    61,    62,    74,    50,    61,    95,   104,
     105,    50,   103,     4,   102,    54,    13,    54,    64,    52,
      61,   100,    50,    61,   105,    52,    57,    45,    48,    72,
      73,    95,    50,     3,    67,    54,    53,    61,   104,    52,
      95,   106,   107,   108,    51,    50,    55,    72,    50,    68,
      69,    95,    65,    66,    95,    67,    54,    61,   106,    50,
      53,    48,    58,    52,    57,    11,    45,     4,    69,   103,
      55,    66,    50,    65,     3,    96,    53,   108,   106,    51,
      45,    29,    52,    55,     4,    95,    97,    98,    42,    54,
      53,    58,   106,    29,    50,    99,     4,    98,    14,    15,
      21,    22,    24,    25,    26,    30,    43,    44,    50,    70,
      75,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    93,    94,    42,    96,    53,    45,
      48,    52,    52,    45,    45,    52,    52,    52,    52,    52,
      52,    46,    52,    56,    57,    46,    52,    56,     9,    77,
      89,    45,    90,    42,    54,    50,     7,    12,    50,    51,
      52,    75,    76,    93,    76,    53,    53,    50,    50,    50,
      75,    76,    76,    76,    91,    92,    50,    76,    76,    92,
      50,    76,    41,    77,    90,    42,    96,    57,    76,    23,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      49,    53,    53,    53,    53,    53,    57,    53,    53,    45,
      48,    53,    58,    45,    53,    45,    55,    41,    77,    90,
      42,    76,    53,    76,    76,    76,    76,    76,    76,    76,
      76,    76,    76,    76,    76,    16,    20,    45,    45,    76,
      45,    45,    91,    46,    55,    41,    77,    90,    58,    90,
      90,    58,    76,    55,    41,    77,    17,    18,    19,    53,
      45,    55,    41,    90,    45,    45,    45,    55,    18,    45
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    59,    60,    60,    60,    60,    60,    60,    60,    61,
      62,    62,    63,    63,    64,    65,    65,    66,    66,    67,
      68,    68,    69,    70,    71,    71,    72,    73,    73,    74,
      75,    75,    76,    76,    76,    76,    76,    76,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      76,    76,    77,    78,    78,    78,    79,    80,    81,    82,
      83,    84,    84,    84,    85,    85,    86,    87,    88,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    90,    90,
      91,    91,    92,    92,    93,    93,    93,    93,    93,    94,
      95,    96,    96,    97,    97,    98,    99,    99,   100,   100,
     101,   101,   102,   103,   103,   103,   103,   103,   103,   104,
     104,   105,   105,   106,   107,   107,   107,   108
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     5,     4,     3,     2,     2,     3,     1,    11,
       2,     1,     7,     8,     2,     2,     1,    12,    11,     3,
       2,     1,     3,     4,     2,     1,     3,     2,     1,     6,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     1,     4,     1,     1,
       1,     1,     3,     5,     8,     5,     3,     3,     4,     5,
       5,     4,     7,     4,    10,     8,     2,     2,     8,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     1,
       1,     0,     3,     1,     4,     1,     1,     1,     1,     2,
       1,     3,     2,     2,     1,     3,     3,     1,     3,     2,
       2,     1,     3,     3,     6,     6,     4,     1,     4,     2,
       1,    12,    11,     1,     3,     1,     0,     2
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
#line 196 "./src/silc.y"
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
#line 1942 "./src/y.tab.c"
    break;

  case 12:
#line 215 "./src/silc.y"
                                                                          { 
                                                                 if(searchClass((yyvsp[-5].name), ClassList)) yyerror("Duplicate class");
                                                                ClassDef *class = (ClassDef*)malloc(sizeof(ClassDef));
                                                                 class->name = (yyvsp[-5].name);
                                                                 curClassName = (yyvsp[-5].name);
                                                                 class->idx = classno++;
                                                                 class->fields = curClassField;
                                                                 int begin = 1;
                                                                 LinkedList *f = class->fields;
                                                                 while(f) {
                                                                     ((Field*)f->data)->idx = begin++;
                                                                     if(((Field*)f->data)->class) begin++;
                                                                     f = f->next;
                                                                 }
                                                                 if(begin>8) yyerror("Maximum field size exceeded");
                                                                 class->methods = curClassMethod;
                                                                 LinkedList *mets = class->methods;
                                                                 int nummethods = 0;
                                                                 while(mets) {
                                                                     nummethods++;
                                                                    ((Method*)mets->data)->class = class;
                                                                    mets = mets->next;
                                                                 }
                                                                 if(nummethods>8) yyerror("Maximum number of methods exceeded");
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
#line 1992 "./src/y.tab.c"
    break;

  case 13:
#line 260 "./src/silc.y"
                                                                                  { 
                                                                 if(searchClass((yyvsp[-6].name), ClassList)) yyerror("Duplicate class");
                                                                 ClassDef *parent = searchClass((yyvsp[-5].name), ClassList);
                                                                 if(!parent) yyerror("Parent class is not defines");
                                                                 ClassDef *class = (ClassDef*)malloc(sizeof(ClassDef));
                                                                 class->name = (yyvsp[-6].name);
                                                                 curClassName = (yyvsp[-6].name);
                                                                 class->idx = classno++;
                                                                 class->fields = curClassField;
                                                                 int begin = 1;
                                                                 LinkedList *f = class->fields;
                                                                 while(f) {
                                                                     ((Field*)f->data)->idx = begin++;
                                                                     if(((Field*)f->data)->class) begin++;
                                                                     f = f->next;
                                                                 }
                                                                 if(begin>8) yyerror("Maximum field size exceeded");
                                                                 class->parent = parent;
                                                                 int nummethods = 0;
                                                                 LinkedList *mtds = curClassMethod;
                                                                 while(mtds) {
                                                                    nummethods++;
                                                                    ((Method*)mtds->data)->class = class;
                                                                     char s[100];
                                                                     strcpy(s, (yyvsp[-6].name));
                                                                     strcat(s, ".");
                                                                     strcat(s, ((Method*)mtds->data)->name);
                                                                     char t[50];
                                                                    sprintf(t, "MOV R19, %s\nMOV [%d], R19\n",s, class->idx*8+4096+((Method*)mtds->data)->idx);
                                                                     strcat(virtual, t);
                                                                    mtds = mtds->next;
                                                                  }
                                                                  int numm = nummethods - 1;
                                                                 mtds = copyList(parent->methods, sizeof(Method));
                                                                 LinkedList* parentm = mtds;
                                                                 while(mtds) {
                                                                    Method* child = searchMethod(((Method*)mtds->data)->name, curClassMethod);
                                                                    if(child) {
                                                                        ((Method*)mtds->data)->idx += child->idx;
                                                                        checkArg(child->params, ((Method*)mtds->data)->params);
                                                                        mtds = mtds->next;
                                                                        continue;
                                                                    }
                                                                    ((Method*)mtds->data)->idx += nummethods-1;
                                                                    numm++;
                                                                     char s[100];
                                                                     strcpy(s, ((Method*)mtds->data)->class->name);
                                                                     strcat(s, ".");
                                                                     strcat(s, ((Method*)mtds->data)->name);
                                                                     char t[50];
                                                                    sprintf(t, "MOV R19, %s\nMOV [%d], R19\n",s, class->idx*8+4096+((Method*)mtds->data)->idx);
                                                                    strcat(virtual, t);
                                                                    mtds = mtds->next;
                                                                  }
                                                                  if(numm>8) yyerror("Maximum method size exceeded");
                                                                 class->methods = connectList(curClassMethod, parentm, sizeof(Method));
                                                                 class->parent = parent;
                                                                 LinkedList *func = (yyvsp[-2].list);
                                                                 ClassList = addNode(class, sizeof(ClassDef), ClassList);
                                                                 tnode *tmp;
                                                                 while(func) {
                                                                     tmp = (tnode*)func->data;
                                                                     char *s = malloc(sizeof((yyvsp[-6].name))+sizeof(tmp->varname)+2);
                                                                     strcpy(s, (yyvsp[-6].name));
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
#line 2072 "./src/y.tab.c"
    break;

  case 14:
#line 335 "./src/silc.y"
                         {
                              curClassField = searchClass((yyvsp[0].name), ClassList)->fields;
                              (yyval.name) = (yyvsp[0].name);
                          }
#line 2081 "./src/y.tab.c"
    break;

  case 15:
#line 341 "./src/silc.y"
                                                                {(yyval.list) = connectList((yyvsp[-1].list), (yyvsp[0].list), sizeof(tnode));}
#line 2087 "./src/y.tab.c"
    break;

  case 16:
#line 342 "./src/silc.y"
                                                                {(yyval.list) = (yyvsp[0].list);}
#line 2093 "./src/y.tab.c"
    break;

  case 17:
#line 344 "./src/silc.y"
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
                                                                    curLvar = NULL;
                                                                    }
#line 2113 "./src/y.tab.c"
    break;

  case 18:
#line 359 "./src/silc.y"
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
                                                                    curLvar = NULL;
                                                                    }
#line 2132 "./src/y.tab.c"
    break;

  case 19:
#line 376 "./src/silc.y"
                                                               {(yyval.list) = (yyvsp[-1].list);}
#line 2138 "./src/y.tab.c"
    break;

  case 20:
#line 379 "./src/silc.y"
                                                                {(yyval.list) = connectList((yyvsp[-1].list), (yyvsp[0].list), sizeof(Field));}
#line 2144 "./src/y.tab.c"
    break;

  case 21:
#line 380 "./src/silc.y"
                                                                    {(yyval.list) = (yyvsp[0].list);}
#line 2150 "./src/y.tab.c"
    break;

  case 22:
#line 382 "./src/silc.y"
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
#line 2184 "./src/y.tab.c"
    break;

  case 23:
#line 413 "./src/silc.y"
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
#line 2212 "./src/y.tab.c"
    break;

  case 24:
#line 439 "./src/silc.y"
                                                          {}
#line 2218 "./src/y.tab.c"
    break;

  case 25:
#line 440 "./src/silc.y"
                                                          {}
#line 2224 "./src/y.tab.c"
    break;

  case 26:
#line 443 "./src/silc.y"
                                                          { Field *field = (Field*)malloc(sizeof(Field));
                                                            Type *type = searchType((yyvsp[-2].name), TypeList);
                                                            *field = type?(Field){.name=(yyvsp[-1].name), .type=type}:(Field){.name=(yyvsp[-1].name), .ndef=(yyvsp[-2].name)};
                                                            (yyval.field) = field;
                                                          }
#line 2234 "./src/y.tab.c"
    break;

  case 27:
#line 450 "./src/silc.y"
                                                           { (yyvsp[0].field)->idx = ((Field*)(yyvsp[-1].list)->data)->idx + 1;
                                                             if(getField((yyvsp[0].field)->name, (yyvsp[-1].list)))
                                                                yyerror("Duplicate field entry");
                                                             (yyval.list) = addNode((yyvsp[0].field), sizeof(Field), (yyvsp[-1].list));}
#line 2243 "./src/y.tab.c"
    break;

  case 28:
#line 454 "./src/silc.y"
                                                           { (yyvsp[0].field)->idx = 1; (yyval.list) = addNode((yyvsp[0].field), sizeof(Field), NULL); }
#line 2249 "./src/y.tab.c"
    break;

  case 29:
#line 457 "./src/silc.y"
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
#line 2272 "./src/y.tab.c"
    break;

  case 30:
#line 477 "./src/silc.y"
                                                            {char *tmp = (char*)malloc(strlen((yyvsp[-2].name)) + strlen((yyvsp[0].name)) + 1);
                                                            strcpy(tmp, (yyvsp[-2].name)); strcat(tmp, ".");strcat(tmp, (yyvsp[0].name)); (yyval.name) = tmp;}
#line 2279 "./src/y.tab.c"
    break;

  case 31:
#line 479 "./src/silc.y"
                                                            {char *tmp = (char*)malloc(strlen((yyvsp[-2].name)) + strlen((yyvsp[0].name)) + 1);
                                                            strcpy(tmp, (yyvsp[-2].name)); strcat(tmp, ".");strcat(tmp, (yyvsp[0].name)); (yyval.name) = tmp;}
#line 2286 "./src/y.tab.c"
    break;

  case 32:
#line 486 "./src/silc.y"
                                                            {(yyval.no) = createNode(OP, "+", -1, (yyvsp[-2].no), (yyvsp[0].no)); 
                                                        (yyval.no)->vartype = searchType("int", TypeList);}
#line 2293 "./src/y.tab.c"
    break;

  case 33:
#line 489 "./src/silc.y"
                                                            {(yyval.no) = createNode(OP, "-", -1, (yyvsp[-2].no), (yyvsp[0].no)); 
                                                        (yyval.no)->vartype = searchType("int", TypeList);}
#line 2300 "./src/y.tab.c"
    break;

  case 34:
#line 492 "./src/silc.y"
                                                                {(yyval.no) = createNode(OP, "*", -1, (yyvsp[-2].no), (yyvsp[0].no)); 
                                                        (yyval.no)->vartype = searchType("int", TypeList);}
#line 2307 "./src/y.tab.c"
    break;

  case 35:
#line 495 "./src/silc.y"
                                                                {(yyval.no) = createNode(OP, "/", -1, (yyvsp[-2].no), (yyvsp[0].no)); 
                                                        (yyval.no)->vartype = searchType("int", TypeList);}
#line 2314 "./src/y.tab.c"
    break;

  case 36:
#line 498 "./src/silc.y"
                                                        {(yyval.no) = createNode(OP, "<=", -1, (yyvsp[-2].no), (yyvsp[0].no)); 
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2321 "./src/y.tab.c"
    break;

  case 37:
#line 501 "./src/silc.y"
                                                        {(yyval.no) = createNode(OP, ">=", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2328 "./src/y.tab.c"
    break;

  case 38:
#line 504 "./src/silc.y"
                                                        {(yyval.no) = createNode(OP, "==", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2335 "./src/y.tab.c"
    break;

  case 39:
#line 507 "./src/silc.y"
                                                        {(yyval.no) = createNode(OP, "<", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2342 "./src/y.tab.c"
    break;

  case 40:
#line 510 "./src/silc.y"
                                                        {(yyval.no) = createNode(OP, ">", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2349 "./src/y.tab.c"
    break;

  case 41:
#line 513 "./src/silc.y"
                                                        {(yyval.no) = createNode(OP, "!=", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2356 "./src/y.tab.c"
    break;

  case 42:
#line 516 "./src/silc.y"
                                                        {(yyval.no) = createNode(OP, "AND", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2363 "./src/y.tab.c"
    break;

  case 43:
#line 519 "./src/silc.y"
                                                        {(yyval.no) = createNode(OP, "%", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("int", TypeList);}
#line 2370 "./src/y.tab.c"
    break;

  case 44:
#line 522 "./src/silc.y"
                                                                {(yyval.no) = (yyvsp[-1].no);}
#line 2376 "./src/y.tab.c"
    break;

  case 45:
#line 523 "./src/silc.y"
                                                                    {(yyval.no) = (yyvsp[0].no); (yyval.no)->vartype = searchType("int", TypeList);}
#line 2382 "./src/y.tab.c"
    break;

  case 46:
#line 524 "./src/silc.y"
                                                        {(yyval.no) = createNode(VAR, (yyvsp[0].name), -1, NULL, NULL); (yyval.no)->vartype = getTypeOfSymbol((yyvsp[0].name)); (yyval.no)->varclass = getClassOfSymbol((yyvsp[0].name));}
#line 2388 "./src/y.tab.c"
    break;

  case 47:
#line 525 "./src/silc.y"
                                                        {(yyval.no) = createNode(VAR, (yyvsp[-3].name), -1, (yyvsp[-1].no), NULL);(yyval.no)->vartype = getTypeOfSymbol((yyvsp[-3].name));}
#line 2394 "./src/y.tab.c"
    break;

  case 48:
#line 526 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no); (yyval.no)->vartype = searchType("str", TypeList);}
#line 2400 "./src/y.tab.c"
    break;

  case 49:
#line 527 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2406 "./src/y.tab.c"
    break;

  case 50:
#line 528 "./src/silc.y"
                                                        {(yyval.no) = createNode(VAR, (yyvsp[0].name), -1, NULL, NULL);(yyval.no)->vartype = getTypeOfSymbol((yyvsp[0].name)); (yyval.no)->varclass = getClassOfSymbol((yyvsp[0].name));}
#line 2412 "./src/y.tab.c"
    break;

  case 51:
#line 529 "./src/silc.y"
                                                        {(yyval.no) = createNode(CONST, "\0", 0, NULL, NULL); (yyval.no)->vartype = searchType("null", TypeList);}
#line 2418 "./src/y.tab.c"
    break;

  case 52:
#line 532 "./src/silc.y"
                                                        {(yyval.no) = createNode(RET, "\0", -1, (yyvsp[-1].no), NULL);}
#line 2424 "./src/y.tab.c"
    break;

  case 53:
#line 536 "./src/silc.y"
                                                        {tnode *tmp = createNode(VAR, (yyvsp[-2].name), -1, NULL, NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-2].name));
                                                        (yyval.no) = createNode(READ, "", -1, tmp, NULL);}
#line 2431 "./src/y.tab.c"
    break;

  case 54:
#line 538 "./src/silc.y"
                                                        {tnode *tmp = createNode(VAR, (yyvsp[-5].name), -1, (yyvsp[-3].no), NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-5].name));
                                                        (yyval.no) = createNode(READ, "", -1, tmp, NULL);}
#line 2438 "./src/y.tab.c"
    break;

  case 55:
#line 540 "./src/silc.y"
                                                        {tnode *tmp = createNode(VAR, (yyvsp[-2].name), -1, NULL, NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-2].name));
                                                        (yyval.no) = createNode(READ, "", -1, tmp, NULL);}
#line 2445 "./src/y.tab.c"
    break;

  case 56:
#line 544 "./src/silc.y"
                                                          {(yyval.no) = createNode(INIT, "", -1, NULL, NULL);(yyval.no)->vartype = searchType("int", TypeList);}
#line 2451 "./src/y.tab.c"
    break;

  case 57:
#line 547 "./src/silc.y"
                                                       {(yyval.no) = createNode(ALLOC, "", -1, NULL, NULL); (yyval.no)->vartype = searchType("int", TypeList);}
#line 2457 "./src/y.tab.c"
    break;

  case 58:
#line 550 "./src/silc.y"
                                                        {(yyval.no) = createNode(NEW, (yyvsp[-1].name), -1, NULL, NULL); (yyval.no)->vartype = searchType("int", TypeList);}
#line 2463 "./src/y.tab.c"
    break;

  case 59:
#line 552 "./src/silc.y"
                                                        {tnode *tmp = createNode(VAR, (yyvsp[-2].name), -1, NULL, NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-2].name));tmp->varclass = getClassOfSymbol((yyvsp[-2].name));
                                                        (yyval.no) = createNode(FFREE, "", -1, tmp, NULL);}
#line 2470 "./src/y.tab.c"
    break;

  case 60:
#line 556 "./src/silc.y"
                                                        {(yyval.no) = createNode(WRITE, "", -1, (yyvsp[-2].no), NULL);}
#line 2476 "./src/y.tab.c"
    break;

  case 61:
#line 559 "./src/silc.y"
                                                        { tnode *tmp = createNode(VAR, (yyvsp[-3].name), -1, NULL, NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-3].name)); tmp->varclass = getClassOfSymbol((yyvsp[-3].name));
                                                         (yyval.no) = createNode(ASSN, "", -1, tmp, (yyvsp[-1].no));}
#line 2483 "./src/y.tab.c"
    break;

  case 62:
#line 561 "./src/silc.y"
                                                        {tnode *tmp = createNode(VAR, (yyvsp[-6].name), -1, (yyvsp[-4].no), NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-6].name)); tmp->varclass = getClassOfSymbol((yyvsp[-6].name));
                                                         (yyval.no) = createNode(ASSN, "", -1, tmp, (yyvsp[-1].no));}
#line 2490 "./src/y.tab.c"
    break;

  case 63:
#line 563 "./src/silc.y"
                                                        { tnode *tmp = createNode(VAR, (yyvsp[-3].name), -1, NULL, NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-3].name)); tmp->varclass = getClassOfSymbol((yyvsp[-3].name));
                                                        (yyval.no) = createNode(ASSN, "", -1, tmp, (yyvsp[-1].no));}
#line 2497 "./src/y.tab.c"
    break;

  case 64:
#line 568 "./src/silc.y"
                                                        {
                                                            tnode *tmp = createNode(IF_BODY, "", -1, (yyvsp[-4].no), (yyvsp[-2].no));
                                                            (yyval.no) = createNode(IF, "", -1, (yyvsp[-7].no), tmp);
                                                        }
#line 2506 "./src/y.tab.c"
    break;

  case 65:
#line 572 "./src/silc.y"
                                                        {
                                                            tnode *tmp = createNode(IF_BODY, "", -1, (yyvsp[-2].no), NULL);
                                                            (yyval.no) = createNode(IF, "", -1, (yyvsp[-5].no), tmp);
                                                        }
#line 2515 "./src/y.tab.c"
    break;

  case 66:
#line 578 "./src/silc.y"
                                                        {(yyval.no) = createNode(BREAK, "", -1, NULL, NULL);}
#line 2521 "./src/y.tab.c"
    break;

  case 67:
#line 581 "./src/silc.y"
                                                        {(yyval.no) = createNode(CONT, "", -1, NULL, NULL);}
#line 2527 "./src/y.tab.c"
    break;

  case 68:
#line 585 "./src/silc.y"
                                                        {(yyval.no) = createNode(WHILE, "", -1, (yyvsp[-5].no), (yyvsp[-2].no));}
#line 2533 "./src/y.tab.c"
    break;

  case 69:
#line 588 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2539 "./src/y.tab.c"
    break;

  case 70:
#line 589 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2545 "./src/y.tab.c"
    break;

  case 71:
#line 590 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2551 "./src/y.tab.c"
    break;

  case 72:
#line 591 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2557 "./src/y.tab.c"
    break;

  case 73:
#line 592 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2563 "./src/y.tab.c"
    break;

  case 74:
#line 593 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2569 "./src/y.tab.c"
    break;

  case 75:
#line 594 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2575 "./src/y.tab.c"
    break;

  case 76:
#line 595 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2581 "./src/y.tab.c"
    break;

  case 77:
#line 596 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2587 "./src/y.tab.c"
    break;

  case 78:
#line 599 "./src/silc.y"
                                                        {(yyval.no) = connect((yyvsp[-1].no), (yyvsp[0].no));}
#line 2593 "./src/y.tab.c"
    break;

  case 79:
#line 600 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2599 "./src/y.tab.c"
    break;

  case 80:
#line 604 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2605 "./src/y.tab.c"
    break;

  case 81:
#line 605 "./src/silc.y"
                                                        {(yyval.no) = NULL;}
#line 2611 "./src/y.tab.c"
    break;

  case 82:
#line 608 "./src/silc.y"
                                                        {(yyval.no) = connect((yyvsp[0].no), (yyvsp[-2].no));}
#line 2617 "./src/y.tab.c"
    break;

  case 83:
#line 609 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2623 "./src/y.tab.c"
    break;

  case 84:
#line 611 "./src/silc.y"
                                                 {  GSymbol* sym = (GSymbol*)searchSymbol((yyvsp[-3].name), GSymList);
                                                    if(sym== NULL) yyerror("Function is not declared");
                                                    (yyval.no) = createNode(FUNC, (yyvsp[-3].name), -1, (yyvsp[-1].no), NULL);
                                                    (yyval.no)->vartype = sym->type;}
#line 2632 "./src/y.tab.c"
    break;

  case 85:
#line 615 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2638 "./src/y.tab.c"
    break;

  case 86:
#line 616 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2644 "./src/y.tab.c"
    break;

  case 87:
#line 617 "./src/silc.y"
                                                         {(yyval.no) = (yyvsp[0].no);}
#line 2650 "./src/y.tab.c"
    break;

  case 88:
#line 618 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2656 "./src/y.tab.c"
    break;

  case 89:
#line 620 "./src/silc.y"
                                                        { (yyval.no) = (yyvsp[-1].no);}
#line 2662 "./src/y.tab.c"
    break;

  case 90:
#line 628 "./src/silc.y"
                                                       {(yyval.name) = (yyvsp[0].name);}
#line 2668 "./src/y.tab.c"
    break;

  case 91:
#line 635 "./src/silc.y"
                                                        { (yyval.list) = (yyvsp[-1].list);}
#line 2674 "./src/y.tab.c"
    break;

  case 92:
#line 636 "./src/silc.y"
                                                        { (yyval.list) = NULL;}
#line 2680 "./src/y.tab.c"
    break;

  case 93:
#line 639 "./src/silc.y"
                                                        { (yyval.list) = connectList((yyvsp[-1].list), (yyvsp[0].list), sizeof(LSymbol));}
#line 2686 "./src/y.tab.c"
    break;

  case 94:
#line 640 "./src/silc.y"
                                                        {(yyval.list) = (yyvsp[0].list);}
#line 2692 "./src/y.tab.c"
    break;

  case 95:
#line 644 "./src/silc.y"
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
#line 2712 "./src/y.tab.c"
    break;

  case 96:
#line 660 "./src/silc.y"
                                                        {(yyval.list) = addNode(strdup((yyvsp[0].name)), sizeof((yyvsp[0].name)), (yyvsp[-2].list));}
#line 2718 "./src/y.tab.c"
    break;

  case 97:
#line 661 "./src/silc.y"
                                                        {(yyval.list) = addNode(strdup((yyvsp[0].name)), sizeof((yyvsp[0].name)), NULL);}
#line 2724 "./src/y.tab.c"
    break;

  case 98:
#line 669 "./src/silc.y"
                                                        {}
#line 2730 "./src/y.tab.c"
    break;

  case 99:
#line 670 "./src/silc.y"
                                                        {}
#line 2736 "./src/y.tab.c"
    break;

  case 100:
#line 673 "./src/silc.y"
                                                        {}
#line 2742 "./src/y.tab.c"
    break;

  case 101:
#line 674 "./src/silc.y"
                                                        {}
#line 2748 "./src/y.tab.c"
    break;

  case 102:
#line 678 "./src/silc.y"
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
#line 2772 "./src/y.tab.c"
    break;

  case 103:
#line 698 "./src/silc.y"
                                                        {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[0].name), .size=1};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), (yyvsp[-2].list));
                                                        }
#line 2781 "./src/y.tab.c"
    break;

  case 104:
#line 703 "./src/silc.y"
                                                        {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[-3].name), .size=(yyvsp[-1].no)->val};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), (yyvsp[-5].list));
                                                        }
#line 2790 "./src/y.tab.c"
    break;

  case 105:
#line 708 "./src/silc.y"
                                                        {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[-3].name), .params=(yyvsp[-1].no), .size=0};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), (yyvsp[-5].list));
                                                         curLvar = NULL;
                                                        }
#line 2800 "./src/y.tab.c"
    break;

  case 106:
#line 714 "./src/silc.y"
                                                        {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[-3].name), .size=(yyvsp[-1].no)->val};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), NULL);
                                                        }
#line 2809 "./src/y.tab.c"
    break;

  case 107:
#line 719 "./src/silc.y"
                                                        {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[0].name), .size=1};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), NULL);
                                                        }
#line 2818 "./src/y.tab.c"
    break;

  case 108:
#line 724 "./src/silc.y"
                                                        {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[-3].name), .params=(yyvsp[-1].no), .size=0, .isfunc=1};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), NULL);
                                                         curLvar = NULL;
                                                        }
#line 2828 "./src/y.tab.c"
    break;

  case 109:
#line 735 "./src/silc.y"
                                                        {
                                                         fprintf(out, "%s:\n", (yyvsp[0].no)->varname);
                                                         eval_func((yyvsp[0].no), out);}
#line 2836 "./src/y.tab.c"
    break;

  case 110:
#line 739 "./src/silc.y"
                                                        {
                                                         fprintf(out, "%s:\n", (yyvsp[0].no)->varname);
                                                         eval_func((yyvsp[0].no), out);}
#line 2844 "./src/y.tab.c"
    break;

  case 111:
#line 744 "./src/silc.y"
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
#line 2864 "./src/y.tab.c"
    break;

  case 112:
#line 759 "./src/silc.y"
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
#line 2884 "./src/y.tab.c"
    break;

  case 113:
#line 776 "./src/silc.y"
                                                        {curLvar = addParam((yyvsp[0].no)); (yyval.no) = (yyvsp[0].no); 
                                                        LinkedList* t = curLvar;
                                                        LSymbol* tmp = malloc(sizeof(LSymbol));
                                                        ClassDef *type = malloc(sizeof(ClassDef));;
                                                        *type = (ClassDef){.fields=curClassField, .methods=curClassMethod};
                                                        *tmp = (LSymbol){.name = "self", .class=type };
                                                        curLvar = addNode(tmp, sizeof(LSymbol), curLvar);
                                                        t = curLvar;
                                                        }
#line 2898 "./src/y.tab.c"
    break;

  case 114:
#line 786 "./src/silc.y"
                                                        { (yyval.no) = connect((yyvsp[0].no), (yyvsp[-2].no));}
#line 2904 "./src/y.tab.c"
    break;

  case 115:
#line 787 "./src/silc.y"
                                                        { (yyval.no) = (yyvsp[0].no);}
#line 2910 "./src/y.tab.c"
    break;

  case 116:
#line 788 "./src/silc.y"
                                                        { (yyval.no) = NULL;}
#line 2916 "./src/y.tab.c"
    break;

  case 117:
#line 791 "./src/silc.y"
                                                        {(yyval.no) = createNode(VAR, (yyvsp[0].name), -1, NULL, NULL); (yyval.no)->vartype = searchType((yyvsp[-1].name), TypeList);}
#line 2922 "./src/y.tab.c"
    break;


#line 2926 "./src/y.tab.c"

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
#line 796 "./src/silc.y"





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
