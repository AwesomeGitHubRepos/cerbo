begin transaction;
create or replace function qat(dstamp date)
		returns table (symbol varchar(16), qty real)
		as
		$body$
		SELECT symbol, sum(shares)
		FROM 
		  portfolio
		  where date <= dstamp
		  GROUP BY
		  symbol
		  order by symbol;
		$body$
		language sql;
commit;		

