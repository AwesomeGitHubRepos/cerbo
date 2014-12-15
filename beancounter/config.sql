/* install using: 
   psql -d beancounter  -f config.sql
*/

-- single line comment

/* spoob - stock price on or before
   returns a date and a price on or before a date for a given symbol
   E.g.
   select * from spoob('RBS.L', '2009-11-18');
    date    | day_close 
------------+-----------
 2009-11-18 |    5342.1

*/


CREATE TABLE stockinfo
(
  id SERIAL PRIMARY KEY,
  symbol character varying(12) NOT NULL DEFAULT ''::character varying,
  name character varying(64) NOT NULL DEFAULT ''::character varying,
  exchange character varying(16) NOT NULL DEFAULT ''::character varying,
  capitalisation real,
  low_52weeks real,
  high_52weeks real,
  earnings real,
  dividend real,
  p_e_ratio real,
  avg_volume integer,
  active boolean DEFAULT true
  
)




CREATE OR REPLACE FUNCTION spoob(symb text, dstamp DATE)
returns table(date date, day_close real)
as
$$
select date, day_close from stockprices where symbol = symb and date <= dstamp
order by dstamp desc limit 1
$$
language sql;


/* Example usage:
select plspoob('RBS.L', '2014-11-17');
*/
CREATE OR REPLACE FUNCTION plspoob(sym text, dstamp date)
returns real
as $$
--declare dummy real;
declare ret real;
begin
--select into dummy, ret where (select day_close from spoob(sym, dstamp));
select day_close into ret from spoob(sym, dstamp);
return ret;
end;
$$ language plpgsql;

CREATE OR REPLACE FUNCTION pltest()
returns void
as $$
begin
raise info 'pltest says hello. 3';
end;
$$ language plpgsql





