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
#define YYLAST   630

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  59
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  50
/* YYNRULES -- Number of rules.  */
#define YYNRULES  119
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  293

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
       0,   182,   182,   183,   184,   185,   186,   187,   188,   189,
     197,   212,   213,   216,   261,   336,   342,   343,   345,   360,
     377,   380,   381,   383,   414,   440,   441,   444,   451,   455,
     458,   478,   480,   487,   490,   493,   496,   499,   502,   505,
     508,   511,   514,   517,   520,   523,   524,   525,   526,   527,
     528,   529,   530,   533,   537,   539,   541,   545,   548,   551,
     553,   557,   560,   562,   564,   568,   573,   579,   582,   585,
     589,   590,   591,   592,   593,   594,   595,   596,   597,   600,
     601,   602,   606,   607,   610,   611,   613,   617,   618,   619,
     620,   622,   630,   637,   638,   641,   642,   646,   662,   663,
     671,   672,   675,   676,   680,   700,   705,   710,   716,   721,
     726,   737,   741,   746,   761,   778,   788,   789,   790,   793
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

#define YYPACT_NINF (-156)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       0,    -3,   -36,   -24,    26,    41,  -156,     9,  -156,     0,
    -156,    -2,  -156,  -156,    53,     3,  -156,    51,    -8,    60,
    -156,  -156,    61,  -156,     9,  -156,    74,    26,  -156,    90,
      -2,  -156,   -34,    22,  -156,  -156,    74,    99,   115,   102,
     104,  -156,    -2,    -2,   106,  -156,  -156,    74,   113,  -156,
     117,  -156,    11,   139,  -156,    74,    74,   115,   126,  -156,
      -2,  -156,    74,   146,   151,   160,  -156,   165,    57,   200,
    -156,   185,     4,  -156,    53,    89,  -156,   177,    74,   231,
    -156,   183,  -156,  -156,    74,  -156,    74,   189,  -156,  -156,
    -156,  -156,    40,   212,  -156,   190,    91,     5,   201,   191,
    -156,   193,   186,  -156,  -156,    74,   218,  -156,   202,     7,
    -156,   288,    14,  -156,  -156,   208,  -156,  -156,    47,  -156,
    -156,   210,   211,   209,   220,   214,   215,   216,   217,   219,
     221,   153,  -156,   125,  -156,  -156,  -156,  -156,  -156,  -156,
    -156,  -156,  -156,  -156,  -156,  -156,   176,   225,  -156,   288,
     230,   226,  -156,   224,   162,   162,  -156,  -156,   228,   235,
     239,   240,   242,   162,   162,   162,   243,   162,   162,   162,
     244,   162,   254,  -156,  -156,   176,   288,    16,  -156,  -156,
    -156,    64,  -156,   162,   -19,   394,  -156,   417,  -156,  -156,
     245,   246,   182,    77,   440,   486,   562,  -156,   130,  -156,
     310,   505,   140,  -156,   524,   241,   256,   176,   288,   258,
     162,   463,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   285,   286,   262,  -156,   263,   162,
     270,   271,  -156,   162,  -156,   273,  -156,  -156,  -156,  -156,
     265,   276,   176,   288,   338,  -156,   581,   581,   581,   581,
     581,   581,   581,    20,    20,  -156,  -156,   178,   288,   288,
    -156,  -156,   366,  -156,  -156,  -156,   162,  -156,   266,   281,
     176,  -156,   207,   234,   272,   543,  -156,   268,   283,   288,
     282,   284,   289,  -156,  -156,   275,   261,  -156,  -156,  -156,
    -156,   290,  -156
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     9,     0,    12,     0,
      26,     0,   101,    92,     0,     0,   103,     0,     0,     0,
       1,    11,     0,     6,     0,    25,     0,    92,     7,     0,
       0,   112,   109,     0,   100,   102,     0,     0,     0,     0,
       0,     8,     0,     0,     0,     5,   111,   118,     0,   104,
       0,    29,     0,     0,    15,     0,     0,     0,     0,     3,
       0,     4,   118,     0,     0,   115,   117,     0,   105,     0,
      28,     0,     0,    22,     0,     0,    17,     0,     0,     0,
       2,     0,   119,   110,     0,   108,   118,     0,    30,    27,
      20,    21,     0,     0,    16,     0,     0,     0,     0,     0,
     116,     0,     0,    23,    13,   118,     0,    94,     0,     0,
      96,    81,     0,   107,   106,     0,    14,    99,     0,    93,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,     0,    70,    88,    87,    90,    78,    71,
      72,    73,    75,    76,    74,    80,     0,     0,    77,    81,
       0,     0,    97,     0,     0,     0,    67,    68,     0,     0,
       0,     0,     0,     0,     0,    83,     0,     0,     0,    83,
       0,     0,     0,    79,    91,     0,    81,     0,    98,    49,
      52,    47,    46,     0,    51,     0,    50,     0,    57,    58,
       0,     0,     0,     0,     0,     0,    82,    85,     0,    31,
       0,     0,     0,    32,     0,     0,     0,     0,    81,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    59,     0,     0,
       0,     0,    62,    83,    86,     0,    64,    24,    53,    10,
       0,     0,     0,    81,     0,    45,    43,    40,    41,    39,
      42,    37,    38,    33,    34,    35,    36,    44,    81,    81,
      60,    54,     0,    56,    61,    84,     0,   114,     0,     0,
       0,    48,     0,     0,     0,     0,   113,     0,     0,    81,
       0,     0,     0,    63,    19,     0,     0,    66,    69,    55,
      18,     0,    65
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -156,  -156,    95,   317,    38,  -156,   250,   -53,   279,  -156,
     267,  -156,  -156,   299,  -156,   328,  -111,  -139,  -155,  -156,
    -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,
    -144,  -136,   107,   184,  -107,  -156,    98,   -91,  -156,   247,
    -156,    84,  -156,   337,   280,    68,    59,   -56,  -156,   274
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,     6,     7,     8,    39,    75,    76,    56,    72,
      73,   132,     9,    51,    52,    10,   184,   196,   172,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   197,   198,   186,   148,    29,    98,   109,   110,
     118,    11,    15,    16,    33,    30,    31,    64,    65,    66
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     133,    12,   173,     1,   147,    37,    81,    34,    90,   107,
       2,   119,     1,   175,    17,   185,   187,    97,    47,    97,
     206,   150,    94,    48,   194,   195,    18,     3,   200,   201,
     101,   173,   204,   169,    19,   133,     3,   170,   133,   147,
     207,    20,   147,    94,   211,    21,    38,    13,    27,   115,
       4,   193,   241,    13,    13,    13,   149,    13,   208,   221,
     222,    13,    21,   173,   133,   133,    69,    49,   147,   147,
      50,   244,   242,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   103,   209,   269,    50,    46,
     262,    22,   152,    26,    43,   153,   133,   133,   173,    14,
     147,   147,    46,    32,    23,    36,    28,   270,    42,    86,
      60,     4,    40,    14,    87,   278,   165,    41,    55,    46,
     166,   210,   272,   273,    13,    45,   173,   275,   173,   173,
     230,   133,   133,   170,    53,   147,   147,    59,    61,    13,
      44,    13,   173,   286,    93,    63,   106,   133,   133,    54,
      53,   147,   147,    74,    77,    80,    57,    58,    62,   133,
      63,   133,   133,   147,    67,   147,   147,    68,   133,   179,
      74,   168,   147,    77,   180,   133,    77,   169,   233,   147,
      79,   170,    63,   234,    63,   171,   125,   126,   233,    71,
     121,   122,   128,   237,    77,   108,    82,   123,   124,   164,
     125,   126,   127,    63,    83,   165,   128,   108,    84,   166,
     167,    88,   181,   182,   183,   219,   220,   221,   222,   129,
     130,   121,   122,    85,   279,   280,   131,    95,   123,   124,
      89,   125,   126,   127,    97,   228,    99,   128,   166,   229,
     102,   104,   105,   111,   114,   112,   113,   116,   121,   122,
     129,   130,   117,   281,   156,   123,   124,   131,   125,   126,
     127,   151,   154,   155,   128,   157,   158,   159,   160,   161,
     174,   162,   176,   163,   178,   121,   122,   129,   130,   291,
     177,   188,   123,   124,   131,   125,   126,   127,   189,   190,
     191,   128,   192,   199,   203,   205,   239,   240,   226,   227,
     243,   258,   121,   122,   129,   130,   259,   260,   261,   123,
     124,   131,   125,   126,   127,   263,   264,   268,   128,   266,
     267,   276,   277,   284,   285,   282,    24,   287,    96,   288,
     290,   129,   130,   212,   289,   292,    78,    25,   131,    91,
     265,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,    70,    35,   202,    92,     0,   120,     0,   100,   223,
       0,   212,     0,     0,     0,     0,     0,     0,   235,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,     0,
       0,     0,     0,     0,     0,     0,     0,   223,     0,   212,
       0,     0,     0,     0,     0,     0,   271,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,     0,     0,     0,
       0,     0,     0,     0,     0,   223,     0,   212,     0,     0,
       0,     0,     0,     0,   274,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,     0,     0,     0,     0,     0,
     212,     0,     0,   223,     0,     0,     0,   224,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,     0,     0,
       0,     0,     0,   212,     0,     0,   223,     0,     0,     0,
     225,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,     0,     0,     0,     0,     0,   212,     0,     0,   223,
       0,     0,     0,   231,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,     0,     0,     0,     0,     0,   212,
       0,     0,   223,     0,     0,     0,   245,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,     0,   212,     0,
       0,   232,     0,     0,     0,   223,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,     0,   212,     0,     0,
     236,     0,     0,     0,   223,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,     0,   212,     0,     0,   238,
       0,     0,     0,   223,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,     0,   212,     0,     0,   283,     0,
       0,     0,   223,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,     0,    -1,     0,     0,     0,     0,     0,
       0,   223,    -1,    -1,    -1,    -1,    -1,    -1,   219,   220,
     221,   222,     0,     0,     0,     0,     0,     0,     0,     0,
     223
};

static const yytype_int16 yycheck[] =
{
     111,     4,   146,     3,   111,    13,    62,     4,     4,     4,
      10,     4,     3,   149,    50,   154,   155,     3,    52,     3,
     175,   112,    75,    57,   163,   164,    50,    27,   167,   168,
      86,   175,   171,    52,     8,   146,    27,    56,   149,   146,
     176,     0,   149,    96,   183,     7,    54,    50,    50,   105,
      50,   162,   207,    50,    50,    50,    42,    50,    42,    39,
      40,    50,    24,   207,   175,   176,    55,    45,   175,   176,
      48,   210,   208,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,    45,   177,   242,    48,    30,
     229,     7,    45,     9,    26,    48,   207,   208,   242,     1,
     207,   208,    43,    50,     9,    54,    11,   243,    24,    52,
      42,    50,    52,    15,    57,   270,    52,    22,     3,    60,
      56,    57,   258,   259,    50,    30,   270,   266,   272,   273,
      53,   242,   243,    56,    36,   242,   243,    42,    43,    50,
      50,    50,   286,   279,    55,    47,    55,   258,   259,    50,
      52,   258,   259,    55,    56,    60,    54,    53,    52,   270,
      62,   272,   273,   270,    51,   272,   273,    50,   279,     7,
      72,    46,   279,    75,    12,   286,    78,    52,    48,   286,
      54,    56,    84,    53,    86,     9,    24,    25,    48,    50,
      14,    15,    30,    53,    96,    97,    50,    21,    22,    46,
      24,    25,    26,   105,    53,    52,    30,   109,    48,    56,
      57,    11,    50,    51,    52,    37,    38,    39,    40,    43,
      44,    14,    15,    58,    17,    18,    50,    50,    21,    22,
      45,    24,    25,    26,     3,    53,    53,    30,    56,    57,
      51,    29,    52,    42,    58,    54,    53,    29,    14,    15,
      43,    44,    50,    19,    45,    21,    22,    50,    24,    25,
      26,    53,    52,    52,    30,    45,    52,    52,    52,    52,
      45,    52,    42,    52,    50,    14,    15,    43,    44,    18,
      54,    53,    21,    22,    50,    24,    25,    26,    53,    50,
      50,    30,    50,    50,    50,    41,    55,    41,    53,    53,
      42,    16,    14,    15,    43,    44,    20,    45,    45,    21,
      22,    50,    24,    25,    26,    45,    45,    41,    30,    46,
      55,    55,    41,    55,    41,    53,     9,    45,    78,    45,
      55,    43,    44,    23,    45,    45,    57,     9,    50,    72,
     233,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    52,    15,   169,    74,    -1,   109,    -1,    84,    49,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    58,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    58,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    49,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    49,    -1,    -1,    -1,    53,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    49,    -1,    -1,    -1,
      53,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    49,
      -1,    -1,    -1,    53,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    49,    -1,    -1,    -1,    53,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    23,    -1,
      -1,    45,    -1,    -1,    -1,    49,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    -1,    23,    -1,    -1,
      45,    -1,    -1,    -1,    49,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    -1,    23,    -1,    -1,    45,
      -1,    -1,    -1,    49,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    23,    -1,    -1,    45,    -1,
      -1,    -1,    49,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      49
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    10,    27,    50,    60,    61,    62,    63,    71,
      74,   100,     4,    50,    95,   101,   102,    50,    50,     8,
       0,    63,   100,    61,    62,    74,   100,    50,    61,    95,
     104,   105,    50,   103,     4,   102,    54,    13,    54,    64,
      52,    61,   100,   104,    50,    61,   105,    52,    57,    45,
      48,    72,    73,    95,    50,     3,    67,    54,    53,    61,
     104,    61,    52,    95,   106,   107,   108,    51,    50,    55,
      72,    50,    68,    69,    95,    65,    66,    95,    67,    54,
      61,   106,    50,    53,    48,    58,    52,    57,    11,    45,
       4,    69,   103,    55,    66,    50,    65,     3,    96,    53,
     108,   106,    51,    45,    29,    52,    55,     4,    95,    97,
      98,    42,    54,    53,    58,   106,    29,    50,    99,     4,
      98,    14,    15,    21,    22,    24,    25,    26,    30,    43,
      44,    50,    70,    75,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    93,    94,    42,
      96,    53,    45,    48,    52,    52,    45,    45,    52,    52,
      52,    52,    52,    52,    46,    52,    56,    57,    46,    52,
      56,     9,    77,    89,    45,    90,    42,    54,    50,     7,
      12,    50,    51,    52,    75,    76,    93,    76,    53,    53,
      50,    50,    50,    75,    76,    76,    76,    91,    92,    50,
      76,    76,    92,    50,    76,    41,    77,    90,    42,    96,
      57,    76,    23,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    49,    53,    53,    53,    53,    53,    57,
      53,    53,    45,    48,    53,    58,    45,    53,    45,    55,
      41,    77,    90,    42,    76,    53,    76,    76,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    76,    16,    20,
      45,    45,    76,    45,    45,    91,    46,    55,    41,    77,
      90,    58,    90,    90,    58,    76,    55,    41,    77,    17,
      18,    19,    53,    45,    55,    41,    90,    45,    45,    45,
      55,    18,    45
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    59,    60,    60,    60,    60,    60,    60,    60,    60,
      61,    62,    62,    63,    63,    64,    65,    65,    66,    66,
      67,    68,    68,    69,    70,    71,    71,    72,    73,    73,
      74,    75,    75,    76,    76,    76,    76,    76,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      76,    76,    76,    77,    78,    78,    78,    79,    80,    81,
      82,    83,    84,    84,    84,    85,    85,    86,    87,    88,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    90,
      90,    90,    91,    91,    92,    92,    93,    93,    93,    93,
      93,    94,    95,    96,    96,    97,    97,    98,    99,    99,
     100,   100,   101,   101,   102,   103,   103,   103,   103,   103,
     103,   104,   104,   105,   105,   106,   107,   107,   107,   108
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     5,     4,     4,     3,     2,     2,     3,     1,
      11,     2,     1,     7,     8,     2,     2,     1,    12,    11,
       3,     2,     1,     3,     4,     2,     1,     3,     2,     1,
       6,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     4,     1,
       1,     1,     1,     3,     5,     8,     5,     3,     3,     4,
       5,     5,     4,     7,     4,    10,     8,     2,     2,     8,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       1,     0,     1,     0,     3,     1,     4,     1,     1,     1,
       1,     2,     1,     3,     2,     2,     1,     3,     3,     1,
       3,     2,     2,     1,     3,     3,     6,     6,     4,     1,
       4,     2,     1,    12,    11,     1,     3,     1,     0,     2
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
  case 10:
#line 197 "./src/silc.y"
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
#line 1941 "./src/y.tab.c"
    break;

  case 13:
#line 216 "./src/silc.y"
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
#line 1991 "./src/y.tab.c"
    break;

  case 14:
#line 261 "./src/silc.y"
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
#line 2071 "./src/y.tab.c"
    break;

  case 15:
#line 336 "./src/silc.y"
                         {
                              curClassField = searchClass((yyvsp[0].name), ClassList)->fields;
                              (yyval.name) = (yyvsp[0].name);
                          }
#line 2080 "./src/y.tab.c"
    break;

  case 16:
#line 342 "./src/silc.y"
                                                                {(yyval.list) = connectList((yyvsp[-1].list), (yyvsp[0].list), sizeof(tnode));}
#line 2086 "./src/y.tab.c"
    break;

  case 17:
#line 343 "./src/silc.y"
                                                                {(yyval.list) = (yyvsp[0].list);}
#line 2092 "./src/y.tab.c"
    break;

  case 18:
#line 345 "./src/silc.y"
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
#line 2112 "./src/y.tab.c"
    break;

  case 19:
#line 360 "./src/silc.y"
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
#line 2131 "./src/y.tab.c"
    break;

  case 20:
#line 377 "./src/silc.y"
                                                               {(yyval.list) = (yyvsp[-1].list);}
#line 2137 "./src/y.tab.c"
    break;

  case 21:
#line 380 "./src/silc.y"
                                                                {(yyval.list) = connectList((yyvsp[-1].list), (yyvsp[0].list), sizeof(Field));}
#line 2143 "./src/y.tab.c"
    break;

  case 22:
#line 381 "./src/silc.y"
                                                                    {(yyval.list) = (yyvsp[0].list);}
#line 2149 "./src/y.tab.c"
    break;

  case 23:
#line 383 "./src/silc.y"
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
#line 2183 "./src/y.tab.c"
    break;

  case 24:
#line 414 "./src/silc.y"
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
#line 2211 "./src/y.tab.c"
    break;

  case 25:
#line 440 "./src/silc.y"
                                                          {}
#line 2217 "./src/y.tab.c"
    break;

  case 26:
#line 441 "./src/silc.y"
                                                          {}
#line 2223 "./src/y.tab.c"
    break;

  case 27:
#line 444 "./src/silc.y"
                                                          { Field *field = (Field*)malloc(sizeof(Field));
                                                            Type *type = searchType((yyvsp[-2].name), TypeList);
                                                            *field = type?(Field){.name=(yyvsp[-1].name), .type=type}:(Field){.name=(yyvsp[-1].name), .ndef=(yyvsp[-2].name)};
                                                            (yyval.field) = field;
                                                          }
#line 2233 "./src/y.tab.c"
    break;

  case 28:
#line 451 "./src/silc.y"
                                                           { (yyvsp[0].field)->idx = ((Field*)(yyvsp[-1].list)->data)->idx + 1;
                                                             if(getField((yyvsp[0].field)->name, (yyvsp[-1].list)))
                                                                yyerror("Duplicate field entry");
                                                             (yyval.list) = addNode((yyvsp[0].field), sizeof(Field), (yyvsp[-1].list));}
#line 2242 "./src/y.tab.c"
    break;

  case 29:
#line 455 "./src/silc.y"
                                                           { (yyvsp[0].field)->idx = 1; (yyval.list) = addNode((yyvsp[0].field), sizeof(Field), NULL); }
#line 2248 "./src/y.tab.c"
    break;

  case 30:
#line 458 "./src/silc.y"
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
#line 2271 "./src/y.tab.c"
    break;

  case 31:
#line 478 "./src/silc.y"
                                                            {char *tmp = (char*)malloc(strlen((yyvsp[-2].name)) + strlen((yyvsp[0].name)) + 1);
                                                            strcpy(tmp, (yyvsp[-2].name)); strcat(tmp, ".");strcat(tmp, (yyvsp[0].name)); (yyval.name) = tmp;}
#line 2278 "./src/y.tab.c"
    break;

  case 32:
#line 480 "./src/silc.y"
                                                            {char *tmp = (char*)malloc(strlen((yyvsp[-2].name)) + strlen((yyvsp[0].name)) + 1);
                                                            strcpy(tmp, (yyvsp[-2].name)); strcat(tmp, ".");strcat(tmp, (yyvsp[0].name)); (yyval.name) = tmp;}
#line 2285 "./src/y.tab.c"
    break;

  case 33:
#line 487 "./src/silc.y"
                                                            {(yyval.no) = createNode(OP, "+", -1, (yyvsp[-2].no), (yyvsp[0].no)); 
                                                        (yyval.no)->vartype = searchType("int", TypeList);}
#line 2292 "./src/y.tab.c"
    break;

  case 34:
#line 490 "./src/silc.y"
                                                            {(yyval.no) = createNode(OP, "-", -1, (yyvsp[-2].no), (yyvsp[0].no)); 
                                                        (yyval.no)->vartype = searchType("int", TypeList);}
#line 2299 "./src/y.tab.c"
    break;

  case 35:
#line 493 "./src/silc.y"
                                                                {(yyval.no) = createNode(OP, "*", -1, (yyvsp[-2].no), (yyvsp[0].no)); 
                                                        (yyval.no)->vartype = searchType("int", TypeList);}
#line 2306 "./src/y.tab.c"
    break;

  case 36:
#line 496 "./src/silc.y"
                                                                {(yyval.no) = createNode(OP, "/", -1, (yyvsp[-2].no), (yyvsp[0].no)); 
                                                        (yyval.no)->vartype = searchType("int", TypeList);}
#line 2313 "./src/y.tab.c"
    break;

  case 37:
#line 499 "./src/silc.y"
                                                        {(yyval.no) = createNode(OP, "<=", -1, (yyvsp[-2].no), (yyvsp[0].no)); 
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2320 "./src/y.tab.c"
    break;

  case 38:
#line 502 "./src/silc.y"
                                                        {(yyval.no) = createNode(OP, ">=", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2327 "./src/y.tab.c"
    break;

  case 39:
#line 505 "./src/silc.y"
                                                        {(yyval.no) = createNode(OP, "==", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2334 "./src/y.tab.c"
    break;

  case 40:
#line 508 "./src/silc.y"
                                                        {(yyval.no) = createNode(OP, "<", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2341 "./src/y.tab.c"
    break;

  case 41:
#line 511 "./src/silc.y"
                                                        {(yyval.no) = createNode(OP, ">", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2348 "./src/y.tab.c"
    break;

  case 42:
#line 514 "./src/silc.y"
                                                        {(yyval.no) = createNode(OP, "!=", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2355 "./src/y.tab.c"
    break;

  case 43:
#line 517 "./src/silc.y"
                                                        {(yyval.no) = createNode(OP, "AND", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("bool", TypeList);}
#line 2362 "./src/y.tab.c"
    break;

  case 44:
#line 520 "./src/silc.y"
                                                        {(yyval.no) = createNode(OP, "%", -1, (yyvsp[-2].no), (yyvsp[0].no));
                                                        (yyval.no)->vartype = searchType("int", TypeList);}
#line 2369 "./src/y.tab.c"
    break;

  case 45:
#line 523 "./src/silc.y"
                                                                {(yyval.no) = (yyvsp[-1].no);}
#line 2375 "./src/y.tab.c"
    break;

  case 46:
#line 524 "./src/silc.y"
                                                                    {(yyval.no) = (yyvsp[0].no); (yyval.no)->vartype = searchType("int", TypeList);}
#line 2381 "./src/y.tab.c"
    break;

  case 47:
#line 525 "./src/silc.y"
                                                        {(yyval.no) = createNode(VAR, (yyvsp[0].name), -1, NULL, NULL); (yyval.no)->vartype = getTypeOfSymbol((yyvsp[0].name)); (yyval.no)->varclass = getClassOfSymbol((yyvsp[0].name));}
#line 2387 "./src/y.tab.c"
    break;

  case 48:
#line 526 "./src/silc.y"
                                                        {(yyval.no) = createNode(VAR, (yyvsp[-3].name), -1, (yyvsp[-1].no), NULL);(yyval.no)->vartype = getTypeOfSymbol((yyvsp[-3].name));}
#line 2393 "./src/y.tab.c"
    break;

  case 49:
#line 527 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no); (yyval.no)->vartype = searchType("str", TypeList);}
#line 2399 "./src/y.tab.c"
    break;

  case 50:
#line 528 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2405 "./src/y.tab.c"
    break;

  case 51:
#line 529 "./src/silc.y"
                                                        {(yyval.no) = createNode(VAR, (yyvsp[0].name), -1, NULL, NULL);(yyval.no)->vartype = getTypeOfSymbol((yyvsp[0].name)); (yyval.no)->varclass = getClassOfSymbol((yyvsp[0].name));}
#line 2411 "./src/y.tab.c"
    break;

  case 52:
#line 530 "./src/silc.y"
                                                        {(yyval.no) = createNode(CONST, "\0", 0, NULL, NULL); (yyval.no)->vartype = searchType("null", TypeList);}
#line 2417 "./src/y.tab.c"
    break;

  case 53:
#line 533 "./src/silc.y"
                                                        {(yyval.no) = createNode(RET, "\0", -1, (yyvsp[-1].no), NULL);}
#line 2423 "./src/y.tab.c"
    break;

  case 54:
#line 537 "./src/silc.y"
                                                        {tnode *tmp = createNode(VAR, (yyvsp[-2].name), -1, NULL, NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-2].name));
                                                        (yyval.no) = createNode(READ, "", -1, tmp, NULL);}
#line 2430 "./src/y.tab.c"
    break;

  case 55:
#line 539 "./src/silc.y"
                                                        {tnode *tmp = createNode(VAR, (yyvsp[-5].name), -1, (yyvsp[-3].no), NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-5].name));
                                                        (yyval.no) = createNode(READ, "", -1, tmp, NULL);}
#line 2437 "./src/y.tab.c"
    break;

  case 56:
#line 541 "./src/silc.y"
                                                        {tnode *tmp = createNode(VAR, (yyvsp[-2].name), -1, NULL, NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-2].name));
                                                        (yyval.no) = createNode(READ, "", -1, tmp, NULL);}
#line 2444 "./src/y.tab.c"
    break;

  case 57:
#line 545 "./src/silc.y"
                                                          {(yyval.no) = createNode(INIT, "", -1, NULL, NULL);(yyval.no)->vartype = searchType("int", TypeList);}
#line 2450 "./src/y.tab.c"
    break;

  case 58:
#line 548 "./src/silc.y"
                                                       {(yyval.no) = createNode(ALLOC, "", -1, NULL, NULL); (yyval.no)->vartype = searchType("int", TypeList);}
#line 2456 "./src/y.tab.c"
    break;

  case 59:
#line 551 "./src/silc.y"
                                                        {(yyval.no) = createNode(NEW, (yyvsp[-1].name), -1, NULL, NULL); (yyval.no)->vartype = searchType("int", TypeList);}
#line 2462 "./src/y.tab.c"
    break;

  case 60:
#line 553 "./src/silc.y"
                                                        {tnode *tmp = createNode(VAR, (yyvsp[-2].name), -1, NULL, NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-2].name));tmp->varclass = getClassOfSymbol((yyvsp[-2].name));
                                                        (yyval.no) = createNode(FFREE, "", -1, tmp, NULL);}
#line 2469 "./src/y.tab.c"
    break;

  case 61:
#line 557 "./src/silc.y"
                                                        {(yyval.no) = createNode(WRITE, "", -1, (yyvsp[-2].no), NULL);}
#line 2475 "./src/y.tab.c"
    break;

  case 62:
#line 560 "./src/silc.y"
                                                        { tnode *tmp = createNode(VAR, (yyvsp[-3].name), -1, NULL, NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-3].name)); tmp->varclass = getClassOfSymbol((yyvsp[-3].name));
                                                         (yyval.no) = createNode(ASSN, "", -1, tmp, (yyvsp[-1].no));}
#line 2482 "./src/y.tab.c"
    break;

  case 63:
#line 562 "./src/silc.y"
                                                        {tnode *tmp = createNode(VAR, (yyvsp[-6].name), -1, (yyvsp[-4].no), NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-6].name)); tmp->varclass = getClassOfSymbol((yyvsp[-6].name));
                                                         (yyval.no) = createNode(ASSN, "", -1, tmp, (yyvsp[-1].no));}
#line 2489 "./src/y.tab.c"
    break;

  case 64:
#line 564 "./src/silc.y"
                                                        { tnode *tmp = createNode(VAR, (yyvsp[-3].name), -1, NULL, NULL);tmp->vartype = getTypeOfSymbol((yyvsp[-3].name)); tmp->varclass = getClassOfSymbol((yyvsp[-3].name));
                                                        (yyval.no) = createNode(ASSN, "", -1, tmp, (yyvsp[-1].no));}
#line 2496 "./src/y.tab.c"
    break;

  case 65:
#line 569 "./src/silc.y"
                                                        {
                                                            tnode *tmp = createNode(IF_BODY, "", -1, (yyvsp[-4].no), (yyvsp[-2].no));
                                                            (yyval.no) = createNode(IF, "", -1, (yyvsp[-7].no), tmp);
                                                        }
#line 2505 "./src/y.tab.c"
    break;

  case 66:
#line 573 "./src/silc.y"
                                                        {
                                                            tnode *tmp = createNode(IF_BODY, "", -1, (yyvsp[-2].no), NULL);
                                                            (yyval.no) = createNode(IF, "", -1, (yyvsp[-5].no), tmp);
                                                        }
#line 2514 "./src/y.tab.c"
    break;

  case 67:
#line 579 "./src/silc.y"
                                                        {(yyval.no) = createNode(BREAK, "", -1, NULL, NULL);}
#line 2520 "./src/y.tab.c"
    break;

  case 68:
#line 582 "./src/silc.y"
                                                        {(yyval.no) = createNode(CONT, "", -1, NULL, NULL);}
#line 2526 "./src/y.tab.c"
    break;

  case 69:
#line 586 "./src/silc.y"
                                                        {(yyval.no) = createNode(WHILE, "", -1, (yyvsp[-5].no), (yyvsp[-2].no));}
#line 2532 "./src/y.tab.c"
    break;

  case 70:
#line 589 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2538 "./src/y.tab.c"
    break;

  case 71:
#line 590 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2544 "./src/y.tab.c"
    break;

  case 72:
#line 591 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2550 "./src/y.tab.c"
    break;

  case 73:
#line 592 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2556 "./src/y.tab.c"
    break;

  case 74:
#line 593 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2562 "./src/y.tab.c"
    break;

  case 75:
#line 594 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2568 "./src/y.tab.c"
    break;

  case 76:
#line 595 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2574 "./src/y.tab.c"
    break;

  case 77:
#line 596 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2580 "./src/y.tab.c"
    break;

  case 78:
#line 597 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2586 "./src/y.tab.c"
    break;

  case 79:
#line 600 "./src/silc.y"
                                                        {(yyval.no) = connect((yyvsp[-1].no), (yyvsp[0].no));}
#line 2592 "./src/y.tab.c"
    break;

  case 80:
#line 601 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2598 "./src/y.tab.c"
    break;

  case 81:
#line 602 "./src/silc.y"
                                                        {(yyval.no) = NULL;}
#line 2604 "./src/y.tab.c"
    break;

  case 82:
#line 606 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2610 "./src/y.tab.c"
    break;

  case 83:
#line 607 "./src/silc.y"
                                                        {(yyval.no) = NULL;}
#line 2616 "./src/y.tab.c"
    break;

  case 84:
#line 610 "./src/silc.y"
                                                        {(yyval.no) = connect((yyvsp[0].no), (yyvsp[-2].no));}
#line 2622 "./src/y.tab.c"
    break;

  case 85:
#line 611 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2628 "./src/y.tab.c"
    break;

  case 86:
#line 613 "./src/silc.y"
                                                 {  GSymbol* sym = (GSymbol*)searchSymbol((yyvsp[-3].name), GSymList);
                                                    if(sym== NULL) yyerror("Function is not declared");
                                                    (yyval.no) = createNode(FUNC, (yyvsp[-3].name), -1, (yyvsp[-1].no), NULL);
                                                    (yyval.no)->vartype = sym->type;}
#line 2637 "./src/y.tab.c"
    break;

  case 87:
#line 617 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2643 "./src/y.tab.c"
    break;

  case 88:
#line 618 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2649 "./src/y.tab.c"
    break;

  case 89:
#line 619 "./src/silc.y"
                                                         {(yyval.no) = (yyvsp[0].no);}
#line 2655 "./src/y.tab.c"
    break;

  case 90:
#line 620 "./src/silc.y"
                                                        {(yyval.no) = (yyvsp[0].no);}
#line 2661 "./src/y.tab.c"
    break;

  case 91:
#line 622 "./src/silc.y"
                                                        { (yyval.no) = (yyvsp[-1].no);}
#line 2667 "./src/y.tab.c"
    break;

  case 92:
#line 630 "./src/silc.y"
                                                       {(yyval.name) = (yyvsp[0].name);}
#line 2673 "./src/y.tab.c"
    break;

  case 93:
#line 637 "./src/silc.y"
                                                        { (yyval.list) = (yyvsp[-1].list);}
#line 2679 "./src/y.tab.c"
    break;

  case 94:
#line 638 "./src/silc.y"
                                                        { (yyval.list) = NULL;}
#line 2685 "./src/y.tab.c"
    break;

  case 95:
#line 641 "./src/silc.y"
                                                        { (yyval.list) = connectList((yyvsp[-1].list), (yyvsp[0].list), sizeof(LSymbol));}
#line 2691 "./src/y.tab.c"
    break;

  case 96:
#line 642 "./src/silc.y"
                                                        {(yyval.list) = (yyvsp[0].list);}
#line 2697 "./src/y.tab.c"
    break;

  case 97:
#line 646 "./src/silc.y"
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
#line 2717 "./src/y.tab.c"
    break;

  case 98:
#line 662 "./src/silc.y"
                                                        {(yyval.list) = addNode(strdup((yyvsp[0].name)), sizeof((yyvsp[0].name)), (yyvsp[-2].list));}
#line 2723 "./src/y.tab.c"
    break;

  case 99:
#line 663 "./src/silc.y"
                                                        {(yyval.list) = addNode(strdup((yyvsp[0].name)), sizeof((yyvsp[0].name)), NULL);}
#line 2729 "./src/y.tab.c"
    break;

  case 100:
#line 671 "./src/silc.y"
                                                        {}
#line 2735 "./src/y.tab.c"
    break;

  case 101:
#line 672 "./src/silc.y"
                                                        {}
#line 2741 "./src/y.tab.c"
    break;

  case 102:
#line 675 "./src/silc.y"
                                                        {}
#line 2747 "./src/y.tab.c"
    break;

  case 103:
#line 676 "./src/silc.y"
                                                        {}
#line 2753 "./src/y.tab.c"
    break;

  case 104:
#line 680 "./src/silc.y"
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
#line 2777 "./src/y.tab.c"
    break;

  case 105:
#line 700 "./src/silc.y"
                                                        {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[0].name), .size=1};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), (yyvsp[-2].list));
                                                        }
#line 2786 "./src/y.tab.c"
    break;

  case 106:
#line 705 "./src/silc.y"
                                                        {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[-3].name), .size=(yyvsp[-1].no)->val};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), (yyvsp[-5].list));
                                                        }
#line 2795 "./src/y.tab.c"
    break;

  case 107:
#line 710 "./src/silc.y"
                                                        {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[-3].name), .params=(yyvsp[-1].no), .size=0};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), (yyvsp[-5].list));
                                                         curLvar = NULL;
                                                        }
#line 2805 "./src/y.tab.c"
    break;

  case 108:
#line 716 "./src/silc.y"
                                                        {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[-3].name), .size=(yyvsp[-1].no)->val};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), NULL);
                                                        }
#line 2814 "./src/y.tab.c"
    break;

  case 109:
#line 721 "./src/silc.y"
                                                        {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[0].name), .size=1};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), NULL);
                                                        }
#line 2823 "./src/y.tab.c"
    break;

  case 110:
#line 726 "./src/silc.y"
                                                        {GVariable* tmp = (GVariable*)malloc(sizeof(GVariable));
                                                         *tmp = (GVariable){.name=(yyvsp[-3].name), .params=(yyvsp[-1].no), .size=0, .isfunc=1};
                                                         (yyval.list) = addNode(tmp, sizeof(GVariable), NULL);
                                                         curLvar = NULL;
                                                        }
#line 2833 "./src/y.tab.c"
    break;

  case 111:
#line 737 "./src/silc.y"
                                                        {
                                                         fprintf(out, "%s:\n", (yyvsp[0].no)->varname);
                                                         eval_func((yyvsp[0].no), out);}
#line 2841 "./src/y.tab.c"
    break;

  case 112:
#line 741 "./src/silc.y"
                                                        {
                                                         fprintf(out, "%s:\n", (yyvsp[0].no)->varname);
                                                         eval_func((yyvsp[0].no), out);}
#line 2849 "./src/y.tab.c"
    break;

  case 113:
#line 746 "./src/silc.y"
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
#line 2869 "./src/y.tab.c"
    break;

  case 114:
#line 761 "./src/silc.y"
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
#line 2889 "./src/y.tab.c"
    break;

  case 115:
#line 778 "./src/silc.y"
                                                        {curLvar = addParam((yyvsp[0].no)); (yyval.no) = (yyvsp[0].no); 
                                                        LinkedList* t = curLvar;
                                                        LSymbol* tmp = malloc(sizeof(LSymbol));
                                                        ClassDef *type = malloc(sizeof(ClassDef));;
                                                        *type = (ClassDef){.fields=curClassField, .methods=curClassMethod};
                                                        *tmp = (LSymbol){.name = "self", .class=type };
                                                        curLvar = addNode(tmp, sizeof(LSymbol), curLvar);
                                                        t = curLvar;
                                                        }
#line 2903 "./src/y.tab.c"
    break;

  case 116:
#line 788 "./src/silc.y"
                                                        { (yyval.no) = connect((yyvsp[0].no), (yyvsp[-2].no));}
#line 2909 "./src/y.tab.c"
    break;

  case 117:
#line 789 "./src/silc.y"
                                                        { (yyval.no) = (yyvsp[0].no);}
#line 2915 "./src/y.tab.c"
    break;

  case 118:
#line 790 "./src/silc.y"
                                                        { (yyval.no) = NULL;}
#line 2921 "./src/y.tab.c"
    break;

  case 119:
#line 793 "./src/silc.y"
                                                        {(yyval.no) = createNode(VAR, (yyvsp[0].name), -1, NULL, NULL); (yyval.no)->vartype = searchType((yyvsp[-1].name), TypeList);}
#line 2927 "./src/y.tab.c"
    break;


#line 2931 "./src/y.tab.c"

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
#line 798 "./src/silc.y"





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
