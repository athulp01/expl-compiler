/* A Bison parser, made by GNU Bison 3.4.2.  */

/* Bison implementation for Yacc-like parsers in C

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
#define YYBISON_VERSION "3.4.2"

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
    int yyerror(char*);
    FILE *out, *yyin;
    LinkedList *GSymList, *LSymList, *LVarList, *GVarList;
    int curMemory;

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

#line 97 "./src/y.tab.c"

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
	

#line 228 "./src/y.tab.c"

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

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
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
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  13
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   438

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  47
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  77
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  182

#define YYUNDEFTOK  2
#define YYMAXUTOK   295

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      41,    42,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    45,     2,    46,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    43,     2,    44,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    61,    61,    62,    63,    71,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   106,   110,   111,   115,   118,   120,
     124,   129,   135,   138,   141,   145,   146,   147,   148,   149,
     150,   151,   152,   155,   156,   160,   161,   164,   165,   167,
     172,   180,   181,   189,   190,   193,   194,   198,   210,   211,
     219,   220,   223,   224,   228,   243,   248,   253,   258,   263,
     268,   278,   282,   287,   306,   307,   308,   311
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "_DECL", "_ENDDECL", "_INT", "_STR",
  "_TEXT", "_MAIN", "_RET", "_IF", "_WHILE", "_THEN", "_ELSE", "_ENDIF",
  "_ENDWHILE", "_DO", "_BREAK", "_CONT", "_AND", "_LT", "_GT", "_EQ",
  "_NE", "_LE", "_GE", "_PLUS", "_MINUS", "_MUL", "_DIV", "_END", "_BEGIN",
  "_READ", "_WRITE", "_SEMI", "_EQUALS", "_Q", "_COMMA", "_MOD", "_ID",
  "_NUM", "'('", "')'", "'{'", "'}'", "'['", "']'", "$accept", "program",
  "mainblock", "expr", "return", "read", "write", "assgn", "ifstmt",
  "break", "cont", "whilestmt", "stmt", "stmtList", "arg", "args",
  "funccall", "funcstmt", "type", "ldeclblock", "ldecllist", "ldecl",
  "lvarlist", "gdeclblock", "gdecllist", "gdecl", "gvarlist", "fdefblock",
  "fdef", "paramlist", "param", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,    40,    41,   123,   125,    91,    93
};
# endif

#define YYPACT_NINF -97

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-97)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-1)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     128,    27,     5,    56,   -97,    -1,   -97,   -97,   -97,    16,
     142,   -97,    25,   -97,     5,   -97,    40,    -1,   -97,   -19,
      57,   -97,   -97,    43,    49,   -97,   -97,    34,    46,   -97,
      68,    52,    34,    75,   -31,   -97,    76,   -16,   123,    22,
     -97,    34,   -97,   -97,    34,   100,   149,   111,   107,   -97,
      26,    97,   -97,   110,   166,   -97,   119,   123,   -97,   -97,
     -97,    98,   -97,   -97,   115,   116,   139,   141,   133,   135,
     -11,   -97,   -97,   -97,   -97,   -97,   -97,   -97,   -97,    60,
     143,   -97,   148,   -97,   144,    -4,    -4,   -97,   -97,   145,
      -4,    -4,    -4,    -4,    -4,   150,   -97,   -97,   119,   -97,
     -97,    35,   -97,    -4,   224,   -97,   248,    99,   272,   320,
     380,   -97,    30,   140,   340,   137,    60,    -4,   296,    -4,
      -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,
      -4,   170,   169,   164,    -4,   165,   -97,    -4,   -97,   167,
     -97,   -97,   171,   168,   -97,   400,   400,   400,   400,   400,
     400,   400,    45,    45,   -97,   -97,    55,   119,   119,   -97,
     196,   -97,   -97,    -4,   156,   -97,    95,   106,   161,   360,
     -97,   119,   173,   174,   175,   -97,    86,   -97,   -97,   -97,
     176,   -97
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     4,     0,    61,    51,    52,     0,
       0,    63,     0,     1,    51,     3,     0,     0,    72,    69,
       0,    60,    62,     0,     0,     2,    71,    76,     0,    64,
       0,     0,    76,     0,     0,    75,     0,    65,     0,     0,
      77,     0,    70,    68,    76,     0,     0,     0,     0,    74,
       0,     0,    54,     0,     0,    56,     0,     0,    67,    66,
      59,     0,    53,    55,     0,     0,     0,     0,     0,     0,
       0,    35,    36,    37,    38,    40,    41,    39,    44,     0,
       0,    42,     0,    57,     0,     0,     0,    32,    33,     0,
       0,     0,    46,     0,     0,     0,    43,    50,     0,    58,
      22,    20,    19,     0,     0,    23,     0,     0,     0,     0,
      45,    48,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    28,    46,    49,     0,
      24,     5,     0,     0,    18,    16,    13,    14,    12,    15,
      10,    11,     6,     7,     8,     9,    17,     0,     0,    25,
       0,    27,    47,     0,     0,    21,     0,     0,     0,     0,
      73,     0,     0,     0,     0,    29,     0,    31,    34,    26,
       0,    30
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -97,   -97,     2,   -76,    88,   -97,   -97,   -97,   -97,   -97,
     -97,   -97,   -78,   -96,    74,   -97,   -56,   -97,    11,   155,
     -97,   151,   -97,   -97,   -97,   203,   -97,   -97,   209,   -24,
     186
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     3,     4,   110,    95,    71,    72,    73,    74,    75,
      76,    77,    78,    79,   111,   112,   105,    81,    33,    47,
      54,    55,    61,     5,    10,    11,    20,    17,    18,    34,
      35
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      80,    96,   116,   100,    14,     8,    41,    15,    39,   104,
     106,    42,     9,    12,   108,   109,    16,   113,   114,    25,
      50,     9,    27,    80,    91,    44,    28,   118,    16,    45,
      92,     6,     7,     8,    93,   101,   102,   103,    96,     7,
       8,   143,    80,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,    19,    13,    53,   160,    41,
      80,   166,   167,    41,    48,    53,    23,   137,    58,    94,
      64,    65,   138,   128,   129,   176,    92,    66,    67,    24,
     117,   126,   127,   128,   129,    31,    36,   169,    96,    96,
      32,    29,    68,    69,    30,    38,    64,    65,    96,    70,
     180,    80,    80,    66,    67,    64,    65,    37,   171,   172,
      80,    80,    66,    67,    40,    80,    64,    65,    68,    69,
      80,   173,    43,    66,    67,    70,    46,    68,    69,    64,
      65,     1,    83,     2,    70,    84,    66,    67,    68,    69,
      51,   133,    56,    59,   134,    70,    21,     7,     8,    60,
      57,    68,    69,    52,     7,     8,    85,    86,    70,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
      62,     7,     8,    87,    89,    88,    90,    97,   130,    98,
     115,   141,   157,    99,   107,   158,   139,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   159,   161,
     170,   164,   163,   174,   142,    63,   130,   177,   178,   179,
     181,   162,    82,    22,   165,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,    26,    49,     0,     0,
       0,     0,     0,     0,   130,     0,     0,     0,     0,     0,
       0,     0,   168,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,     0,     0,     0,     0,     0,     0,
       0,     0,   130,     0,     0,     0,   131,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,     0,     0,
       0,     0,     0,     0,     0,     0,   130,     0,     0,     0,
     132,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,     0,     0,     0,     0,     0,     0,     0,     0,
     130,     0,     0,     0,   135,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,     0,     0,     0,     0,
       0,     0,     0,     0,   130,     0,     0,     0,   144,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
       0,     0,     0,     0,   136,     0,     0,     0,   130,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
       0,     0,     0,     0,   140,     0,     0,     0,   130,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
       0,     0,     0,     0,   175,     0,     0,     0,   130,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
       0,     0,     0,     0,     0,     0,     0,     0,   130,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
       0,     0,     0,     0,     0,     0,     0,     0,   130
};

static const yytype_int16 yycheck[] =
{
      56,    79,    98,     7,     5,     6,    37,     5,    32,    85,
      86,    42,     1,     8,    90,    91,     5,    93,    94,    17,
      44,    10,    41,    79,    35,    41,    45,   103,    17,    45,
      41,     4,     5,     6,    45,    39,    40,    41,   116,     5,
       6,   117,    98,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,    39,     0,    46,   134,    37,
     116,   157,   158,    37,    42,    54,    41,    37,    42,     9,
      10,    11,    42,    28,    29,   171,    41,    17,    18,    39,
      45,    26,    27,    28,    29,    42,    40,   163,   166,   167,
      41,    34,    32,    33,    37,    43,    10,    11,   176,    39,
      14,   157,   158,    17,    18,    10,    11,    39,    13,    14,
     166,   167,    17,    18,    39,   171,    10,    11,    32,    33,
     176,    15,    46,    17,    18,    39,     3,    32,    33,    10,
      11,     3,    34,     5,    39,    37,    17,    18,    32,    33,
      40,    42,    31,    46,    45,    39,     4,     5,     6,    39,
      43,    32,    33,     4,     5,     6,    41,    41,    39,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
       4,     5,     6,    34,    41,    34,    41,    34,    38,    31,
      30,    44,    12,    39,    39,    16,    46,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    34,    34,
      44,    30,    35,    42,   116,    54,    38,    34,    34,    34,
      34,   137,    57,    10,    46,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    17,    41,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    46,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    42,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,
      42,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    -1,    -1,    -1,    42,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    42,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    38,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    38,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    38,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     5,    48,    49,    70,     4,     5,     6,    65,
      71,    72,     8,     0,     5,    49,    65,    74,    75,    39,
      73,     4,    72,    41,    39,    49,    75,    41,    45,    34,
      37,    42,    41,    65,    76,    77,    40,    39,    43,    76,
      39,    37,    42,    46,    41,    45,     3,    66,    42,    77,
      76,    40,     4,    65,    67,    68,    31,    43,    42,    46,
      39,    69,     4,    68,    10,    11,    17,    18,    32,    33,
      39,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      63,    64,    66,    34,    37,    41,    41,    34,    34,    41,
      41,    35,    41,    45,     9,    51,    59,    34,    31,    39,
       7,    39,    40,    41,    50,    63,    50,    39,    50,    50,
      50,    61,    62,    50,    50,    30,    60,    45,    50,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      38,    42,    42,    42,    45,    42,    34,    37,    42,    46,
      34,    44,    51,    50,    42,    50,    50,    50,    50,    50,
      50,    50,    50,    50,    50,    50,    50,    12,    16,    34,
      50,    34,    61,    35,    30,    46,    60,    60,    46,    50,
      44,    13,    14,    15,    42,    34,    60,    34,    34,    34,
      14,    34
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    47,    48,    48,    48,    49,    50,    50,    50,    50,
      50,    50,    50,    50,    50,    50,    50,    50,    50,    50,
      50,    50,    50,    50,    51,    52,    52,    53,    54,    54,
      55,    55,    56,    57,    58,    59,    59,    59,    59,    59,
      59,    59,    59,    60,    60,    61,    61,    62,    62,    63,
      64,    65,    65,    66,    66,    67,    67,    68,    69,    69,
      70,    70,    71,    71,    72,    73,    73,    73,    73,    73,
      73,    74,    74,    75,    76,    76,    76,    77
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     2,     1,    11,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     4,     1,     1,     3,     5,     8,     5,     4,     7,
      10,     8,     2,     2,     8,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     1,     0,     3,     1,     4,
       2,     1,     1,     3,     2,     2,     1,     3,     3,     1,
       3,     2,     2,     1,     3,     3,     6,     6,     4,     1,
       4,     2,     1,    12,     3,     1,     0,     2
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
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
{
  unsigned long yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
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
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
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
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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

  if (! yyres)
    return yystrlen (yystr);

  return (YYSIZE_T) (yystpcpy (yyres, yystr) - yyres);
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
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

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
      int yyn = yypact[*yyssp];
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
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
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
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
          yyp++;
          yyformat++;
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
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

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

    YYSIZE_T yystacksize;

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
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
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
| yynewstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  *yyssp = (yytype_int16) yystate;

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = (YYSIZE_T) (yyssp - yyss + 1);

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
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
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
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

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long) yystacksize));

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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
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
  case 5:
#line 71 "./src/silc.y"
    {tnode *node = createNode(FUNC, "main", -1, connect((yyvsp[-3].no),(yyvsp[-2].no)), NULL); (yyval.no)->vartype = 0;
                                                             GSymbol* tmp = malloc(sizeof(GSymbol));
                                                             Frame *frame = (Frame*)malloc(sizeof(Frame));
                                                             frame->Lvars = (yyvsp[-5].list);
                                                             *tmp = (GSymbol){.name="main", .params=NULL, .frame=frame};
                                                             GSymList = addNode(tmp, sizeof(GSymbol), GSymList);
                                                             fprintf(out ,"main:\nBRKP\n");
                                                             eval_func(node, out);
                                                            }
#line 1614 "./src/y.tab.c"
    break;

  case 6:
#line 86 "./src/silc.y"
    {(yyval.no) = createNode(OP, "+", -1, (yyvsp[-2].no), (yyvsp[0].no)); (yyval.no)->vartype = INT;}
#line 1620 "./src/y.tab.c"
    break;

  case 7:
#line 87 "./src/silc.y"
    {(yyval.no) = createNode(OP, "-", -1, (yyvsp[-2].no), (yyvsp[0].no)); (yyval.no)->vartype = INT;}
#line 1626 "./src/y.tab.c"
    break;

  case 8:
#line 88 "./src/silc.y"
    {(yyval.no) = createNode(OP, "*", -1, (yyvsp[-2].no), (yyvsp[0].no)); (yyval.no)->vartype = INT;}
#line 1632 "./src/y.tab.c"
    break;

  case 9:
#line 89 "./src/silc.y"
    {(yyval.no) = createNode(OP, "/", -1, (yyvsp[-2].no), (yyvsp[0].no)); (yyval.no)->vartype = INT;}
#line 1638 "./src/y.tab.c"
    break;

  case 10:
#line 90 "./src/silc.y"
    {(yyval.no) = createNode(OP, "<=", -1, (yyvsp[-2].no), (yyvsp[0].no)); (yyval.no)->vartype = BOOL;}
#line 1644 "./src/y.tab.c"
    break;

  case 11:
#line 91 "./src/silc.y"
    {(yyval.no) = createNode(OP, ">=", -1, (yyvsp[-2].no), (yyvsp[0].no));(yyval.no)->vartype = BOOL;}
#line 1650 "./src/y.tab.c"
    break;

  case 12:
#line 92 "./src/silc.y"
    {(yyval.no) = createNode(OP, "==", -1, (yyvsp[-2].no), (yyvsp[0].no));(yyval.no)->vartype = BOOL;}
#line 1656 "./src/y.tab.c"
    break;

  case 13:
#line 93 "./src/silc.y"
    {(yyval.no) = createNode(OP, "<", -1, (yyvsp[-2].no), (yyvsp[0].no));(yyval.no)->vartype = BOOL;}
#line 1662 "./src/y.tab.c"
    break;

  case 14:
#line 94 "./src/silc.y"
    {(yyval.no) = createNode(OP, ">", -1, (yyvsp[-2].no), (yyvsp[0].no));(yyval.no)->vartype = BOOL;}
#line 1668 "./src/y.tab.c"
    break;

  case 15:
#line 95 "./src/silc.y"
    {(yyval.no) = createNode(OP, "!=", -1, (yyvsp[-2].no), (yyvsp[0].no));(yyval.no)->vartype = BOOL;}
#line 1674 "./src/y.tab.c"
    break;

  case 16:
#line 96 "./src/silc.y"
    {(yyval.no) = createNode(OP, "AND", -1, (yyvsp[-2].no), (yyvsp[0].no));(yyval.no)->vartype = BOOL;}
#line 1680 "./src/y.tab.c"
    break;

  case 17:
#line 97 "./src/silc.y"
    {(yyval.no) = createNode(OP, "%", -1, (yyvsp[-2].no), (yyvsp[0].no));(yyval.no)->vartype = INT;}
#line 1686 "./src/y.tab.c"
    break;

  case 18:
#line 98 "./src/silc.y"
    {(yyval.no) = (yyvsp[-1].no);}
#line 1692 "./src/y.tab.c"
    break;

  case 19:
#line 99 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no); (yyval.no)->vartype = INT;}
#line 1698 "./src/y.tab.c"
    break;

  case 20:
#line 100 "./src/silc.y"
    {(yyval.no) = createNode(VAR, (yyvsp[0].name), -1, NULL, NULL);}
#line 1704 "./src/y.tab.c"
    break;

  case 21:
#line 101 "./src/silc.y"
    {(yyval.no) = createNode(VAR, (yyvsp[-3].name), -1, (yyvsp[-1].no), NULL);}
#line 1710 "./src/y.tab.c"
    break;

  case 22:
#line 102 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no); (yyval.no)->vartype = STRING;}
#line 1716 "./src/y.tab.c"
    break;

  case 23:
#line 103 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 1722 "./src/y.tab.c"
    break;

  case 24:
#line 106 "./src/silc.y"
    {(yyval.no) = createNode(RET, "\0", -1, (yyvsp[-1].no), NULL);}
#line 1728 "./src/y.tab.c"
    break;

  case 25:
#line 110 "./src/silc.y"
    {(yyval.no) = createNode(READ, "", -1, createNode(VAR, (yyvsp[-2].name), -1, NULL, NULL), NULL);}
#line 1734 "./src/y.tab.c"
    break;

  case 26:
#line 111 "./src/silc.y"
    {(yyval.no) = createNode(READ, "", -1, createNode(VAR, (yyvsp[-5].name), -1, (yyvsp[-3].no), NULL), NULL);}
#line 1740 "./src/y.tab.c"
    break;

  case 27:
#line 115 "./src/silc.y"
    {(yyval.no) = createNode(WRITE, "", -1, (yyvsp[-2].no), NULL);}
#line 1746 "./src/y.tab.c"
    break;

  case 28:
#line 118 "./src/silc.y"
    { 
                                                         (yyval.no) = createNode(ASSN, "", -1, createNode(VAR, (yyvsp[-3].name), -1, NULL, NULL), (yyvsp[-1].no));}
#line 1753 "./src/y.tab.c"
    break;

  case 29:
#line 120 "./src/silc.y"
    {
                                                         (yyval.no) = createNode(ASSN, "", -1, createNode(VAR, (yyvsp[-6].name), -1, (yyvsp[-4].no), NULL), (yyvsp[-1].no));}
#line 1760 "./src/y.tab.c"
    break;

  case 30:
#line 125 "./src/silc.y"
    {
                                                            tnode *tmp = createNode(IF_BODY, "", -1, (yyvsp[-4].no), (yyvsp[-2].no));
                                                            (yyval.no) = createNode(IF, "", -1, (yyvsp[-7].no), tmp);
                                                        }
#line 1769 "./src/y.tab.c"
    break;

  case 31:
#line 129 "./src/silc.y"
    {
                                                            tnode *tmp = createNode(IF_BODY, "", -1, (yyvsp[-2].no), NULL);
                                                            (yyval.no) = createNode(IF, "", -1, (yyvsp[-5].no), tmp);
                                                        }
#line 1778 "./src/y.tab.c"
    break;

  case 32:
#line 135 "./src/silc.y"
    {(yyval.no) = createNode(BREAK, "", -1, NULL, NULL);}
#line 1784 "./src/y.tab.c"
    break;

  case 33:
#line 138 "./src/silc.y"
    {(yyval.no) = createNode(CONT, "", -1, NULL, NULL);}
#line 1790 "./src/y.tab.c"
    break;

  case 34:
#line 142 "./src/silc.y"
    {(yyval.no) = createNode(WHILE, "", -1, (yyvsp[-5].no), (yyvsp[-2].no));}
#line 1796 "./src/y.tab.c"
    break;

  case 35:
#line 145 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 1802 "./src/y.tab.c"
    break;

  case 36:
#line 146 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 1808 "./src/y.tab.c"
    break;

  case 37:
#line 147 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 1814 "./src/y.tab.c"
    break;

  case 38:
#line 148 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 1820 "./src/y.tab.c"
    break;

  case 39:
#line 149 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 1826 "./src/y.tab.c"
    break;

  case 40:
#line 150 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 1832 "./src/y.tab.c"
    break;

  case 41:
#line 151 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 1838 "./src/y.tab.c"
    break;

  case 42:
#line 152 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 1844 "./src/y.tab.c"
    break;

  case 43:
#line 155 "./src/silc.y"
    {(yyval.no) = connect((yyvsp[-1].no), (yyvsp[0].no));}
#line 1850 "./src/y.tab.c"
    break;

  case 44:
#line 156 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 1856 "./src/y.tab.c"
    break;

  case 45:
#line 160 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 1862 "./src/y.tab.c"
    break;

  case 46:
#line 161 "./src/silc.y"
    {(yyval.no) = NULL;}
#line 1868 "./src/y.tab.c"
    break;

  case 47:
#line 164 "./src/silc.y"
    {(yyval.no) = connect((yyvsp[0].no), (yyvsp[-2].no));}
#line 1874 "./src/y.tab.c"
    break;

  case 48:
#line 165 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 1880 "./src/y.tab.c"
    break;

  case 49:
#line 167 "./src/silc.y"
    {  GSymbol* sym = (GSymbol*)searchSymbol((yyvsp[-3].name), GSymList);
                                                    if(sym== NULL) yyerror("Function is not declared");
                                                    checkArg(sym->params, (yyvsp[-1].no));
                                                    (yyval.no) = createNode(FUNC, (yyvsp[-3].name), -1, (yyvsp[-1].no), NULL);}
#line 1889 "./src/y.tab.c"
    break;

  case 50:
#line 172 "./src/silc.y"
    { (yyval.no) = (yyvsp[-1].no);}
#line 1895 "./src/y.tab.c"
    break;

  case 51:
#line 180 "./src/silc.y"
    {(yyval.type) = 0;}
#line 1901 "./src/y.tab.c"
    break;

  case 52:
#line 181 "./src/silc.y"
    {(yyval.type) = 2;}
#line 1907 "./src/y.tab.c"
    break;

  case 53:
#line 189 "./src/silc.y"
    { (yyval.list) = (yyvsp[-1].list);}
#line 1913 "./src/y.tab.c"
    break;

  case 54:
#line 190 "./src/silc.y"
    { (yyval.list) = NULL;}
#line 1919 "./src/y.tab.c"
    break;

  case 55:
#line 193 "./src/silc.y"
    { (yyval.list) = connectList((yyvsp[-1].list), (yyvsp[0].list), sizeof(LSymbol));}
#line 1925 "./src/y.tab.c"
    break;

  case 56:
#line 194 "./src/silc.y"
    {(yyval.list) = (yyvsp[0].list);}
#line 1931 "./src/y.tab.c"
    break;

  case 57:
#line 198 "./src/silc.y"
    {
                                                            LinkedList* gvars = (yyvsp[-1].list);
                                                            (yyval.list) = NULL;
                                                            while(gvars) {
                                                                char *var = (char*)(gvars->data);
                                                                LSymbol* tmp = (LSymbol*)malloc(sizeof(LSymbol));
                                                                *tmp = (LSymbol){.name=var, .type=(yyvsp[-2].type), .size=1};
                                                                (yyval.list) = addNode(tmp, sizeof(LSymbol), (yyval.list));
                                                                gvars = gvars->next;
                                                            }
                                                        }
#line 1947 "./src/y.tab.c"
    break;

  case 58:
#line 210 "./src/silc.y"
    {(yyval.list) = addNode(strdup((yyvsp[0].name)), sizeof((yyvsp[0].name)), (yyvsp[-2].list));}
#line 1953 "./src/y.tab.c"
    break;

  case 59:
#line 211 "./src/silc.y"
    {(yyval.list) = addNode(strdup((yyvsp[0].name)), sizeof((yyvsp[0].name)), NULL);}
#line 1959 "./src/y.tab.c"
    break;

  case 60:
#line 219 "./src/silc.y"
    { }
#line 1965 "./src/y.tab.c"
    break;

  case 61:
#line 220 "./src/silc.y"
    { }
#line 1971 "./src/y.tab.c"
    break;

  case 62:
#line 223 "./src/silc.y"
    {}
#line 1977 "./src/y.tab.c"
    break;

  case 63:
#line 224 "./src/silc.y"
    {}
#line 1983 "./src/y.tab.c"
    break;

  case 64:
#line 228 "./src/silc.y"
    {
                                                            LinkedList* gvars = (yyvsp[-1].list);
                                                            while(gvars) {
                                                                GVariable *var = (GVariable*)(gvars->data);
                                                                if(searchSymbol(var->name, GSymList)) {
                                                                    yyerror("Variable is already declared");
                                                                }
                                                                GSymbol* tmp = (GSymbol*)malloc(sizeof(GSymbol));
                                                                *tmp = (GSymbol){.name=var->name, .type=(yyvsp[-2].type), .size=var->size, .params=var->params, .binding=curMemory};
                                                                GSymList = addNode(tmp, sizeof(GSymbol), GSymList);
                                                                gvars = gvars->next;
                                                                curMemory+=var->size;
                                                            }
                                                        }
#line 2002 "./src/y.tab.c"
    break;

  case 65:
#line 243 "./src/silc.y"
    {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[0].name), .size=1};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), (yyvsp[-2].list));
                                                        }
#line 2011 "./src/y.tab.c"
    break;

  case 66:
#line 248 "./src/silc.y"
    {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[-3].name), .size=(yyvsp[-1].no)->val};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), (yyvsp[-5].list));
                                                        }
#line 2020 "./src/y.tab.c"
    break;

  case 67:
#line 253 "./src/silc.y"
    {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[-3].name), .params=(yyvsp[-1].no), .size=0};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), (yyvsp[-5].list));
                                                        }
#line 2029 "./src/y.tab.c"
    break;

  case 68:
#line 258 "./src/silc.y"
    {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[-3].name), .size=(yyvsp[-1].no)->val};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), NULL);
                                                        }
#line 2038 "./src/y.tab.c"
    break;

  case 69:
#line 263 "./src/silc.y"
    {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[0].name), .size=1};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), NULL);
                                                        }
#line 2047 "./src/y.tab.c"
    break;

  case 70:
#line 268 "./src/silc.y"
    {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[-3].name), .params=(yyvsp[-1].no), .size=0};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), NULL);
                                                        }
#line 2056 "./src/y.tab.c"
    break;

  case 71:
#line 278 "./src/silc.y"
    {
                                                         fprintf(out, "%s:\n", (yyvsp[0].no)->varname);
                                                         eval_func((yyvsp[0].no), out);}
#line 2064 "./src/y.tab.c"
    break;

  case 72:
#line 282 "./src/silc.y"
    {
                                                         fprintf(out, "%s:\n", (yyvsp[0].no)->varname);
                                                         eval_func((yyvsp[0].no), out);}
#line 2072 "./src/y.tab.c"
    break;

  case 73:
#line 287 "./src/silc.y"
    {
                                                                    if((yyvsp[-2].no)->left->type == VAR) {  
                                                                        LSymbol *sym = (LSymbol*)searchSymbol((yyvsp[-2].no)->left->varname, (yyvsp[-5].list));
                                                                        if(sym && (yyvsp[-11].type) != sym->type) yyerror("Return type is not correct");
                                                                        if(!sym) {
                                                                            LSymbol *sym = (LSymbol*)searchSymbol((yyvsp[-2].no)->left->varname,GSymList);
                                                                            if(sym && (yyvsp[-11].type) != sym->type) yyerror("Return type is not correct");
                                                                        }
                                                                    } else if((yyvsp[-11].type) != (yyvsp[-2].no)->left->vartype) yyerror("Return type is not correct");
                                                                    (yyval.no) = createNode(FUNC, (yyvsp[-10].name), -1, connect((yyvsp[-3].no), (yyvsp[-2].no)), (yyvsp[-8].no)); (yyval.no)->vartype = (yyvsp[-11].type);
                                                                    GSymbol* tmp = searchSymbol((yyvsp[-10].name), GSymList);
                                                                    if(tmp == NULL) yyerror("Function is not declared");
                                                                    checkArg(tmp->params, (yyvsp[-8].no));
                                                                    Frame *frame = (Frame*)malloc(sizeof(Frame));
                                                                    frame->Lvars = (yyvsp[-5].list);
                                                                    tmp->frame = frame;
                                                                    }
#line 2094 "./src/y.tab.c"
    break;

  case 74:
#line 306 "./src/silc.y"
    { (yyval.no) = connect((yyvsp[0].no), (yyvsp[-2].no));}
#line 2100 "./src/y.tab.c"
    break;

  case 75:
#line 307 "./src/silc.y"
    { (yyval.no) = (yyvsp[0].no);}
#line 2106 "./src/y.tab.c"
    break;

  case 76:
#line 308 "./src/silc.y"
    {(yyval.no) = NULL;}
#line 2112 "./src/y.tab.c"
    break;

  case 77:
#line 311 "./src/silc.y"
    {(yyval.no) = createNode(VAR, (yyvsp[0].name), -1, NULL, NULL); (yyval.no)->vartype = (yyvsp[-1].type);}
#line 2118 "./src/y.tab.c"
    break;


#line 2122 "./src/y.tab.c"

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
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
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
                  yystos[*yyssp], yyvsp, yylsp);
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
#line 316 "./src/silc.y"





int main(int argc, char **argv) {
    if(argc < 2) {
        printf("Usage: ./silc <source_file>\n");
        exit(1);
    }
    curMemory = 4096;
    GSymList = NULL;
    LSymList = NULL;
    LVarList = NULL;
    GVarList = NULL;
    yyin = fopen(argv[1], "r");
    if(yyin == NULL) yyerror("Input file not found");
	out = fopen("./out.xsm", "w");
    fprintf(out, "0\n2056\n0\n0\n0\n0\n0\n0\nMOV SP, 4300\nCALL main\n");
    fprintf(out, "MOV R19, \"Exit\"\nPUSH R19\nPUSH R19\nPUSH R19\nPUSH R19\nPUSH R19\nCALL 0\n");
    if(out == NULL) yyerror("file");
	yyparse();
	return 0;
}
