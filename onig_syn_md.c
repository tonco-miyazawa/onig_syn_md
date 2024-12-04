﻿/*
 * onig_syn_md.c
 * Copyright (c) 2024  K.Kosako
 *
 * Oniguruma OWner: K.Kosako       https://github.com/kkos/oniguruma
 * SYNTAX.md      : seanofw        https://github.com/seanofw
 * onig_syn_md.c  : tonco-miyazawa https://github.com/tonco-miyazawa
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "oniguruma.h"

#define ONIG_SYN_MD_VERSION_INT           (00001)
#define TOTAL_NUM_OF_BITS                 (32)

#define PRINT_SEPARATOR                   (printf("===================================================\n"))

#define INPUT_SYNTAX(syn, abb, set_in)    { (syn), (#syn), (abb), (set_in) }
#define INPUT_FLAG(arg)                   { (arg), (#arg) }



/************************************* Settings *********************************************/
/* NOW_MODE
1: (OP)
2: (OP2)
3: (BEHAVIOR) */
#define NOW_MODE                          (1)


/*  #define PRINT_UNDEFINED_FLAG  */

/*  #define PRINT_SYNTAX_FORWARD_ORDER  */

/*  #define USE_YOUR_OWN_SYNTAX  */

/*  #define PRINT_DEBUG_INFO  */

#define PRINT_VERSION_INFO
#define PRINT_SET_IN_INFO
#define PRINT_TABLE_INFO
#define WARN_UNDEFINED_FLAG_USED
/************************************* Settings *********************************************/


/************************ Switch between OP, OP2, BEHAVIOR **********************************/
#if   NOW_MODE == 1

#define SYNTAX_MEMBER_NAME                ("op")
#define TITLE_STRING                      ("Group One Flags (op)")
#define SYNTAX_MEMBER(syn)                ((syn)->op)
#define IS_SYNTAX_MEMBER(syn, opm)        (((syn)->op & (opm)) != 0)

#elif NOW_MODE == 2

#define SYNTAX_MEMBER_NAME                ("op2")
#define TITLE_STRING                      ("Group Two Flags (op2)")
#define SYNTAX_MEMBER(syn)                ((syn)->op2)
#define IS_SYNTAX_MEMBER(syn, opm)        (((syn)->op2 & (opm)) != 0)

#elif NOW_MODE == 3

#define SYNTAX_MEMBER_NAME                ("behavior")
#define TITLE_STRING                      ("Syntax Flags (syn)")
#define SYNTAX_MEMBER(syn)                ((syn)->behavior)
#define IS_SYNTAX_MEMBER(syn, opm)        (((syn)->behavior & (opm)) != 0)

#else
#error "Check 'NOW_MODE' value."
#endif
/************************ Switch between OP, OP2, BEHAVIOR **********************************/


/*********************************** Your own syntax ****************************************/
#ifdef USE_YOUR_OWN_SYNTAX
static OnigSyntaxType OnigSyntaxYourOwn;
#define ONIG_SYNTAX_YOUROWN    (&OnigSyntaxYourOwn)

static OnigSyntaxType OnigSyntaxYourOwn = {
    0xf0f0f0f0      /*  Group One Flags (op)   */
  , 0xffff0000      /*  Group Two Flags (op2)  */
  , 0x00ff00ff      /*  Syntax Flags    (syn)  */
  , ONIG_OPTION_NONE
  ,
  {
      (OnigCodePoint )'\\'                       /* esc */
    , (OnigCodePoint )ONIG_INEFFECTIVE_META_CHAR /* anychar '.'  */
    , (OnigCodePoint )ONIG_INEFFECTIVE_META_CHAR /* anytime '*'  */
    , (OnigCodePoint )ONIG_INEFFECTIVE_META_CHAR /* zero or one time '?' */
    , (OnigCodePoint )ONIG_INEFFECTIVE_META_CHAR /* one or more time '+' */
    , (OnigCodePoint )ONIG_INEFFECTIVE_META_CHAR /* anychar anytime */
  }
};
#endif
/*********************************** Your own syntax ****************************************/


/************************************** syntax data *****************************************/
typedef struct {
    OnigSyntaxType* syn;
    char *name;
    char *abb;
    char *set_in;
} syn_data;


static syn_data  syn_data_list[] =
{
   /*  INPUT_SYNTAX(syn, abb, set_in)  ===>   { (syn), (#syn), (abb), (set_in) }  */

    INPUT_SYNTAX( ONIG_SYNTAX_ASIS           , "ASIS" , "ASIS" )
  , INPUT_SYNTAX( ONIG_SYNTAX_POSIX_BASIC    , "PosB" , "PosixBasic" )
  , INPUT_SYNTAX( ONIG_SYNTAX_POSIX_EXTENDED , "PosEx", "PosixExtended")
  , INPUT_SYNTAX( ONIG_SYNTAX_EMACS          , "Emacs", "Emacs")
  , INPUT_SYNTAX( ONIG_SYNTAX_GREP           , "Grep" , "Grep" )
  , INPUT_SYNTAX( ONIG_SYNTAX_GNU_REGEX      , "Gnu"  , "GnuRegex" )
  , INPUT_SYNTAX( ONIG_SYNTAX_JAVA           , "Java" , "Java" )
  , INPUT_SYNTAX( ONIG_SYNTAX_PERL           , "Perl" , "Perl" )
  , INPUT_SYNTAX( ONIG_SYNTAX_PERL_NG        , "PeNG" , "Perl_NG" )
  , INPUT_SYNTAX( ONIG_SYNTAX_RUBY           , "Ruby" , "Ruby" )
  , INPUT_SYNTAX( ONIG_SYNTAX_PYTHON         , "Pythn", "Python")
  , INPUT_SYNTAX( ONIG_SYNTAX_ONIGURUMA      , "Onig" , "Oniguruma" )

#ifdef USE_YOUR_OWN_SYNTAX
  , INPUT_SYNTAX( ONIG_SYNTAX_YOUROWN        , "Your" , "YourOwn" )
#endif
};

static const int num_of_syntax_types = (sizeof syn_data_list /sizeof syn_data_list[0]);


static void print_syn_data_list()
{
  int y;
  PRINT_SEPARATOR;
  for (y=0; y < num_of_syntax_types; y++ )
  {
    printf( "\nsyn_data_list[%d]\n", y);
    printf( "name='%s'\n"   , syn_data_list[y].name);
    printf( "abb='%s'\n"    , syn_data_list[y].abb);
    printf( "syn->");
    printf( SYNTAX_MEMBER_NAME );
    printf( "=0x%x\n", SYNTAX_MEMBER(syn_data_list[y].syn) );
  };
  return ;
}
/************************************** syntax data *****************************************/


/************************************** flag data *******************************************/
typedef struct {
    unsigned int num;
    char *name;
} flag_data;


/*  OP  */
#if NOW_MODE == 1
static flag_data  flag_data_list[] =
{

/*  The following are no need to sort them in bit order.  */

/*  INPUT_FLAG(arg)  ===>    { (arg), (#arg) }   */

    INPUT_FLAG( ONIG_SYN_OP_VARIABLE_META_CHARACTERS )
  , INPUT_FLAG( ONIG_SYN_OP_DOT_ANYCHAR )
  , INPUT_FLAG( ONIG_SYN_OP_ASTERISK_ZERO_INF )
  , INPUT_FLAG( ONIG_SYN_OP_ESC_ASTERISK_ZERO_INF )
  , INPUT_FLAG( ONIG_SYN_OP_PLUS_ONE_INF )
  , INPUT_FLAG( ONIG_SYN_OP_ESC_PLUS_ONE_INF )
  , INPUT_FLAG( ONIG_SYN_OP_QMARK_ZERO_ONE )
  , INPUT_FLAG( ONIG_SYN_OP_ESC_QMARK_ZERO_ONE )
  , INPUT_FLAG( ONIG_SYN_OP_BRACE_INTERVAL )
  , INPUT_FLAG( ONIG_SYN_OP_ESC_BRACE_INTERVAL )
  , INPUT_FLAG( ONIG_SYN_OP_VBAR_ALT )
  , INPUT_FLAG( ONIG_SYN_OP_ESC_VBAR_ALT )
  , INPUT_FLAG( ONIG_SYN_OP_LPAREN_SUBEXP )
  , INPUT_FLAG( ONIG_SYN_OP_ESC_LPAREN_SUBEXP )
  , INPUT_FLAG( ONIG_SYN_OP_ESC_AZ_BUF_ANCHOR )
  , INPUT_FLAG( ONIG_SYN_OP_ESC_CAPITAL_G_BEGIN_ANCHOR )
  , INPUT_FLAG( ONIG_SYN_OP_DECIMAL_BACKREF )
  , INPUT_FLAG( ONIG_SYN_OP_BRACKET_CC )
  , INPUT_FLAG( ONIG_SYN_OP_ESC_W_WORD )
  , INPUT_FLAG( ONIG_SYN_OP_ESC_LTGT_WORD_BEGIN_END )
  , INPUT_FLAG( ONIG_SYN_OP_ESC_B_WORD_BOUND )
  , INPUT_FLAG( ONIG_SYN_OP_ESC_S_WHITE_SPACE )
  , INPUT_FLAG( ONIG_SYN_OP_ESC_D_DIGIT )
  , INPUT_FLAG( ONIG_SYN_OP_LINE_ANCHOR )
  , INPUT_FLAG( ONIG_SYN_OP_POSIX_BRACKET )
  , INPUT_FLAG( ONIG_SYN_OP_QMARK_NON_GREEDY )
  , INPUT_FLAG( ONIG_SYN_OP_ESC_CONTROL_CHARS )
  , INPUT_FLAG( ONIG_SYN_OP_ESC_C_CONTROL )
  , INPUT_FLAG( ONIG_SYN_OP_ESC_OCTAL3 )
  , INPUT_FLAG( ONIG_SYN_OP_ESC_X_HEX2 )
  , INPUT_FLAG( ONIG_SYN_OP_ESC_X_BRACE_HEX8 )
  , INPUT_FLAG( ONIG_SYN_OP_ESC_O_BRACE_OCTAL )
};
#endif


/*  OP2 */
#if NOW_MODE == 2
static flag_data  flag_data_list[] =
{
    INPUT_FLAG( ONIG_SYN_OP2_ESC_CAPITAL_Q_QUOTE )
  , INPUT_FLAG( ONIG_SYN_OP2_QMARK_GROUP_EFFECT )
  , INPUT_FLAG( ONIG_SYN_OP2_OPTION_PERL )
  , INPUT_FLAG( ONIG_SYN_OP2_OPTION_RUBY )
  , INPUT_FLAG( ONIG_SYN_OP2_PLUS_POSSESSIVE_REPEAT )
  , INPUT_FLAG( ONIG_SYN_OP2_PLUS_POSSESSIVE_INTERVAL )
  , INPUT_FLAG( ONIG_SYN_OP2_CCLASS_SET_OP )
  , INPUT_FLAG( ONIG_SYN_OP2_QMARK_LT_NAMED_GROUP )
  , INPUT_FLAG( ONIG_SYN_OP2_ESC_K_NAMED_BACKREF )
  , INPUT_FLAG( ONIG_SYN_OP2_ESC_G_SUBEXP_CALL )
  , INPUT_FLAG( ONIG_SYN_OP2_ATMARK_CAPTURE_HISTORY )
  , INPUT_FLAG( ONIG_SYN_OP2_ESC_CAPITAL_C_BAR_CONTROL )
  , INPUT_FLAG( ONIG_SYN_OP2_ESC_CAPITAL_M_BAR_META )
  , INPUT_FLAG( ONIG_SYN_OP2_ESC_V_VTAB )
  , INPUT_FLAG( ONIG_SYN_OP2_ESC_U_HEX4 )
  , INPUT_FLAG( ONIG_SYN_OP2_ESC_GNU_BUF_ANCHOR )
  , INPUT_FLAG( ONIG_SYN_OP2_ESC_P_BRACE_CHAR_PROPERTY )
  , INPUT_FLAG( ONIG_SYN_OP2_ESC_P_BRACE_CIRCUMFLEX_NOT )
  /*  , INPUT_FLAG( ONIG_SYN_OP2_CHAR_PROPERTY_PREFIX_IS )  */
  , INPUT_FLAG( ONIG_SYN_OP2_ESC_H_XDIGIT )
  , INPUT_FLAG( ONIG_SYN_OP2_INEFFECTIVE_ESCAPE )
  , INPUT_FLAG( ONIG_SYN_OP2_QMARK_LPAREN_IF_ELSE )
  , INPUT_FLAG( ONIG_SYN_OP2_ESC_CAPITAL_K_KEEP )
  , INPUT_FLAG( ONIG_SYN_OP2_ESC_CAPITAL_R_GENERAL_NEWLINE )
  , INPUT_FLAG( ONIG_SYN_OP2_ESC_CAPITAL_N_O_SUPER_DOT )
  , INPUT_FLAG( ONIG_SYN_OP2_QMARK_TILDE_ABSENT_GROUP )
  /*  , INPUT_FLAG( ONIG_SYN_OP2_ESC_X_Y_GRAPHEME_CLUSTER ) */   /* obsoleted: use next */
  , INPUT_FLAG( ONIG_SYN_OP2_ESC_X_Y_TEXT_SEGMENT )
  , INPUT_FLAG( ONIG_SYN_OP2_QMARK_PERL_SUBEXP_CALL )
  , INPUT_FLAG( ONIG_SYN_OP2_QMARK_BRACE_CALLOUT_CONTENTS )
  , INPUT_FLAG( ONIG_SYN_OP2_ASTERISK_CALLOUT_NAME )
  , INPUT_FLAG( ONIG_SYN_OP2_OPTION_ONIGURUMA )
  , INPUT_FLAG( ONIG_SYN_OP2_QMARK_CAPITAL_P_NAME )
};
#endif


/*  BEHAVIOR  */
#if NOW_MODE == 3
static flag_data  flag_data_list[] =
{
    INPUT_FLAG( ONIG_SYN_CONTEXT_INDEP_ANCHORS )
  , INPUT_FLAG( ONIG_SYN_CONTEXT_INDEP_REPEAT_OPS )
  , INPUT_FLAG( ONIG_SYN_CONTEXT_INVALID_REPEAT_OPS )
  , INPUT_FLAG( ONIG_SYN_ALLOW_UNMATCHED_CLOSE_SUBEXP )
  , INPUT_FLAG( ONIG_SYN_ALLOW_INVALID_INTERVAL )
  , INPUT_FLAG( ONIG_SYN_ALLOW_INTERVAL_LOW_ABBREV )
  , INPUT_FLAG( ONIG_SYN_STRICT_CHECK_BACKREF )
  , INPUT_FLAG( ONIG_SYN_DIFFERENT_LEN_ALT_LOOK_BEHIND )
  , INPUT_FLAG( ONIG_SYN_CAPTURE_ONLY_NAMED_GROUP )
  , INPUT_FLAG( ONIG_SYN_ALLOW_MULTIPLEX_DEFINITION_NAME )
  , INPUT_FLAG( ONIG_SYN_FIXED_INTERVAL_IS_GREEDY_ONLY )
  , INPUT_FLAG( ONIG_SYN_ISOLATED_OPTION_CONTINUE_BRANCH )
  , INPUT_FLAG( ONIG_SYN_VARIABLE_LEN_LOOK_BEHIND )
  , INPUT_FLAG( ONIG_SYN_PYTHON )
  , INPUT_FLAG( ONIG_SYN_WHOLE_OPTIONS )
  , INPUT_FLAG( ONIG_SYN_BRE_ANCHOR_AT_EDGE_OF_SUBEXP )

  /* syntax (behavior) in char class [...] */
  , INPUT_FLAG( ONIG_SYN_NOT_NEWLINE_IN_NEGATIVE_CC )
  , INPUT_FLAG( ONIG_SYN_BACKSLASH_ESCAPE_IN_CC )
  , INPUT_FLAG( ONIG_SYN_ALLOW_EMPTY_RANGE_IN_CC )
  , INPUT_FLAG( ONIG_SYN_ALLOW_DOUBLE_RANGE_OP_IN_CC )
  , INPUT_FLAG( ONIG_SYN_ALLOW_INVALID_CODE_END_OF_RANGE_IN_CC )
  , INPUT_FLAG( ONIG_SYN_ALLOW_CHAR_TYPE_FOLLOWED_BY_MINUS_IN_CC )

  /* syntax (behavior) warning */
  , INPUT_FLAG( ONIG_SYN_WARN_CC_OP_NOT_ESCAPED )
  , INPUT_FLAG( ONIG_SYN_WARN_REDUNDANT_NESTED_REPEAT )
};
#endif


static const int num_of_flags = (sizeof flag_data_list /sizeof flag_data_list[0]);

static int flag_name_max_len = 0;


static int flag_name_max_len_int()
{
  int i, n, max_len;
  max_len = 0;

  for (i=0; i < num_of_flags; i++)
  {
    n = strlen(flag_data_list[i].name);
    if ( max_len < n )
    {
      max_len = n;
    }
  }

  /*  Debug: printf("max_len=%d\n", max_len);  */
  return max_len;
}


static int convert_bit_shift_num_to_flag_data_list_element( int bit_shift_num )
{
  int i;
  for ( i=0; i < num_of_flags; i++ )
  {
    if ( flag_data_list[i].num == ( 1U << bit_shift_num) ){

#ifdef PRINT_DEBUG_INFO
      printf("(1U << %d) ===> flag_data_list[%d]\n", bit_shift_num, i );
#endif
      return i;
    }
  }

#ifdef PRINT_DEBUG_INFO
  printf("(1U << %d) ===>     ( none )\n", bit_shift_num );
#endif

  /*  If not found, returns "-1". This is not Error. */
  return -1;
}


static int convert_num_to_bit_shift_num(unsigned int arg_num){
  int x;
  for ( x=0; (x < TOTAL_NUM_OF_BITS); x++ )
  {
     if ( arg_num == (1U << x) )
     {
       return x;
     }
  }

  /* Error */
  printf("<Error:line%d> '%x' is not bit flag.\n", __LINE__, arg_num );
  exit(-1);
  return -1;
}


static int check_flag_data_duplication()
{
  int i, shift_num;
  unsigned int used_bits = 0;

  for ( i=0; i < num_of_flags; i++ )
  {
    if ( (used_bits & flag_data_list[i].num) != 0 ){

       shift_num = convert_num_to_bit_shift_num(flag_data_list[i].num);

       PRINT_SEPARATOR;
       fprintf(stderr, "\n<Error:line%d>  The following bit has already been used.\n", __LINE__ );
       fprintf(stderr, "flag_data_list[%d] : '%s' ", i, flag_data_list[i].name);
       fprintf(stderr, "(1U << %d)\n", shift_num);

#ifndef PRINT_DEBUG_INFO
       fprintf(stderr, "\nPlease use '#define PRINT_DEBUG_INFO'.\n");
#endif
       exit(-1);
       return -1;
    }
    used_bits |= flag_data_list[i].num;
  }
  return 0;
}


static void print_flag_data_list()
{
  int i, shift_num;

  PRINT_SEPARATOR;
  for (i=0; i < num_of_flags; i++ )
  {
    shift_num = convert_num_to_bit_shift_num( flag_data_list[i].num );

    printf( "\nflag_data_list[%d]\n", i);
    printf( "name='%s' "   , flag_data_list[i].name);
    printf( "(1U << %d)\n" , shift_num);
    printf( "num=0x%x\n"   , flag_data_list[i].num);
  }
  return ;
}
/************************************** flag data *******************************************/


/************************************** print table *****************************************/
static void print_table_head()
{
  int i, y;

  printf("\n### ");
  printf( TITLE_STRING );
  printf("\n\n| ID    | Option");
  for (i=0; i < (flag_name_max_len - 1); i++){ printf(" "); }
  printf("|");

  /*  ex. print "PeNG "  */
#ifdef PRINT_SYNTAX_FORWARD_ORDER
  for ( y=0; y < num_of_syntax_types; y++)
#else
  for ( y=num_of_syntax_types -1; y > -1; y--)
#endif
  {
    printf(" %-5.5s |", syn_data_list[y].abb );
  }

  printf("\n| ----- | ");
  for (i=0; i < (flag_name_max_len + 4); i++){ printf("-"); }
  printf(" |");

  for ( y=0; y < num_of_syntax_types; y++ ){ printf(" ----- |"); }
  printf("\n");
  return ;
}


static void print_table_body_one_line( int shift_num )
{
  int i, y, elem, name_chars;

  elem = convert_bit_shift_num_to_flag_data_list_element(shift_num);

  printf("| %2d    | ", shift_num);

  if (elem < 0){
    name_chars = printf("     `( Undefined )`");    /*  elem == -1  */
  } else {
    name_chars = printf("`%s`", flag_data_list[elem].name);
  }

  for ( i=0; i < (flag_name_max_len - name_chars + 5); i++){ printf(" "); }
  printf("|");

  /*  ex. print  ' Yes   |'   */
#ifdef PRINT_SYNTAX_FORWARD_ORDER
  for ( y=0; y < num_of_syntax_types; y++)
#else
  for ( y=num_of_syntax_types -1; y > -1; y--)
#endif
  {
    if ( IS_SYNTAX_MEMBER(syn_data_list[y].syn, ( 1U << shift_num)) )
    {
      printf(" Yes   |");
    } else {
      printf(" -     |");
    }
  }
  printf("\n");
  return ;
}


static void print_table_body()
{
  int x, elem;
  for ( x=0; x < TOTAL_NUM_OF_BITS; x++)
  {
    elem = convert_bit_shift_num_to_flag_data_list_element( x );

    if (elem < 0)
    {
#if !(defined( PRINT_UNDEFINED_FLAG ))
      continue;
#endif
    }
    print_table_body_one_line(x);
  }
  return ;
}

/************************************** print table *****************************************/


/************************************* print Set_in *****************************************/
static void print_set_in_one_line(int shift_num)
{
  int y, elem, count;

  elem = convert_bit_shift_num_to_flag_data_list_element(shift_num);

  if (elem < 0)
  {
    printf("### %d.       ( Undefined )\n", shift_num );    /*  elem == -1  */
  } else {
    printf("### %d. %s\n", shift_num, flag_data_list[elem].name );
  }

  count = 0;
  printf("_Set in: ");
#ifdef PRINT_SYNTAX_FORWARD_ORDER
  for ( y=0; y < num_of_syntax_types; y++)
#else
  for ( y=num_of_syntax_types -1; y > -1; y--)
#endif
  {
    if ( IS_SYNTAX_MEMBER(syn_data_list[y].syn, (1U << shift_num)) )
    {
      if (count > 0){ printf(", "); };
      printf("%s", syn_data_list[y].set_in);
      count++;
    }
  }   /* for y */

  if (count==0){ printf("none"); };

  printf("_\n\n");
  return ;
}


static void print_set_in()
{
  int x, elem;

  PRINT_SEPARATOR;
  printf("The following are 'Set in' for oniguruma/doc/SYNTAX.md\n\n## ");
  printf( TITLE_STRING );
  printf("\n\n");
  for ( x=0; x < TOTAL_NUM_OF_BITS; x++)
  {
    elem = convert_bit_shift_num_to_flag_data_list_element( x );

    if (elem < 0)
    {
#if !(defined( PRINT_UNDEFINED_FLAG ))
      continue;
#endif
    }

    print_set_in_one_line(x);
  }
  return ;
}

/************************************* print Set_in *****************************************/


/**************************************    main()   *****************************************/

static void print_version()
{
  printf( "[ onig_syn_md.c ver.%05d ] ", ONIG_SYN_MD_VERSION_INT );
  printf("The loaded oniguruma is '%d.%d.%d'.\n"
    , ONIGURUMA_VERSION_MAJOR
    , ONIGURUMA_VERSION_MINOR
    , ONIGURUMA_VERSION_TEENY );
  return ;
}


static void print_debug()
{
  int x;

  print_version();
  print_syn_data_list();
  print_flag_data_list();
  check_flag_data_duplication();

  /*  print: (1U << x) ===> flag_data_list[elem] */
  PRINT_SEPARATOR;
  printf("\nThe following are the array elements that correspond to each bit.\n\n");
  for ( x=0; (x < TOTAL_NUM_OF_BITS); x++)
  {
    convert_bit_shift_num_to_flag_data_list_element(x);
  }
  return ;
}


static void warn_undefined_flag_used()
{
  int x, y, elem;

  for ( x=0; x < TOTAL_NUM_OF_BITS; x++)
  {
    elem = convert_bit_shift_num_to_flag_data_list_element(x);

    if (elem < 0){
      for ( y=0; y < num_of_syntax_types; y++)
      {
        if ( IS_SYNTAX_MEMBER( syn_data_list[y].syn, (1U << x) ) )
        {
          PRINT_SEPARATOR;
          printf("warning: An undefined bit flag is used.");
          printf("  (1U << %d)\n\n", x);
          print_set_in_one_line( x );

#ifndef PRINT_UNDEFINED_FLAG
          printf("Please use '#define PRINT_UNDEFINED_FLAG'.\n");
#endif
          return ;
        }
      }   /*  for y  */
    }   /*  if elem */
  }   /*  for x  */
  return ;
}


extern int main(int argc, char* argv[])
{
#ifdef PRINT_DEBUG_INFO
  print_debug();
  return 0;
#endif

#ifdef PRINT_VERSION_INFO
  print_version();
#endif

  flag_name_max_len = flag_name_max_len_int();
  check_flag_data_duplication();

#ifdef PRINT_SET_IN_INFO
  print_set_in();
#endif

#ifdef PRINT_TABLE_INFO
  print_table_head();
  print_table_body();
#endif

#ifdef WARN_UNDEFINED_FLAG_USED
  warn_undefined_flag_used();
#endif

  /*  test  */
  /*
  printf("\n------  test -------\n");

  int test_x = 17;
  print_set_in_one_line( test_x );
  print_table_body_one_line( test_x );
  printf("--------------------\n");
  */

  /*  To avoid 'gcc -Wall' warnings.  */
  if (0)
  {
    print_debug();
    print_version();
    print_set_in();
    print_table_head();
    print_table_body();
    warn_undefined_flag_used();
  }

  return 0;
}


