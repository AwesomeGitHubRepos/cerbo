-- return the closing price for s symbol on or before a given date
-- Example usage: 
--     select beanat('ZYT.L', '2016-12-31');
create or replace function beanat(sym character varying(12), dstamp date)
returns text as
$$
declare
	strresult text;
	blah text;
	ds text;
begin
	select day_close  || E'\t' || date || E'\t' || symbol
	into blah
       	from stockprices where symbol = sym and date <= dstamp
	order by date desc limit 1;
	strresult := blah;
	return strresult;
end;
$$
language 'plpgsql';
