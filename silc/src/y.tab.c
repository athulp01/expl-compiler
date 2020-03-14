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

#line 212 "./src/y.tab.c"

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
    _FREE = 280,
    _CLASS = 281,
    _SELF = 282,
    _ENDCLASS = 283,
    _NEW = 284,
    _LT = 285,
    _GT = 286,
    _EQ = 287,
    _NE = 288,
    _LE = 289,
    _GE = 290,
    _PLUS = 291,
    _MINUS = 292,
    _MUL = 293,
    _DIV = 294,
    _END = 295,
    _BEGIN = 296,
    _READ = 297,
    _WRITE = 298,
    _SEMI = 299,
    _EQUALS = 300,
    _Q = 301,
    _COMMA = 302,
    _MOD = 303,
    _ID = 304,
    _NUM = 305
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
#define _CLASS 281
#define _SELF 282
#define _ENDCLASS 283
#define _NEW 284
#define _LT 285
#define _GT 286
#define _EQ 287
#define _NE 288
#define _LE 289
#define _GE 290
#define _PLUS 291
#define _MINUS 292
#define _MUL 293
#define _DIV 294
#define _END 295
#define _BEGIN 296
#define _READ 297
#define _WRITE 298
#define _SEMI 299
#define _EQUALS 300
#define _Q 301
#define _COMMA 302
#define _MOD 303
#define _ID 304
#define _NUM 305

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 145 "./src/silc.y"

	tnode *no;
    char *name;
    char* type;
    LinkedList* list;
    Field* field;
	

#line 364 "./src/y.tab.c"

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
#define YYFINAL  20
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   656

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  58
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  115
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  282

#define YYUNDEFTOK  2
#define YYMAXUTOK   305

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
      51,    52,     2,     2,     2,     2,    55,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    56,     2,    57,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    53,     2,    54,     2,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   178,   178,   179,   180,   181,   182,   183,   184,   192,
     206,   207,   210,   235,   236,   238,   252,   268,   271,   272,
     274,   303,   328,   329,   332,   339,   343,   346,   366,   368,
     375,   378,   381,   384,   387,   390,   393,   396,   399,   402,
     405,   408,   411,   412,   413,   414,   415,   416,   417,   418,
     421,   425,   427,   429,   433,   436,   439,   441,   445,   448,
     450,   452,   456,   461,   467,   470,   473,   477,   478,   479,
     480,   481,   482,   483,   484,   485,   488,   489,   493,   494,
     497,   498,   500,   504,   505,   506,   507,   509,   517,   524,
     525,   528,   529,   533,   549,   550,   558,   559,   562,   563,
     567,   586,   591,   596,   601,   606,   611,   621,   625,   630,
     645,   662,   669,   670,   671,   674
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
  "_AND", "_INIT", "_ALLOC", "_FREE", "_CLASS", "_SELF", "_ENDCLASS",
  "_NEW", "_LT", "_GT", "_EQ", "_NE", "_LE", "_GE", "_PLUS", "_MINUS",
  "_MUL", "_DIV", "_END", "_BEGIN", "_READ", "_WRITE", "_SEMI", "_EQUALS",
  "_Q", "_COMMA", "_MOD", "_ID", "_NUM", "'('", "')'", "'{'", "'}'", "'.'",
  "'['", "']'", "$accept", "program", "mainblock", "classdefblock",
  "classdef", "methodlst", "methoddef", "classmemberblock",
  "classmemberlst", "classmember", "methodcall", "typedefblock",
  "fielddef", "fieldlst", "typedef", "field", "expr", "return", "read",
  "init", "alloc", "new", "free", "write", "assgn", "ifstmt", "break",
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
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,    40,    41,   123,   125,    46,    91,    93
};
# endif

#define YYPACT_NINF -148

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-148)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-1)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       3,    -1,     8,    12,    85,    87,  -148,    19,  -148,    -5,
    -148,    45,  -148,  -148,    64,     4,  -148,    70,    90,    66,
    -148,  -148,    83,  -148,    19,  -148,    85,  -148,    88,    45,
    -148,    35,   126,  -148,  -148,    95,   146,    93,  -148,    45,
     106,  -148,  -148,    95,   109,  -148,   114,  -148,    -3,   119,
      95,    95,   116,  -148,    45,    95,   130,   123,   134,  -148,
     127,    60,   179,  -148,   148,     5,  -148,    64,    65,  -148,
     147,   192,  -148,   145,  -148,  -148,    95,  -148,    95,   150,
    -148,  -148,  -148,  -148,   136,   173,  -148,   152,     6,   157,
     151,  -148,   154,   153,  -148,  -148,    95,  -148,   158,     7,
    -148,   315,    17,  -148,  -148,   156,  -148,   142,  -148,  -148,
     160,   162,   165,   171,   163,   167,   178,   180,   182,   183,
      47,  -148,    14,  -148,  -148,  -148,  -148,  -148,  -148,  -148,
    -148,  -148,  -148,  -148,  -148,   203,   186,  -148,   315,   181,
     184,  -148,   189,   170,   170,  -148,  -148,   187,   188,   193,
     194,   195,   170,   170,   170,   200,   170,   170,   170,   204,
     170,   201,  -148,  -148,   203,   315,    44,  -148,  -148,  -148,
      54,  -148,   170,   -15,   421,  -148,   444,  -148,  -148,   208,
     209,    84,    52,   467,   513,   589,  -148,    68,  -148,   337,
     532,    77,  -148,   551,   202,   222,   203,   315,   223,   170,
     490,   170,   170,   170,   170,   170,   170,   170,   170,   170,
     170,   170,   170,   220,   246,   224,  -148,   225,   170,   226,
     227,  -148,   170,  -148,   221,  -148,  -148,  -148,  -148,   213,
     232,   203,   315,   365,  -148,   608,   608,   608,   608,   608,
     608,   608,   -13,   -13,  -148,  -148,   117,   315,   315,  -148,
    -148,   393,  -148,  -148,  -148,   170,  -148,   219,   238,   203,
    -148,   234,   261,   228,   570,  -148,   233,   248,   315,   245,
     247,   249,  -148,  -148,   240,   288,  -148,  -148,  -148,  -148,
     251,  -148
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     8,     0,    11,     0,
      23,     0,    97,    88,     0,     0,    99,     0,     0,     0,
       1,    10,     0,     5,     0,    22,    88,     6,     0,     0,
     108,   105,     0,    96,    98,     0,     0,     0,     7,     0,
       0,     4,   107,   114,     0,   100,     0,    26,     0,     0,
       0,     0,     0,     3,     0,   114,     0,     0,   111,   113,
       0,   101,     0,    25,     0,     0,    19,     0,     0,    14,
       0,     0,     2,     0,   115,   106,     0,   104,   114,     0,
      27,    24,    17,    18,     0,     0,    13,     0,     0,     0,
       0,   112,     0,     0,    20,    12,   114,    90,     0,     0,
      92,     0,     0,   103,   102,     0,    95,     0,    89,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    85,     0,    67,    84,    83,    86,    75,    68,    69,
      70,    72,    73,    71,    77,     0,     0,    74,     0,     0,
       0,    93,     0,     0,     0,    64,    65,     0,     0,     0,
       0,     0,     0,     0,    79,     0,     0,     0,    79,     0,
       0,     0,    76,    87,     0,     0,     0,    94,    46,    49,
      44,    43,     0,    48,     0,    47,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    78,    81,     0,    28,     0,
       0,     0,    29,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,     0,     0,     0,
       0,    59,    79,    82,     0,    61,    21,    50,     9,     0,
       0,     0,     0,     0,    42,    40,    37,    38,    36,    39,
      34,    35,    30,    31,    32,    33,    41,     0,     0,    57,
      51,     0,    53,    58,    80,     0,   110,     0,     0,     0,
      45,     0,     0,     0,     0,   109,     0,     0,     0,     0,
       0,     0,    60,    16,     0,     0,    63,    66,    52,    15,
       0,    62
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -148,  -148,    79,   283,     9,  -148,   229,  -148,  -148,   231,
    -148,  -148,   250,  -148,   290,  -101,  -129,  -147,  -148,  -148,
    -148,  -148,  -148,  -148,  -148,  -148,  -148,  -148,  -148,  -134,
    -126,    78,   149,   -97,  -148,   137,  -100,  -148,   207,  -148,
      11,  -148,   299,   252,   276,   -22,   -36,  -148,   242
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,     6,     7,     8,    68,    69,    51,    65,    66,
     121,     9,    47,    48,    10,   173,   185,   161,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   186,   187,   175,   137,    56,    89,    99,   100,   107,
      11,    15,    16,    32,    29,    30,    57,    58,    59
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     122,   162,   139,    12,   136,     2,     1,    42,    33,    82,
      97,   108,   164,     2,   174,   176,    21,   195,    22,    73,
      88,     3,     1,   183,   184,   210,   211,   189,   190,     3,
     162,   193,    42,    21,   122,    39,   158,   122,   136,   196,
     159,   136,    92,   200,     4,     3,    13,    88,    13,   230,
     182,    62,     4,    13,    13,    13,    13,    17,   138,   157,
     105,    18,   162,   122,   122,   158,   198,   136,   136,   159,
     233,   231,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   258,   197,    43,    20,    23,   251,
      27,    44,   153,    19,    26,   122,   122,   162,   154,   136,
     136,    38,   155,   156,   219,   154,   259,   159,    41,   155,
     199,    78,   267,    31,    13,   222,    79,    37,    53,    85,
     223,   261,   262,    35,   222,   162,   264,   162,   162,   226,
     122,   122,     4,    72,   136,   136,   217,    40,    14,   155,
     218,   162,   275,    36,    13,    52,   122,   122,    28,    50,
     136,   136,    14,   208,   209,   210,   211,    55,   122,    60,
     122,   122,   136,    61,   136,   136,    28,   122,    64,    71,
      45,   136,    49,    46,   122,    75,    28,   168,   136,    74,
      94,    76,   169,    46,    77,    49,   141,    67,    70,   142,
      80,    28,    81,   114,   115,    88,    87,    90,   101,   117,
      93,    95,    67,    96,   102,    70,   103,   106,   140,   145,
     104,   143,   160,   144,   147,   146,   110,   111,   148,   170,
     171,   172,   165,   112,   113,    98,   114,   115,   116,   149,
     163,   150,   117,   151,   152,   247,    98,   166,   167,   177,
     178,   194,   179,   180,   181,   118,   119,   110,   111,   188,
     268,   269,   120,   192,   112,   113,   228,   114,   115,   116,
     215,   216,   229,   117,   232,   248,   255,   256,   249,   250,
     252,   253,   257,   265,   110,   111,   118,   119,   266,   270,
     271,   112,   113,   120,   114,   115,   116,   273,   274,   276,
     117,   277,    24,   278,   279,   281,    83,    86,    63,    25,
     254,   110,   111,   118,   119,   280,   109,   191,   112,   113,
     120,   114,   115,   116,    34,    54,     0,   117,    91,    84,
       0,     0,     0,     0,     0,     0,     0,     0,   110,   111,
     118,   119,     0,     0,     0,   112,   113,   120,   114,   115,
     116,     0,     0,     0,   117,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   118,   119,   201,
       0,     0,     0,     0,   120,     0,     0,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,     0,     0,     0,
       0,     0,     0,     0,     0,   212,     0,   201,     0,     0,
       0,     0,     0,     0,   224,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,     0,     0,     0,     0,     0,
       0,     0,     0,   212,     0,   201,     0,     0,     0,     0,
       0,     0,   260,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,     0,     0,     0,     0,     0,     0,     0,
       0,   212,     0,   201,     0,     0,     0,     0,     0,     0,
     263,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,     0,     0,     0,     0,     0,   201,     0,     0,   212,
       0,     0,     0,   213,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,     0,     0,     0,     0,     0,   201,
       0,     0,   212,     0,     0,     0,   214,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,     0,     0,     0,
       0,     0,   201,     0,     0,   212,     0,     0,     0,   220,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
       0,     0,     0,     0,     0,   201,     0,     0,   212,     0,
       0,     0,   234,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,     0,   201,     0,     0,   221,     0,     0,
       0,   212,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,     0,   201,     0,     0,   225,     0,     0,     0,
     212,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,     0,   201,     0,     0,   227,     0,     0,     0,   212,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
       0,   201,     0,     0,   272,     0,     0,     0,   212,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,     0,
      -1,     0,     0,     0,     0,     0,     0,   212,    -1,    -1,
      -1,    -1,    -1,    -1,   208,   209,   210,   211,     0,     0,
       0,     0,     0,     0,     0,     0,   212
};

static const yytype_int16 yycheck[] =
{
     101,   135,   102,     4,   101,    10,     3,    29,     4,     4,
       4,     4,   138,    10,   143,   144,     7,   164,     7,    55,
       3,    26,     3,   152,   153,    38,    39,   156,   157,    26,
     164,   160,    54,    24,   135,    24,    51,   138,   135,   165,
      55,   138,    78,   172,    49,    26,    49,     3,    49,   196,
     151,    54,    49,    49,    49,    49,    49,    49,    41,    45,
      96,    49,   196,   164,   165,    51,   166,   164,   165,    55,
     199,   197,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   231,    41,    51,     0,     9,   218,
      11,    56,    45,     8,    49,   196,   197,   231,    51,   196,
     197,    22,    55,    56,    52,    51,   232,    55,    29,    55,
      56,    51,   259,    49,    49,    47,    56,    51,    39,    54,
      52,   247,   248,    53,    47,   259,   255,   261,   262,    52,
     231,   232,    49,    54,   231,   232,    52,    49,     1,    55,
      56,   275,   268,    53,    49,    52,   247,   248,    11,     3,
     247,   248,    15,    36,    37,    38,    39,    51,   259,    50,
     261,   262,   259,    49,   261,   262,    29,   268,    49,    53,
      44,   268,    35,    47,   275,    52,    39,     7,   275,    49,
      44,    47,    12,    47,    57,    48,    44,    50,    51,    47,
      11,    54,    44,    23,    24,     3,    49,    52,    41,    29,
      50,    28,    65,    51,    53,    68,    52,    49,    52,    44,
      57,    51,     9,    51,    51,    44,    13,    14,    51,    49,
      50,    51,    41,    20,    21,    88,    23,    24,    25,    51,
      44,    51,    29,    51,    51,    15,    99,    53,    49,    52,
      52,    40,    49,    49,    49,    42,    43,    13,    14,    49,
      16,    17,    49,    49,    20,    21,    54,    23,    24,    25,
      52,    52,    40,    29,    41,    19,    45,    54,    44,    44,
      44,    44,    40,    54,    13,    14,    42,    43,    40,    18,
      52,    20,    21,    49,    23,    24,    25,    54,    40,    44,
      29,    44,     9,    44,    54,    44,    65,    68,    48,     9,
     222,    13,    14,    42,    43,    17,    99,   158,    20,    21,
      49,    23,    24,    25,    15,    39,    -1,    29,    76,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,    14,
      42,    43,    -1,    -1,    -1,    20,    21,    49,    23,    24,
      25,    -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    43,    22,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    48,
      -1,    -1,    -1,    52,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    48,    -1,    -1,    -1,    52,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    48,    -1,    -1,    -1,    52,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    48,    -1,
      -1,    -1,    52,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    -1,    22,    -1,    -1,    44,    -1,    -1,
      -1,    48,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    -1,    22,    -1,    -1,    44,    -1,    -1,    -1,
      48,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    -1,    22,    -1,    -1,    44,    -1,    -1,    -1,    48,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      -1,    22,    -1,    -1,    44,    -1,    -1,    -1,    48,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    48,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    48
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    10,    26,    49,    59,    60,    61,    62,    69,
      72,    98,     4,    49,    93,    99,   100,    49,    49,     8,
       0,    62,    98,    60,    61,    72,    49,    60,    93,   102,
     103,    49,   101,     4,   100,    53,    53,    51,    60,    98,
      49,    60,   103,    51,    56,    44,    47,    70,    71,    93,
       3,    65,    52,    60,   102,    51,    93,   104,   105,   106,
      50,    49,    54,    70,    49,    66,    67,    93,    63,    64,
      93,    53,    60,   104,    49,    52,    47,    57,    51,    56,
      11,    44,     4,    67,   101,    54,    64,    49,     3,    94,
      52,   106,   104,    50,    44,    28,    51,     4,    93,    95,
      96,    41,    53,    52,    57,   104,    49,    97,     4,    96,
      13,    14,    20,    21,    23,    24,    25,    29,    42,    43,
      49,    68,    73,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    91,    92,    41,    94,
      52,    44,    47,    51,    51,    44,    44,    51,    51,    51,
      51,    51,    51,    45,    51,    55,    56,    45,    51,    55,
       9,    75,    87,    44,    88,    41,    53,    49,     7,    12,
      49,    50,    51,    73,    74,    91,    74,    52,    52,    49,
      49,    49,    73,    74,    74,    74,    89,    90,    49,    74,
      74,    90,    49,    74,    40,    75,    88,    41,    94,    56,
      74,    22,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    48,    52,    52,    52,    52,    52,    56,    52,
      52,    44,    47,    52,    57,    44,    52,    44,    54,    40,
      75,    88,    41,    74,    52,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    15,    19,    44,
      44,    74,    44,    44,    89,    45,    54,    40,    75,    88,
      57,    88,    88,    57,    74,    54,    40,    75,    16,    17,
      18,    52,    44,    54,    40,    88,    44,    44,    44,    54,
      17,    44
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    58,    59,    59,    59,    59,    59,    59,    59,    60,
      61,    61,    62,    63,    63,    64,    64,    65,    66,    66,
      67,    68,    69,    69,    70,    71,    71,    72,    73,    73,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      75,    76,    76,    76,    77,    78,    79,    80,    81,    82,
      82,    82,    83,    83,    84,    85,    86,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    88,    88,    89,    89,
      90,    90,    91,    91,    91,    91,    91,    92,    93,    94,
      94,    95,    95,    96,    97,    97,    98,    98,    99,    99,
     100,   101,   101,   101,   101,   101,   101,   102,   102,   103,
     103,   104,   105,   105,   105,   106
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     5,     4,     3,     2,     2,     3,     1,    11,
       2,     1,     7,     2,     1,    12,    11,     3,     2,     1,
       3,     4,     2,     1,     3,     2,     1,     6,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     1,     4,     1,     1,     1,     1,
       3,     5,     8,     5,     3,     3,     4,     5,     5,     4,
       7,     4,    10,     8,     2,     2,     8,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     1,     0,
       3,     1,     4,     1,     1,     1,     1,     2,     1,     3,
       2,     2,     1,     3,     3,     1,     3,     2,     2,     1,
       3,     3,     6,     6,     4,     1,     4,     2,     1,    12,
      11,     1,     3,     1,     0,     2
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
  case 9:
#line 192 "./src/silc.y"
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
#line 1846 "./src/y.tab.c"
    break;

  case 12:
#line 210 "./src/silc.y"
    { 
                                                                 if(searchClass((yyvsp[-5].name), ClassList)) yyerror("Duplicate class");
                                                                ClassDef *class = (ClassDef*)malloc(sizeof(ClassDef));
                                                                 class->name = (yyvsp[-5].name);
                                                                 curClassName = (yyvsp[-5].name);
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
                                                                     fprintf(out, "%s:\n",s);
                                                                     eval_method(tmp, curClassMethod, out);
                                                                     func = func->next;
                                                                 }
                                                                 curClassMethod = NULL;
                                                                 curClassField = NULL;
                                                                }
#line 1874 "./src/y.tab.c"
    break;

  case 13:
#line 235 "./src/silc.y"
    {(yyval.list) = connectList((yyvsp[-1].list), (yyvsp[0].list), sizeof(tnode));}
#line 1880 "./src/y.tab.c"
    break;

  case 14:
#line 236 "./src/silc.y"
    {(yyval.list) = (yyvsp[0].list);}
#line 1886 "./src/y.tab.c"
    break;

  case 15:
#line 238 "./src/silc.y"
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
#line 1905 "./src/y.tab.c"
    break;

  case 16:
#line 252 "./src/silc.y"
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
#line 1923 "./src/y.tab.c"
    break;

  case 17:
#line 268 "./src/silc.y"
    {(yyval.list) = (yyvsp[-1].list);}
#line 1929 "./src/y.tab.c"
    break;

  case 18:
#line 271 "./src/silc.y"
    {(yyval.list) = connectList((yyvsp[-1].list), (yyvsp[0].list), sizeof(Field));}
#line 1935 "./src/y.tab.c"
    break;

  case 19:
#line 272 "./src/silc.y"
    {(yyval.list) = (yyvsp[0].list);}
#line 1941 "./src/y.tab.c"
    break;

  case 20:
#line 274 "./src/silc.y"
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
                                                                        methods = addNode(tmp, sizeof(Method), methods);
                                                                    } else {
                                                                        if(getField(var->name, curClassField)) yyerror("Duplicate field");
                                                                        Field* tmp = (Field*)malloc(sizeof(Field));
                                                                        ClassDef *class = searchClass((yyvsp[-2].name), ClassList);
                                                                        Type *type = (Type*)searchType((yyvsp[-2].name), TypeList);
                                                                        if(type==NULL && class==NULL) yyerror("Undefined type");
                                                                        *tmp = (Field){.name=var->name, .type=type, .class=class, .idx = curClassFieldMem++};
                                                                        fields = addNode(tmp, sizeof(Field), fields);
                                                                    }
                                                                    gvars = gvars->next;
                                                                }
                                                                curClassMethod = connectList(curClassMethod, methods, sizeof(Method));
                                                                curClassField = connectList(curClassField, fields, sizeof(Field));
                                                            }
#line 1973 "./src/y.tab.c"
    break;

  case 21:
#line 303 "./src/silc.y"
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
#line 2000 "./src/y.tab.c"
    break;

  case 22:
#line 328 "./src/silc.y"
    {}
#line 2006 "./src/y.tab.c"
    break;

  case 23:
#line 329 "./src/silc.y"
    {}
#line 2012 "./src/y.tab.c"
    break;

  case 24:
#line 332 "./src/silc.y"
    { Field *field = (Field*)malloc(sizeof(Field));
                                                            Type *type = searchType((yyvsp[-2].name), TypeList);
                                                            *field = type?(Field){.name=(yyvsp[-1].name), .type=type}:(Field){.name=(yyvsp[-1].name), .ndef=(yyvsp[-2].name)};
                                                            (yyval.field) = field;
                                                          }
#line 2022 "./src/y.tab.c"
    break;

  case 25:
#line 339 "./src/silc.y"
    { (yyvsp[0].field)->idx = ((Field*)(yyvsp[-1].list)->data)->idx + 1;
                                                             if(getField((yyvsp[0].field)->name, (yyvsp[-1].list)))
                                                                yyerror("Duplicate field entry");
                                                             (yyval.list) = addNode((yyvsp[0].field), sizeof(Field), (yyvsp[-1].list));}
#line 2031 "./src/y.tab.c"
    break;

  case 26:
#line 343 "./src/silc.y"
    { (yyvsp[0].field)->idx = 1; (yyval.list) = addNode((yyvsp[0].field), sizeof(Field), NULL); }
#line 2037 "./src/y.tab.c"
    break;

  case 27:
#line 346 "./src/silc.y"
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
#line 2060 "./src/y.tab.c"
    break;

  case 28:
#line 366 "./src/silc.y"
    {char *tmp = (char*)malloc(strlen((yyvsp[-2].name)) + strlen((yyvsp[0].name)) + 1);
                                                            strcpy(tmp, (yyvsp[-2].name)); strcat(tmp, ".");strcat(tmp, (yyvsp[0].name)); (yyval.name) = tmp;}
#line 2067 "./src/y.tab.c"
    break;

  case 29:
#line 368 "./src/silc.y"
    {char *tmp = (char*)malloc(strlen((yyvsp[-2].name)) + strlen((yyvsp[0].name)) + 1);
                                                            strcpy(tmp, (yyvsp[-2].name)); strcat(tmp, ".");strcat(tmp, (yyvsp[0].name)); (yyval.name) = tmp;}
#line 2074 "./src/y.tab.c"
    break;

  case 30:
#line 375 "./src/silc.y"
    {(yyval.no) = createNode(OP, "+", -1, (yyvsp[-2].no), (yyvsp[0].no)); 
                                                        (yyval.no)->vartype = searchType("int", TypeList);}
#line 2081 "./src/y.tab.c"
    break;

  case 31:
#line 378 "./src/silc.y"
    {(yyval.no) = createNode(OP, "-", -1, (yyvsp[-2].no), (yyvsp[0].no)); 
                                                        (yyval.no)->vartype = searchType("int", TypeList);}
#line 2088 "./src/y.tab.c"
    break;

  case 32:
#line 381 "./src/silc.y"
    {(yyval.no) = createNode(OP, "*", -1, (yyvsp[-2].no), (yyvsp[0].no)); 
                                                        (yyval.no)->vartype = searchType("int", TypeList);}
#line 2095 "./src/y.tab.c"
    break;

  case 33:
#line 384 "./src/silc.y"
    {(yyval.no) = createNode(OP, "/", -1, (yyvsp[-2].no), (yyvsp[0].no)); 
                                                        (yyval.no)->vartype = searchType("int", TypeList);}
#line 2102 "./src/y.tab.c"
    break;

  case 34:
#line 387 "./src/silc.y"
    {(yyval.no) = createNode(OP, "<=", -1, (yyvsp[-2].no), (yyvsp[0].no)); 
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2109 "./src/y.tab.c"
    break;

  case 35:
#line 390 "./src/silc.y"
    {(yyval.no) = createNode(OP, ">=", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2116 "./src/y.tab.c"
    break;

  case 36:
#line 393 "./src/silc.y"
    {(yyval.no) = createNode(OP, "==", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2123 "./src/y.tab.c"
    break;

  case 37:
#line 396 "./src/silc.y"
    {(yyval.no) = createNode(OP, "<", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2130 "./src/y.tab.c"
    break;

  case 38:
#line 399 "./src/silc.y"
    {(yyval.no) = createNode(OP, ">", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2137 "./src/y.tab.c"
    break;

  case 39:
#line 402 "./src/silc.y"
    {(yyval.no) = createNode(OP, "!=", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2144 "./src/y.tab.c"
    break;

  case 40:
#line 405 "./src/silc.y"
    {(yyval.no) = createNode(OP, "AND", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2151 "./src/y.tab.c"
    break;

  case 41:
#line 408 "./src/silc.y"
    {(yyval.no) = createNode(OP, "%", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("int", TypeList);}
#line 2158 "./src/y.tab.c"
    break;

  case 42:
#line 411 "./src/silc.y"
    {(yyval.no) = (yyvsp[-1].no);}
#line 2164 "./src/y.tab.c"
    break;

  case 43:
#line 412 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no); (yyval.no)->vartype = searchType("int", TypeList);}
#line 2170 "./src/y.tab.c"
    break;

  case 44:
#line 413 "./src/silc.y"
    {(yyval.no) = createNode(VAR, (yyvsp[0].name), -1, NULL, NULL); (yyval.no)->vartype = getTypeOfSymbol((yyvsp[0].name)); (yyval.no)->varclass = getClassOfSymbol((yyvsp[0].name));}
#line 2176 "./src/y.tab.c"
    break;

  case 45:
#line 414 "./src/silc.y"
    {(yyval.no) = createNode(VAR, (yyvsp[-3].name), -1, (yyvsp[-1].no), NULL);(yyval.no)->vartype = getTypeOfSymbol((yyvsp[-3].name));}
#line 2182 "./src/y.tab.c"
    break;

  case 46:
#line 415 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no); (yyval.no)->vartype = searchType("str", TypeList);}
#line 2188 "./src/y.tab.c"
    break;

  case 47:
#line 416 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2194 "./src/y.tab.c"
    break;

  case 48:
#line 417 "./src/silc.y"
    {(yyval.no) = createNode(VAR, (yyvsp[0].name), -1, NULL, NULL);(yyval.no)->vartype = getTypeOfSymbol((yyvsp[0].name)); (yyval.no)->varclass = getClassOfSymbol((yyvsp[0].name));}
#line 2200 "./src/y.tab.c"
    break;

  case 49:
#line 418 "./src/silc.y"
    {(yyval.no) = createNode(CONST, "\0", 0, NULL, NULL); (yyval.no)->vartype = searchType("null", TypeList);}
#line 2206 "./src/y.tab.c"
    break;

  case 50:
#line 421 "./src/silc.y"
    {(yyval.no) = createNode(RET, "\0", -1, (yyvsp[-1].no), NULL);}
#line 2212 "./src/y.tab.c"
    break;

  case 51:
#line 425 "./src/silc.y"
    {tnode *tmp = createNode(VAR, (yyvsp[-2].name), -1, NULL, NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-2].name));
                                                        (yyval.no) = createNode(READ, "", -1, tmp, NULL);}
#line 2219 "./src/y.tab.c"
    break;

  case 52:
#line 427 "./src/silc.y"
    {tnode *tmp = createNode(VAR, (yyvsp[-5].name), -1, (yyvsp[-3].no), NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-5].name));
                                                        (yyval.no) = createNode(READ, "", -1, tmp, NULL);}
#line 2226 "./src/y.tab.c"
    break;

  case 53:
#line 429 "./src/silc.y"
    {tnode *tmp = createNode(VAR, (yyvsp[-2].name), -1, NULL, NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-2].name));
                                                        (yyval.no) = createNode(READ, "", -1, tmp, NULL);}
#line 2233 "./src/y.tab.c"
    break;

  case 54:
#line 433 "./src/silc.y"
    {(yyval.no) = createNode(INIT, "", -1, NULL, NULL);(yyval.no)->vartype = searchType("int", TypeList);}
#line 2239 "./src/y.tab.c"
    break;

  case 55:
#line 436 "./src/silc.y"
    {(yyval.no) = createNode(ALLOC, "", -1, NULL, NULL); (yyval.no)->vartype = searchType("int", TypeList);}
#line 2245 "./src/y.tab.c"
    break;

  case 56:
#line 439 "./src/silc.y"
    {(yyval.no) = createNode(NEW, (yyvsp[-1].name), -1, NULL, NULL); (yyval.no)->vartype = searchType("int", TypeList);}
#line 2251 "./src/y.tab.c"
    break;

  case 57:
#line 441 "./src/silc.y"
    {tnode *tmp = createNode(VAR, (yyvsp[-2].name), -1, NULL, NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-2].name));tmp->varclass = getClassOfSymbol((yyvsp[-2].name));
                                                        (yyval.no) = createNode(FFREE, "", -1, tmp, NULL);}
#line 2258 "./src/y.tab.c"
    break;

  case 58:
#line 445 "./src/silc.y"
    {(yyval.no) = createNode(WRITE, "", -1, (yyvsp[-2].no), NULL);}
#line 2264 "./src/y.tab.c"
    break;

  case 59:
#line 448 "./src/silc.y"
    { tnode *tmp = createNode(VAR, (yyvsp[-3].name), -1, NULL, NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-3].name)); tmp->varclass = getClassOfSymbol((yyvsp[-3].name));
                                                         (yyval.no) = createNode(ASSN, "", -1, tmp, (yyvsp[-1].no));}
#line 2271 "./src/y.tab.c"
    break;

  case 60:
#line 450 "./src/silc.y"
    {tnode *tmp = createNode(VAR, (yyvsp[-6].name), -1, (yyvsp[-4].no), NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-6].name)); tmp->varclass = getClassOfSymbol((yyvsp[-6].name));
                                                         (yyval.no) = createNode(ASSN, "", -1, tmp, (yyvsp[-1].no));}
#line 2278 "./src/y.tab.c"
    break;

  case 61:
#line 452 "./src/silc.y"
    { tnode *tmp = createNode(VAR, (yyvsp[-3].name), -1, NULL, NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-3].name)); tmp->varclass = getClassOfSymbol((yyvsp[-3].name));
                                                        (yyval.no) = createNode(ASSN, "", -1, tmp, (yyvsp[-1].no));}
#line 2285 "./src/y.tab.c"
    break;

  case 62:
#line 457 "./src/silc.y"
    {
                                                            tnode *tmp = createNode(IF_BODY, "", -1, (yyvsp[-4].no), (yyvsp[-2].no));
                                                            (yyval.no) = createNode(IF, "", -1, (yyvsp[-7].no), tmp);
                                                        }
#line 2294 "./src/y.tab.c"
    break;

  case 63:
#line 461 "./src/silc.y"
    {
                                                            tnode *tmp = createNode(IF_BODY, "", -1, (yyvsp[-2].no), NULL);
                                                            (yyval.no) = createNode(IF, "", -1, (yyvsp[-5].no), tmp);
                                                        }
#line 2303 "./src/y.tab.c"
    break;

  case 64:
#line 467 "./src/silc.y"
    {(yyval.no) = createNode(BREAK, "", -1, NULL, NULL);}
#line 2309 "./src/y.tab.c"
    break;

  case 65:
#line 470 "./src/silc.y"
    {(yyval.no) = createNode(CONT, "", -1, NULL, NULL);}
#line 2315 "./src/y.tab.c"
    break;

  case 66:
#line 474 "./src/silc.y"
    {(yyval.no) = createNode(WHILE, "", -1, (yyvsp[-5].no), (yyvsp[-2].no));}
#line 2321 "./src/y.tab.c"
    break;

  case 67:
#line 477 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2327 "./src/y.tab.c"
    break;

  case 68:
#line 478 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2333 "./src/y.tab.c"
    break;

  case 69:
#line 479 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2339 "./src/y.tab.c"
    break;

  case 70:
#line 480 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2345 "./src/y.tab.c"
    break;

  case 71:
#line 481 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2351 "./src/y.tab.c"
    break;

  case 72:
#line 482 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2357 "./src/y.tab.c"
    break;

  case 73:
#line 483 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2363 "./src/y.tab.c"
    break;

  case 74:
#line 484 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2369 "./src/y.tab.c"
    break;

  case 75:
#line 485 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2375 "./src/y.tab.c"
    break;

  case 76:
#line 488 "./src/silc.y"
    {(yyval.no) = connect((yyvsp[-1].no), (yyvsp[0].no));}
#line 2381 "./src/y.tab.c"
    break;

  case 77:
#line 489 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2387 "./src/y.tab.c"
    break;

  case 78:
#line 493 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2393 "./src/y.tab.c"
    break;

  case 79:
#line 494 "./src/silc.y"
    {(yyval.no) = NULL;}
#line 2399 "./src/y.tab.c"
    break;

  case 80:
#line 497 "./src/silc.y"
    {(yyval.no) = connect((yyvsp[0].no), (yyvsp[-2].no));}
#line 2405 "./src/y.tab.c"
    break;

  case 81:
#line 498 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2411 "./src/y.tab.c"
    break;

  case 82:
#line 500 "./src/silc.y"
    {  GSymbol* sym = (GSymbol*)searchSymbol((yyvsp[-3].name), GSymList);
                                                    if(sym== NULL) yyerror("Function is not declared");
                                                    (yyval.no) = createNode(FUNC, (yyvsp[-3].name), -1, (yyvsp[-1].no), NULL);
                                                    (yyval.no)->vartype = sym->type;}
#line 2420 "./src/y.tab.c"
    break;

  case 83:
#line 504 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2426 "./src/y.tab.c"
    break;

  case 84:
#line 505 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2432 "./src/y.tab.c"
    break;

  case 85:
#line 506 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2438 "./src/y.tab.c"
    break;

  case 86:
#line 507 "./src/silc.y"
    {(yyval.no) = (yyvsp[0].no);}
#line 2444 "./src/y.tab.c"
    break;

  case 87:
#line 509 "./src/silc.y"
    { (yyval.no) = (yyvsp[-1].no);}
#line 2450 "./src/y.tab.c"
    break;

  case 88:
#line 517 "./src/silc.y"
    {(yyval.name) = (yyvsp[0].name);}
#line 2456 "./src/y.tab.c"
    break;

  case 89:
#line 524 "./src/silc.y"
    { (yyval.list) = (yyvsp[-1].list);}
#line 2462 "./src/y.tab.c"
    break;

  case 90:
#line 525 "./src/silc.y"
    { (yyval.list) = NULL;}
#line 2468 "./src/y.tab.c"
    break;

  case 91:
#line 528 "./src/silc.y"
    { (yyval.list) = connectList((yyvsp[-1].list), (yyvsp[0].list), sizeof(LSymbol));}
#line 2474 "./src/y.tab.c"
    break;

  case 92:
#line 529 "./src/silc.y"
    {(yyval.list) = (yyvsp[0].list);}
#line 2480 "./src/y.tab.c"
    break;

  case 93:
#line 533 "./src/silc.y"
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
#line 2500 "./src/y.tab.c"
    break;

  case 94:
#line 549 "./src/silc.y"
    {(yyval.list) = addNode(strdup((yyvsp[0].name)), sizeof((yyvsp[0].name)), (yyvsp[-2].list));}
#line 2506 "./src/y.tab.c"
    break;

  case 95:
#line 550 "./src/silc.y"
    {(yyval.list) = addNode(strdup((yyvsp[0].name)), sizeof((yyvsp[0].name)), NULL);}
#line 2512 "./src/y.tab.c"
    break;

  case 96:
#line 558 "./src/silc.y"
    {}
#line 2518 "./src/y.tab.c"
    break;

  case 97:
#line 559 "./src/silc.y"
    {}
#line 2524 "./src/y.tab.c"
    break;

  case 98:
#line 562 "./src/silc.y"
    {}
#line 2530 "./src/y.tab.c"
    break;

  case 99:
#line 563 "./src/silc.y"
    {}
#line 2536 "./src/y.tab.c"
    break;

  case 100:
#line 567 "./src/silc.y"
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
                                                                GSymList = addNode(tmp, sizeof(GSymbol), GSymList);
                                                                gvars = gvars->next;
                                                                curMemory+=var->size;
                                                            }
                                                        }
#line 2559 "./src/y.tab.c"
    break;

  case 101:
#line 586 "./src/silc.y"
    {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[0].name), .size=1};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), (yyvsp[-2].list));
                                                        }
#line 2568 "./src/y.tab.c"
    break;

  case 102:
#line 591 "./src/silc.y"
    {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[-3].name), .size=(yyvsp[-1].no)->val};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), (yyvsp[-5].list));
                                                        }
#line 2577 "./src/y.tab.c"
    break;

  case 103:
#line 596 "./src/silc.y"
    {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[-3].name), .params=(yyvsp[-1].no), .size=0};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), (yyvsp[-5].list));
                                                        }
#line 2586 "./src/y.tab.c"
    break;

  case 104:
#line 601 "./src/silc.y"
    {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[-3].name), .size=(yyvsp[-1].no)->val};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), NULL);
                                                        }
#line 2595 "./src/y.tab.c"
    break;

  case 105:
#line 606 "./src/silc.y"
    {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[0].name), .size=1};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), NULL);
                                                        }
#line 2604 "./src/y.tab.c"
    break;

  case 106:
#line 611 "./src/silc.y"
    {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[-3].name), .params=(yyvsp[-1].no), .size=0, .isfunc=1};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), NULL);
                                                        }
#line 2613 "./src/y.tab.c"
    break;

  case 107:
#line 621 "./src/silc.y"
    {
                                                         fprintf(out, "%s:\n", (yyvsp[0].no)->varname);
                                                         eval_func((yyvsp[0].no), out);}
#line 2621 "./src/y.tab.c"
    break;

  case 108:
#line 625 "./src/silc.y"
    {
                                                         fprintf(out, "%s:\n", (yyvsp[0].no)->varname);
                                                         eval_func((yyvsp[0].no), out);}
#line 2629 "./src/y.tab.c"
    break;

  case 109:
#line 630 "./src/silc.y"
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
#line 2649 "./src/y.tab.c"
    break;

  case 110:
#line 645 "./src/silc.y"
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
#line 2669 "./src/y.tab.c"
    break;

  case 111:
#line 662 "./src/silc.y"
    {curLvar = addParam((yyvsp[0].no)); (yyval.no) = (yyvsp[0].no); 
                                                        LSymbol* tmp = malloc(sizeof(LSymbol));
                                                        ClassDef *type = malloc(sizeof(ClassDef));;
                                                        *type = (ClassDef){.fields=curClassField, .methods=curClassMethod};
                                                        *tmp = (LSymbol){.name = "self", .class=type };
                                                        curLvar = addNode(tmp, sizeof(LSymbol), curLvar);}
#line 2680 "./src/y.tab.c"
    break;

  case 112:
#line 669 "./src/silc.y"
    { (yyval.no) = connect((yyvsp[0].no), (yyvsp[-2].no));}
#line 2686 "./src/y.tab.c"
    break;

  case 113:
#line 670 "./src/silc.y"
    { (yyval.no) = (yyvsp[0].no);}
#line 2692 "./src/y.tab.c"
    break;

  case 114:
#line 671 "./src/silc.y"
    { (yyval.no) = NULL;}
#line 2698 "./src/y.tab.c"
    break;

  case 115:
#line 674 "./src/silc.y"
    {(yyval.no) = createNode(VAR, (yyvsp[0].name), -1, NULL, NULL); (yyval.no)->vartype = searchType((yyvsp[-1].name), TypeList);}
#line 2704 "./src/y.tab.c"
    break;


#line 2708 "./src/y.tab.c"

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
#line 679 "./src/silc.y"





int main(int argc, char **argv) {

    if(argc < 2) {
        printf("Usage: ./silc <source_file>\n");
        exit(1);
    }
    curMemory = 4096;
    curClassFieldMem = 1;
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
