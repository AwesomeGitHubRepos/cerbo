/* created by mcarter
27-Oct-2013
 */

enum yytokentype {
	CMD = 258,
	DOUBLE,
	STRING ,
	NEWLINE 
};

typedef union YYSTYPE
{
   double dval;
   char *string;
} YYSTYPE;

# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1


extern YYSTYPE yylval;
