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
    LinkedList *GSymList, *LSymList, *LVarList, *GVarList, *TypeList, *curLvar;
    Frame *curFrame;
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

    Type* getSymbol(char *oname) {
        char *sname = strdup(oname);
        char *name = strtok(sname, ".");
        GSymbol* sym = searchSymbol(name, curLvar);
        if (sym) {  // local variable
            Field* field = NULL;
            char *tok;
            while((tok = strtok(NULL, "."))) {
                field = getField(tok, sym->type->fields);
                if(!field) yyerror("Undefined field");
            }
            return field?field->type:sym->type;            
        } else {
            sym = searchSymbol(name, GSymList);
            if (sym) {  // global variable
                Field* field = NULL;
                char *tok;
                while((tok = strtok(NULL, "."))) {
                    field = getField(tok, sym->type->fields);
                    if(!field) yyerror("Undefined field");
                }
                return field?field->type:sym->type; 
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

#line 151 "./src/y.tab.c"

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
    _LT = 280,
    _GT = 281,
    _EQ = 282,
    _NE = 283,
    _LE = 284,
    _GE = 285,
    _PLUS = 286,
    _MINUS = 287,
    _MUL = 288,
    _DIV = 289,
    _END = 290,
    _BEGIN = 291,
    _READ = 292,
    _WRITE = 293,
    _SEMI = 294,
    _EQUALS = 295,
    _Q = 296,
    _COMMA = 297,
    _MOD = 298,
    _ID = 299,
    _NUM = 300
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
#define _LT 280
#define _GT 281
#define _EQ 282
#define _NE 283
#define _LE 284
#define _GE 285
#define _PLUS 286
#define _MINUS 287
#define _MUL 288
#define _DIV 289
#define _END 290
#define _BEGIN 291
#define _READ 292
#define _WRITE 293
#define _SEMI 294
#define _EQUALS 295
#define _Q 296
#define _COMMA 297
#define _MOD 298
#define _ID 299
#define _NUM 300

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
	

#line 293 "./src/y.tab.c"

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
#define YYFINAL  16
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   528

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  53
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  39
/* YYNRULES -- Number of rules.  */
#define YYNRULES  96
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  222

#define YYUNDEFTOK  2
#define YYMAXUTOK   300

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
      46,    47,     2,     2,     2,     2,    50,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    51,     2,    52,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    48,     2,    49,     2,     2,     2,     2,
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
      45
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   117,   117,   118,   119,   120,   121,   122,   130,   145,
     146,   149,   156,   158,   161,   180,   182,   189,   192,   195,
     198,   201,   204,   207,   210,   213,   216,   219,   222,   225,
     226,   227,   228,   229,   230,   231,   232,   235,   239,   241,
     243,   247,   250,   253,   256,   258,   260,   264,   269,   275,
     278,   281,   285,   286,   287,   288,   289,   290,   291,   292,
     295,   296,   300,   301,   304,   305,   307,   312,   313,   315,
     323,   330,   331,   334,   335,   339,   354,   355,   363,   364,
     367,   368,   372,   389,   394,   399,   404,   409,   414,   424,
     428,   433,   451,   453,   454,   455,   458
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "_DECL", "_ENDDECL", "_INT", "_STR",
  "_TEXT", "_MAIN", "_RET", "_TYPE", "_ENDTYPE", "_NULL", "_IF", "_WHILE",
  "_THEN", "_ELSE", "_ENDIF", "_ENDWHILE", "_DO", "_BREAK", "_CONT",
  "_AND", "_INIT", "_ALLOC", "_LT", "_GT", "_EQ", "_NE", "_LE", "_GE",
  "_PLUS", "_MINUS", "_MUL", "_DIV", "_END", "_BEGIN", "_READ", "_WRITE",
  "_SEMI", "_EQUALS", "_Q", "_COMMA", "_MOD", "_ID", "_NUM", "'('", "')'",
  "'{'", "'}'", "'.'", "'['", "']'", "$accept", "program", "mainblock",
  "typedefblock", "fielddef", "fieldlst", "typedef", "field", "expr",
  "return", "read", "init", "alloc", "write", "assgn", "ifstmt", "break",
  "cont", "whilestmt", "stmt", "stmtList", "arg", "args", "funccall",
  "funcstmt", "type", "ldeclblock", "ldecllist", "ldecl", "lvarlist",
  "gdeclblock", "gdecllist", "gdecl", "gvarlist", "fdefblock", "fdef",
  "paramlist", "params", "param", YY_NULLPTR
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
     295,   296,   297,   298,   299,   300,    40,    41,   123,   125,
      46,    91,    93
};
# endif

#define YYPACT_NINF -125

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-125)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-1)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       1,     2,   -19,    35,    68,  -125,     1,  -125,    50,  -125,
    -125,    56,     3,  -125,   -13,    29,  -125,  -125,  -125,    50,
      35,  -125,    64,    50,  -125,   -33,    54,  -125,  -125,    66,
      39,  -125,    50,    67,  -125,  -125,    66,    40,  -125,    75,
    -125,    32,    76,    77,  -125,    66,    82,    90,    88,  -125,
      92,    31,   131,  -125,   106,   145,   109,  -125,  -125,    66,
    -125,    66,   108,  -125,  -125,    25,   121,   112,  -125,   118,
     114,  -125,   123,    27,  -125,   194,   145,  -125,  -125,  -125,
      59,  -125,  -125,   122,   125,   130,   133,   141,   142,   144,
     148,    -2,   -26,  -125,  -125,  -125,  -125,  -125,  -125,  -125,
    -125,  -125,  -125,   126,   140,  -125,   155,  -125,   156,    83,
      83,  -125,  -125,   152,   154,   158,    83,    83,    83,   159,
      83,    83,   161,    83,   171,  -125,  -125,   194,  -125,  -125,
    -125,    -9,  -125,    83,   127,   298,  -125,   321,  -125,  -125,
      41,    62,   344,   390,   466,  -125,    42,  -125,   214,   409,
    -125,   428,   162,   126,    83,   367,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,   197,   200,
     174,    83,   181,   182,  -125,    83,  -125,   183,  -125,  -125,
    -125,   187,   242,  -125,   485,   485,   485,   485,   485,   485,
     485,     0,     0,  -125,  -125,    84,   194,   194,  -125,   270,
    -125,  -125,  -125,    83,   175,  -125,   138,   160,   178,   447,
    -125,   194,   188,   189,   190,  -125,   172,  -125,  -125,  -125,
     191,  -125
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     7,     0,    10,     0,    79,
      70,     0,     0,    81,     0,     0,     1,     5,     9,     0,
      70,     6,     0,     0,    90,    87,     0,    78,    80,     0,
       0,     3,     0,     0,     4,    89,    95,     0,    82,     0,
      13,     0,     0,     0,     2,    95,     0,     0,    92,    94,
       0,    83,     0,    12,     0,     0,     0,    96,    88,     0,
      86,    95,     0,    14,    11,     0,     0,     0,    93,     0,
       0,    72,     0,     0,    74,     0,     0,    85,    84,    77,
       0,    71,    73,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,    68,    67,    53,    54,    55,    57,
      58,    56,    61,     0,     0,    59,     0,    75,     0,     0,
       0,    49,    50,     0,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,    60,    69,     0,    76,    33,
      36,    31,    30,     0,    35,     0,    34,     0,    41,    42,
       0,     0,     0,     0,    62,    65,     0,    15,     0,     0,
      16,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    44,    63,    66,     0,    46,    37,
       8,     0,     0,    29,    27,    24,    25,    23,    26,    21,
      22,    17,    18,    19,    20,    28,     0,     0,    38,     0,
      40,    43,    64,     0,     0,    32,     0,     0,     0,     0,
      91,     0,     0,     0,     0,    45,     0,    48,    51,    39,
       0,    47
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -125,  -125,     4,  -125,   185,  -125,   227,   -75,  -101,    81,
    -125,  -125,  -125,  -125,  -125,  -125,  -125,  -125,  -125,  -102,
    -124,    60,  -125,   -73,  -125,    38,   173,  -125,   164,  -125,
     244,  -125,   239,  -125,   233,    -6,   -40,  -125,   195
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     4,     5,     6,    40,    41,     7,   134,   144,   124,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   145,   146,   136,   105,    22,    66,    73,    74,    80,
       8,    12,    13,    26,    23,    24,    47,    48,    49
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      92,   125,   104,   153,     1,    56,     9,    27,   135,   137,
      17,     2,    21,    36,   121,   142,   143,    35,    37,   148,
     149,    69,   151,    31,   122,    14,    35,    34,    92,    71,
     104,    81,   155,   165,   166,    29,    44,   118,   117,    11,
     141,   119,   154,    15,   118,     3,    10,    10,   119,   120,
      11,   125,    92,   182,   104,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,    42,    16,    10,
     199,    10,   206,   207,    46,    30,    10,    61,    92,    42,
     104,    52,    62,    46,   175,    50,    43,   216,   170,   176,
     129,   119,   171,    38,    20,   130,    39,    46,   107,    46,
      25,   108,   209,    72,   125,   125,    87,    88,    33,   172,
      10,    72,   122,    45,   125,   163,   164,   165,   166,    51,
      54,    92,    92,   104,   104,    55,    57,   131,   132,   133,
      59,    92,    92,   104,   104,   123,    92,    58,   104,    83,
      84,    92,    63,   104,    60,    64,    85,    86,    65,    87,
      88,    83,    84,    70,   211,   212,    67,    75,    85,    86,
      76,    87,    88,    89,    90,    77,    78,    79,   109,   111,
      91,   110,   112,    83,    84,    89,    90,   122,   213,   126,
      85,    86,    91,    87,    88,    83,    84,   113,   114,   220,
     115,   127,    85,    86,   116,    87,    88,    89,    90,   138,
     128,   139,   140,   147,    91,   150,   152,    83,    84,    89,
      90,   180,   196,   198,    85,    86,    91,    87,    88,   197,
     200,   201,   204,   203,   210,   214,    53,   217,   218,   219,
     221,    89,    90,    18,   181,   202,   156,    82,    91,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   106,
      19,    28,    32,     0,    68,     0,     0,   167,     0,     0,
       0,     0,     0,     0,   156,     0,   177,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,     0,     0,     0,
       0,     0,     0,     0,     0,   167,     0,     0,     0,     0,
       0,     0,   156,     0,   205,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,     0,     0,     0,     0,     0,
       0,     0,     0,   167,     0,     0,     0,     0,     0,     0,
     156,     0,   208,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,     0,     0,     0,     0,     0,     0,     0,
       0,   167,     0,   156,     0,   168,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,     0,     0,     0,     0,
       0,     0,     0,     0,   167,     0,   156,     0,   169,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,     0,
       0,     0,     0,     0,     0,     0,     0,   167,     0,   156,
       0,   173,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,     0,     0,     0,     0,     0,     0,     0,     0,
     167,     0,   156,     0,   183,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,     0,     0,     0,     0,   174,
       0,   156,     0,   167,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,     0,     0,     0,     0,   178,     0,
     156,     0,   167,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,     0,     0,     0,     0,   179,     0,   156,
       0,   167,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,     0,     0,     0,     0,   215,     0,   156,     0,
     167,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,     0,     0,     0,     0,     0,     0,    -1,     0,   167,
      -1,    -1,    -1,    -1,    -1,    -1,   163,   164,   165,   166,
       0,     0,     0,     0,     0,     0,     0,     0,   167
};

static const yytype_int16 yycheck[] =
{
      75,   103,    75,   127,     3,    45,     4,     4,   109,   110,
       6,    10,     8,    46,    40,   116,   117,    23,    51,   120,
     121,    61,   123,    19,    50,    44,    32,    23,   103,     4,
     103,     4,   133,    33,    34,    48,    32,    46,    40,     1,
     115,    50,    51,     8,    46,    44,    44,    44,    50,    51,
      12,   153,   127,   154,   127,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,    29,     0,    44,
     171,    44,   196,   197,    36,    46,    44,    46,   153,    41,
     153,    49,    51,    45,    42,    45,    47,   211,    47,    47,
       7,    50,    51,    39,    44,    12,    42,    59,    39,    61,
      44,    42,   203,    65,   206,   207,    23,    24,    44,    47,
      44,    73,    50,    46,   216,    31,    32,    33,    34,    44,
      44,   196,   197,   196,   197,    48,    44,    44,    45,    46,
      42,   206,   207,   206,   207,     9,   211,    47,   211,    13,
      14,   216,    11,   216,    52,    39,    20,    21,     3,    23,
      24,    13,    14,    45,    16,    17,    47,    36,    20,    21,
      48,    23,    24,    37,    38,    47,    52,    44,    46,    39,
      44,    46,    39,    13,    14,    37,    38,    50,    18,    39,
      20,    21,    44,    23,    24,    13,    14,    46,    46,    17,
      46,    36,    20,    21,    46,    23,    24,    37,    38,    47,
      44,    47,    44,    44,    44,    44,    35,    13,    14,    37,
      38,    49,    15,    39,    20,    21,    44,    23,    24,    19,
      39,    39,    35,    40,    49,    47,    41,    39,    39,    39,
      39,    37,    38,     6,   153,   175,    22,    73,    44,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    76,
       6,    12,    19,    -1,    59,    -1,    -1,    43,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    52,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    52,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    52,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    43,    -1,    22,    -1,    47,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    43,    -1,    22,    -1,    47,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,    -1,    22,
      -1,    47,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      43,    -1,    22,    -1,    47,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,    39,
      -1,    22,    -1,    43,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    -1,    -1,    -1,    -1,    39,    -1,
      22,    -1,    43,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    -1,    -1,    -1,    -1,    39,    -1,    22,
      -1,    43,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    -1,    -1,    -1,    -1,    39,    -1,    22,    -1,
      43,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    43,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    43
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    10,    44,    54,    55,    56,    59,    83,     4,
      44,    78,    84,    85,    44,     8,     0,    55,    59,    83,
      44,    55,    78,    87,    88,    44,    86,     4,    85,    48,
      46,    55,    87,    44,    55,    88,    46,    51,    39,    42,
      57,    58,    78,    47,    55,    46,    78,    89,    90,    91,
      45,    44,    49,    57,    44,    48,    89,    44,    47,    42,
      52,    46,    51,    11,    39,     3,    79,    47,    91,    89,
      45,     4,    78,    80,    81,    36,    48,    47,    52,    44,
      82,     4,    81,    13,    14,    20,    21,    23,    24,    37,
      38,    44,    60,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    76,    77,    79,    39,    42,    46,
      46,    39,    39,    46,    46,    46,    46,    40,    46,    50,
      51,    40,    50,     9,    62,    72,    39,    36,    44,     7,
      12,    44,    45,    46,    60,    61,    76,    61,    47,    47,
      44,    60,    61,    61,    61,    74,    75,    44,    61,    61,
      44,    61,    35,    73,    51,    61,    22,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    43,    47,    47,
      47,    51,    47,    47,    39,    42,    47,    52,    39,    39,
      49,    62,    61,    47,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    15,    19,    39,    61,
      39,    39,    74,    40,    35,    52,    73,    73,    52,    61,
      49,    16,    17,    18,    47,    39,    73,    39,    39,    39,
      17,    39
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    53,    54,    54,    54,    54,    54,    54,    55,    56,
      56,    57,    58,    58,    59,    60,    60,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    62,    63,    63,
      63,    64,    65,    66,    67,    67,    67,    68,    68,    69,
      70,    71,    72,    72,    72,    72,    72,    72,    72,    72,
      73,    73,    74,    74,    75,    75,    76,    76,    76,    77,
      78,    79,    79,    80,    80,    81,    82,    82,    83,    83,
      84,    84,    85,    86,    86,    86,    86,    86,    86,    87,
      87,    88,    89,    90,    90,    90,    91
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     3,     3,     2,     2,     1,    11,     2,
       1,     3,     2,     1,     6,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     4,     1,     1,     1,     1,     3,     5,     8,
       5,     3,     3,     5,     4,     7,     4,    10,     8,     2,
       2,     8,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     1,     0,     3,     1,     4,     1,     1,     2,
       1,     3,     2,     2,     1,     3,     3,     1,     3,     2,
       2,     1,     3,     3,     6,     6,     4,     1,     4,     2,
       1,    12,     1,     3,     1,     0,     2
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
  case 8:
#line 130 "./src/silc.y"
    {
                                                             tnode *node = createNode(FUNC, "main", -1, connect((yyvsp[-3].no),(yyvsp[-2].no)), NULL); (yyval.no)->vartype = 0;
                                                             GSymbol* tmp = malloc(sizeof(GSymbol));
                                                             Frame *frame = (Frame*)malloc(sizeof(Frame));
                                                             frame->Lvars = (yyvsp[-5].list);
                                                             *tmp = (GSymbol){.name="main", .params=NULL, .frame=frame};
                                                             GSymList = addNode(tmp, sizeof(GSymbol), GSymList);
                                                             fprintf(out ,"main:\nBRKP\n");
                                                             eval_func(node, out);
                                                            }
#line 1720 "./src/y.tab.c"
    break;

  case 9:
#line 145 "./src/silc.y"
    {}
#line 1726 "./src/y.tab.c"
    break;

  case 10:
#line 146 "./src/silc.y"
    {}
#line 1732 "./src/y.tab.c"
    break;

  case 11:
#line 149 "./src/silc.y"
    { Field *field = (Field*)malloc(sizeof(Field));
                                                            Type *type = searchType((yyvsp[-2].name), TypeList);
                                                            *field = type?(Field){.name=(yyvsp[-1].name), .type=type}:(Field){.name=(yyvsp[-1].name), .ndef=(yyvsp[-2].name)};
                                                            (yyval.field) = field;
                                                          }
#line 1742 "./src/y.tab.c"
    break;

  case 12:
#line 156 "./src/silc.y"
    { (yyvsp[0].field)->idx = ((Field*)(yyvsp[-1].list)->data)->idx + 1;
                                                             (yyval.list) = addNode((yyvsp[0].field), sizeof(Field), (yyvsp[-1].list));}
#line 1749 "./src/y.tab.c"
    break;

  case 13:
#line 158 "./src/silc.y"
    { (yyvsp[0].field)->idx = 1; (yyval.list) = addNode((yyvsp[0].field), sizeof(Field), NULL); }
#line 1755 "./src/y.tab.c"
    break;

  case 14:
#line 161 "./src/silc.y"
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
                                                            type->size = size;
                                                             TypeList = addNode(type, sizeof(Type), TypeList);
                                                           }
#line 1777 "./src/y.tab.c"
    break;

  case 15:
#line 180 "./src/silc.y"
    {char *tmp = (char*)malloc(strlen((yyvsp[-2].name)) + strlen((yyvsp[0].name)) + 1);
                                                            strcpy(tmp, (yyvsp[-2].name)); strcat(tmp, ".");strcat(tmp, (yyvsp[0].name)); (yyval.name) = tmp;}
#line 1784 "./src/y.tab.c"
    break;

  case 16:
#line 182 "./src/silc.y"
    {char *tmp = (char*)malloc(strlen((yyvsp[-2].name)) + strlen((yyvsp[0].name)) + 1);
                                                            strcpy(tmp, (yyvsp[-2].name)); strcat(tmp, ".");strcat(tmp, (yyvsp[0].name)); (yyval.name) = tmp;}
#line 1791 "./src/y.tab.c"
    break;

  case 17:
#line 189 "./src/silc.y"
    {(yyval.no) = createNode(OP, "+", -1, (yyvsp[-2].no), (yyvsp[0].no)); 
                                                        (yyval.no)->vartype = searchType("int", TypeList);}
#line 1798 "./src/y.tab.c"
    break;

  case 18:
#line 192 "./src/silc.y"
    {(yyval.no) = createNode(OP, "-", -1, (yyvsp[-2].no), (yyvsp[0].no)); 
                                                        (yyval.no)->vartype = searchType("int", TypeList);}
#line 1805 "./src/y.tab.c"
    break;

  case 19:
#line 195 "./src/silc.y"
    {(yyval.no) = createNode(OP, "*", -1, (yyvsp[-2].no), (yyvsp[0].no)); 
                                                        (yyval.no)->vartype = searchType("int", TypeList);}
#line 1812 "./src/y.tab.c"
    break;

  case 20:
#line 198 "./src/silc.y"
    {(yyval.no) = createNode(OP, "/", -1, (yyvsp[-2].no), (yyvsp[0].no)); 
                                                        (yyval.no)->vartype = searchType("int", TypeList);}
#line 1819 "./src/y.tab.c"
    break;

  case 21:
#line 201 "./src/silc.y"
    {(yyval.no) = createNode(OP, "<=", -1, (yyvsp[-2].no), (yyvsp[0].no)); 
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 1826 "./src/y.tab.c"
    break;

  case 22:
#line 204 "./src/silc.y"
    {(yyval.no) = createNode(OP, ">=", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 1833 "./src/y.tab.c"
    break;

  case 23:
#line 207 "./src/silc.y"
    {(yyval.no) = createNode(OP, "==", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 1840 "./src/y.tab.c"
    break;

  case 24:
#line 210 "./src/silc.y"
    {(yyval.no) = createNode(OP, "<", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 1847 "./src/y.tab.c"
    break;

  case 25:
#line 213 "./src/silc.y"
    {(yyval.no) = createNode(OP, ">", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 1854 "./src/y.tab.c"
    break;

  case 26:
#line 216 "./src/silc.y"
    {(yyval.no) = createNode(OP, "!=", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 1861 "./src/y.tab.c"
    break;

  case 27:
#line 219 "./src/silc.y"
    {(yyval.no) = createNode(OP, "AND", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 1868 "./src/y.tab.c"
    break;

  case 28:
#line 222 "./src/silc.y"
    {(yyval.no) = createNode(OP, "%", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("int", TypeList);}
#line 1875 "./src/y.tab.c"
    break;

  case 29:
#line 225 "./src/silc.y"
    {(yyval.no) = (yyvsp[-1].no);}
#line 1881 "./src/y.tab.c"
    break;

  case 30:
#line 226 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no); (yyval.no)->vartype = searchType("int", TypeList);}
#line 1887 "./src/y.tab.c"
    break;

  case 31:
#line 227 "./src/silc.y"
    {(yyval.no) = createNode(VAR, (yyvsp[0].name), -1, NULL, NULL); (yyval.no)->vartype = getSymbol((yyvsp[0].name));}
#line 1893 "./src/y.tab.c"
    break;

  case 32:
#line 228 "./src/silc.y"
    {(yyval.no) = createNode(VAR, (yyvsp[-3].name), -1, (yyvsp[-1].no), NULL);(yyval.no)->vartype = getSymbol((yyvsp[-3].name));}
#line 1899 "./src/y.tab.c"
    break;

  case 33:
#line 229 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no); (yyval.no)->vartype = searchType("str", TypeList);}
#line 1905 "./src/y.tab.c"
    break;

  case 34:
#line 230 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 1911 "./src/y.tab.c"
    break;

  case 35:
#line 231 "./src/silc.y"
    {(yyval.no) = createNode(VAR, (yyvsp[0].name), -1, NULL, NULL);(yyval.no)->vartype = getSymbol((yyvsp[0].name));}
#line 1917 "./src/y.tab.c"
    break;

  case 36:
#line 232 "./src/silc.y"
    {(yyval.no) = createNode(CONST, "\0", 0, NULL, NULL); (yyval.no)->vartype = searchType("null", TypeList);}
#line 1923 "./src/y.tab.c"
    break;

  case 37:
#line 235 "./src/silc.y"
    {(yyval.no) = createNode(RET, "\0", -1, (yyvsp[-1].no), NULL);}
#line 1929 "./src/y.tab.c"
    break;

  case 38:
#line 239 "./src/silc.y"
    {tnode *tmp = createNode(VAR, (yyvsp[-2].name), -1, NULL, NULL);tmp->vartype = getSymbol((yyvsp[-2].name));
                                                        (yyval.no) = createNode(READ, "", -1, tmp, NULL);}
#line 1936 "./src/y.tab.c"
    break;

  case 39:
#line 241 "./src/silc.y"
    {tnode *tmp = createNode(VAR, (yyvsp[-5].name), -1, (yyvsp[-3].no), NULL);tmp->vartype = getSymbol((yyvsp[-5].name));
                                                        (yyval.no) = createNode(READ, "", -1, tmp, NULL);}
#line 1943 "./src/y.tab.c"
    break;

  case 40:
#line 243 "./src/silc.y"
    {tnode *tmp = createNode(VAR, (yyvsp[-2].name), -1, NULL, NULL);tmp->vartype = getSymbol((yyvsp[-2].name));
                                                        (yyval.no) = createNode(READ, "", -1, tmp, NULL);}
#line 1950 "./src/y.tab.c"
    break;

  case 41:
#line 247 "./src/silc.y"
    {(yyval.no) = createNode(INIT, "", -1, NULL, NULL);(yyval.no)->vartype = searchType("int", TypeList);}
#line 1956 "./src/y.tab.c"
    break;

  case 42:
#line 250 "./src/silc.y"
    {(yyval.no) = createNode(ALLOC, "", -1, NULL, NULL); (yyval.no)->vartype = searchType("int", TypeList);}
#line 1962 "./src/y.tab.c"
    break;

  case 43:
#line 253 "./src/silc.y"
    {(yyval.no) = createNode(WRITE, "", -1, (yyvsp[-2].no), NULL);}
#line 1968 "./src/y.tab.c"
    break;

  case 44:
#line 256 "./src/silc.y"
    { tnode *tmp = createNode(VAR, (yyvsp[-3].name), -1, NULL, NULL);tmp->vartype = getSymbol((yyvsp[-3].name));
                                                         (yyval.no) = createNode(ASSN, "", -1, tmp, (yyvsp[-1].no));}
#line 1975 "./src/y.tab.c"
    break;

  case 45:
#line 258 "./src/silc.y"
    {tnode *tmp = createNode(VAR, (yyvsp[-6].name), -1, (yyvsp[-4].no), NULL);tmp->vartype = getSymbol((yyvsp[-6].name));
                                                         (yyval.no) = createNode(ASSN, "", -1, tmp, (yyvsp[-1].no));}
#line 1982 "./src/y.tab.c"
    break;

  case 46:
#line 260 "./src/silc.y"
    { tnode *tmp = createNode(VAR, (yyvsp[-3].name), -1, NULL, NULL);tmp->vartype = getSymbol((yyvsp[-3].name));
                                                        (yyval.no) = createNode(ASSN, "", -1, tmp, (yyvsp[-1].no));}
#line 1989 "./src/y.tab.c"
    break;

  case 47:
#line 265 "./src/silc.y"
    {
                                                            tnode *tmp = createNode(IF_BODY, "", -1, (yyvsp[-4].no), (yyvsp[-2].no));
                                                            (yyval.no) = createNode(IF, "", -1, (yyvsp[-7].no), tmp);
                                                        }
#line 1998 "./src/y.tab.c"
    break;

  case 48:
#line 269 "./src/silc.y"
    {
                                                            tnode *tmp = createNode(IF_BODY, "", -1, (yyvsp[-2].no), NULL);
                                                            (yyval.no) = createNode(IF, "", -1, (yyvsp[-5].no), tmp);
                                                        }
#line 2007 "./src/y.tab.c"
    break;

  case 49:
#line 275 "./src/silc.y"
    {(yyval.no) = createNode(BREAK, "", -1, NULL, NULL);}
#line 2013 "./src/y.tab.c"
    break;

  case 50:
#line 278 "./src/silc.y"
    {(yyval.no) = createNode(CONT, "", -1, NULL, NULL);}
#line 2019 "./src/y.tab.c"
    break;

  case 51:
#line 282 "./src/silc.y"
    {(yyval.no) = createNode(WHILE, "", -1, (yyvsp[-5].no), (yyvsp[-2].no));}
#line 2025 "./src/y.tab.c"
    break;

  case 52:
#line 285 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2031 "./src/y.tab.c"
    break;

  case 53:
#line 286 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2037 "./src/y.tab.c"
    break;

  case 54:
#line 287 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2043 "./src/y.tab.c"
    break;

  case 55:
#line 288 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2049 "./src/y.tab.c"
    break;

  case 56:
#line 289 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2055 "./src/y.tab.c"
    break;

  case 57:
#line 290 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2061 "./src/y.tab.c"
    break;

  case 58:
#line 291 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2067 "./src/y.tab.c"
    break;

  case 59:
#line 292 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2073 "./src/y.tab.c"
    break;

  case 60:
#line 295 "./src/silc.y"
    {(yyval.no) = connect((yyvsp[-1].no), (yyvsp[0].no));}
#line 2079 "./src/y.tab.c"
    break;

  case 61:
#line 296 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2085 "./src/y.tab.c"
    break;

  case 62:
#line 300 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2091 "./src/y.tab.c"
    break;

  case 63:
#line 301 "./src/silc.y"
    {(yyval.no) = NULL;}
#line 2097 "./src/y.tab.c"
    break;

  case 64:
#line 304 "./src/silc.y"
    {(yyval.no) = connect((yyvsp[0].no), (yyvsp[-2].no));}
#line 2103 "./src/y.tab.c"
    break;

  case 65:
#line 305 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2109 "./src/y.tab.c"
    break;

  case 66:
#line 307 "./src/silc.y"
    {  GSymbol* sym = (GSymbol*)searchSymbol((yyvsp[-3].name), GSymList);
                                                    if(sym== NULL) yyerror("Function is not declared");
                                                    (yyval.no) = createNode(FUNC, (yyvsp[-3].name), -1, (yyvsp[-1].no), NULL);
                                                    
                                                    (yyval.no)->vartype = sym->type;}
#line 2119 "./src/y.tab.c"
    break;

  case 67:
#line 312 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2125 "./src/y.tab.c"
    break;

  case 68:
#line 313 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2131 "./src/y.tab.c"
    break;

  case 69:
#line 315 "./src/silc.y"
    { (yyval.no) = (yyvsp[-1].no);}
#line 2137 "./src/y.tab.c"
    break;

  case 70:
#line 323 "./src/silc.y"
    {(yyval.name) = (yyvsp[0].name);}
#line 2143 "./src/y.tab.c"
    break;

  case 71:
#line 330 "./src/silc.y"
    { (yyval.list) = (yyvsp[-1].list);}
#line 2149 "./src/y.tab.c"
    break;

  case 72:
#line 331 "./src/silc.y"
    { (yyval.list) = NULL;}
#line 2155 "./src/y.tab.c"
    break;

  case 73:
#line 334 "./src/silc.y"
    { (yyval.list) = connectList((yyvsp[-1].list), (yyvsp[0].list), sizeof(LSymbol));}
#line 2161 "./src/y.tab.c"
    break;

  case 74:
#line 335 "./src/silc.y"
    {(yyval.list) = (yyvsp[0].list);}
#line 2167 "./src/y.tab.c"
    break;

  case 75:
#line 339 "./src/silc.y"
    {   (yyval.list) = NULL;
                                                            LinkedList* gvars = (yyvsp[-1].list);
                                                            while(gvars) {
                                                                char *var = (char*)(gvars->data);
                                                                LSymbol* tmp = (LSymbol*)malloc(sizeof(LSymbol));
                                                                Type *type = (Type*)searchType((yyvsp[-2].name), TypeList);
                                                                if(type==NULL) yyerror("Undefined type");
                                                                *tmp = (LSymbol){.name=var, .type=type, .size=1};
                                                                (yyval.list) = addNode(tmp, sizeof(LSymbol), (yyval.list));
                                                                gvars = gvars->next;
                                                            }
                                                            LinkedList *copy = copyList((yyval.list), sizeof(LSymbol));
                                                            curLvar = connectList(curLvar, copy, sizeof(LSymbol));
                                                        }
#line 2186 "./src/y.tab.c"
    break;

  case 76:
#line 354 "./src/silc.y"
    {(yyval.list) = addNode(strdup((yyvsp[0].name)), sizeof((yyvsp[0].name)), (yyvsp[-2].list));}
#line 2192 "./src/y.tab.c"
    break;

  case 77:
#line 355 "./src/silc.y"
    {(yyval.list) = addNode(strdup((yyvsp[0].name)), sizeof((yyvsp[0].name)), NULL);}
#line 2198 "./src/y.tab.c"
    break;

  case 78:
#line 363 "./src/silc.y"
    {}
#line 2204 "./src/y.tab.c"
    break;

  case 79:
#line 364 "./src/silc.y"
    {}
#line 2210 "./src/y.tab.c"
    break;

  case 80:
#line 367 "./src/silc.y"
    {}
#line 2216 "./src/y.tab.c"
    break;

  case 81:
#line 368 "./src/silc.y"
    {}
#line 2222 "./src/y.tab.c"
    break;

  case 82:
#line 372 "./src/silc.y"
    {
                                                            LinkedList* gvars = (yyvsp[-1].list);
                                                            while(gvars) {
                                                                GVariable *var = (GVariable*)(gvars->data);
                                                                if(searchSymbol(var->name, GSymList)) {
                                                                    yyerror("Variable is already declared");
                                                                }
                                                                GSymbol* tmp = (GSymbol*)malloc(sizeof(GSymbol));
                                                                Type *type = (Type*)searchType((yyvsp[-2].name), TypeList);
                                                                if(type==NULL) yyerror("Undefined type");
                                                                *tmp = (GSymbol){.name=var->name, .type=type, .size=var->size, .params=var->params, .binding=curMemory};
                                                                GSymList = addNode(tmp, sizeof(GSymbol), GSymList);
                                                                gvars = gvars->next;
                                                                curMemory+=var->size;
                                                            }
                                                        }
#line 2243 "./src/y.tab.c"
    break;

  case 83:
#line 389 "./src/silc.y"
    {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[0].name), .size=1};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), (yyvsp[-2].list));
                                                        }
#line 2252 "./src/y.tab.c"
    break;

  case 84:
#line 394 "./src/silc.y"
    {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[-3].name), .size=(yyvsp[-1].no)->val};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), (yyvsp[-5].list));
                                                        }
#line 2261 "./src/y.tab.c"
    break;

  case 85:
#line 399 "./src/silc.y"
    {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[-3].name), .params=(yyvsp[-1].no), .size=0};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), (yyvsp[-5].list));
                                                        }
#line 2270 "./src/y.tab.c"
    break;

  case 86:
#line 404 "./src/silc.y"
    {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[-3].name), .size=(yyvsp[-1].no)->val};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), NULL);
                                                        }
#line 2279 "./src/y.tab.c"
    break;

  case 87:
#line 409 "./src/silc.y"
    {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[0].name), .size=1};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), NULL);
                                                        }
#line 2288 "./src/y.tab.c"
    break;

  case 88:
#line 414 "./src/silc.y"
    {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[-3].name), .params=(yyvsp[-1].no), .size=0};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), NULL);
                                                        }
#line 2297 "./src/y.tab.c"
    break;

  case 89:
#line 424 "./src/silc.y"
    {
                                                         fprintf(out, "%s:\n", (yyvsp[0].no)->varname);
                                                         eval_func((yyvsp[0].no), out);}
#line 2305 "./src/y.tab.c"
    break;

  case 90:
#line 428 "./src/silc.y"
    {
                                                         fprintf(out, "%s:\n", (yyvsp[0].no)->varname);
                                                         eval_func((yyvsp[0].no), out);}
#line 2313 "./src/y.tab.c"
    break;

  case 91:
#line 433 "./src/silc.y"
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
                                                                        if(strcmp((yyvsp[-11].name), getSymbol((yyvsp[-2].no)->left->varname)->name)) yyerror("Return type is not correct");
                                                                    } else if(strcmp((yyvsp[-11].name), (yyvsp[-2].no)->left->vartype->name)) yyerror("Return type is not correct");
                                                                    (yyval.no) = createNode(FUNC, (yyvsp[-10].name), -1, connect((yyvsp[-3].no), (yyvsp[-2].no)), (yyvsp[-8].no)); (yyval.no)->vartype = searchType((yyvsp[-11].name), TypeList);
                                                                    curLvar = NULL;
                                                                    }
#line 2333 "./src/y.tab.c"
    break;

  case 92:
#line 451 "./src/silc.y"
    {curLvar = addParam((yyvsp[0].no)); (yyval.no) = (yyvsp[0].no);}
#line 2339 "./src/y.tab.c"
    break;

  case 93:
#line 453 "./src/silc.y"
    { (yyval.no) = connect((yyvsp[0].no), (yyvsp[-2].no));}
#line 2345 "./src/y.tab.c"
    break;

  case 94:
#line 454 "./src/silc.y"
    { (yyval.no) = (yyvsp[0].no);}
#line 2351 "./src/y.tab.c"
    break;

  case 95:
#line 455 "./src/silc.y"
    { (yyval.no) = NULL;}
#line 2357 "./src/y.tab.c"
    break;

  case 96:
#line 458 "./src/silc.y"
    {(yyval.no) = createNode(VAR, (yyvsp[0].name), -1, NULL, NULL); (yyval.no)->vartype = searchType((yyvsp[-1].name), TypeList);}
#line 2363 "./src/y.tab.c"
    break;


#line 2367 "./src/y.tab.c"

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
#line 463 "./src/silc.y"





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
    curLvar = NULL;
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
