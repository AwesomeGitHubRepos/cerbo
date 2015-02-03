#include <stdio.h>

#include "data.h"

FILE *fofx;

char *part1 =
"OFXHEADER:100\n"
"DATA:OFXSGML\n"
"VERSION:102\n"
"SECURITY:NONE\n"
"ENCODING:USASCII\n"
"CHARSET:1252\n"
"COMPRESSION:NONE\n"
"OLDFILEUID:NONE\n"
"NEWFILEUID:NONE\n"
"\n"
"<OFX>\n"
"<SIGNONMSGSRSV1>\n"
"<SONRS>\n"
"<STATUS>\n"
"<CODE>0\n"
"<SEVERITY>INFO\n"
"</STATUS>\n"
"<DTSERVER>20140615113211.090\n"
"<LANGUAGE>ENG\n"
"</SONRS>\n"
"</SIGNONMSGSRSV1>\n"
"<INVSTMTMSGSRSV1>\n"
"<INVSTMTTRNRS>\n"
  //"<TRNUID>1001\n"
"<STATUS>\n"
"<CODE>0\n"
"<SEVERITY>INFO\n"
"</STATUS>\n"
"<INVSTMTRS>\n"
"<DTASOF>20140615113211.090\n"
"<CURDEF>GBP\n"
"<INVACCTFROM>\n"
"<BROKERID>google.com\n"
"<ACCTID>My Portfolio\n"
"</INVACCTFROM>\n"
"<INVTRANLIST>\n"
"<DTSTART>19691210020252.800\n"
"<DTEND>19691210020252.800\n"
;


char *part2 =
"</INVTRANLIST>\n"
"</INVSTMTRS>\n"
"</INVSTMTTRNRS>\n"
"</INVSTMTMSGSRSV1>\n"
"<SECLISTMSGSRSV1>\n"
"<SECLIST>\n"
;


char *part3 =
"</SECLIST>\n"
"</SECLISTMSGSRSV1>\n"
"</OFX>\n"
;

void out(char *str)
{
	fprintf(fofx, "%s\n", str);
}

void outc(char *str) { fprintf(fofx, "%s", str); }
void outs(char *s1, char *s2) { fprintf(fofx, "%s%s\n", s1, s2); }

void uniqueid(comm *c) { fprintf(fofx, "<UNIQUEID>%s:%s\n<UNIQUEIDTYPE>TICKER\n", 
				 c->exchange, c->ticker); }

void buystock(comm *c)
{

	out("<BUYSTOCK>");
	out("<INVBUY>");
	out("<INVTRAN>");
	//out("<FITID>1");
	//out("<DTTRADE>19691210020252.800");
	out("<MEMO>");
	out("</INVTRAN>");
	out("<SECID>");

	uniqueid(c);


	out("</SECID>");
	//out("<UNITS>10");
	double qty = 0.0;
	double ucost = 0.0;
	if(c->type == 'Y') {
		qty = c->end_qty;
		ucost = c->ucost;
	}
	fprintf(fofx, "<UNITS>%.5f\n", qty);
	fprintf(fofx, "<UNITPRICE>%.5f\n", ucost);
	fprintf(fofx, "<TOTAL>-%.5f\n", qty * ucost);
	//}

	out("<SUBACCTSEC>CASH");
	out("<SUBACCTFUND>CASH");
	out("</INVBUY>");
	out("<BUYTYPE>BUY");
	out("</BUYSTOCK>");

}


void stockinfo(comm *c)
{
	out("<STOCKINFO>");
	out("<SECINFO>");
	out("<SECID>");
	uniqueid(c);
	out("</SECID>");
	//out("<SECNAME>hello FTSE AIM All-Share Index");
	outs("<SECNAME>", c->name);
	//out("<TICKER>AXX");
	outs("<TICKER>", c->ticker);
	out("</SECINFO>");
	out("</STOCKINFO>");

}

bool include(comm *c)
{
	if(c->type == 'O') return false;
	if(c->type == 'I') return true;
	return c->end_qty != 0;
}
void create_ofx()
{
	int i;
	fofx = fout_cache("ofx.ofx");
	out(part1);

	comm *c;
	while(c = linode(&comms)) if(include(c)) buystock(c);
	out(part2);
	while(c = linode(&comms)) if(include(c)) stockinfo(c);
	out(part3);
	fclose(fofx);

}
