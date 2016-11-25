begin;
drop table if exists temp_prices;

create temp table temp_prices (
	dstamp date,
	open real,
	high real,
	low real, 
	cls real,
	volume numeric,
	aclose real
);
\copy temp_prices from 'raw.dat'  delimiter ',' csv header;

\! basename `pwd` >/tmp/upsert-sp.txt
create temp table temp_sym ( sym varchar(12));
\copy temp_sym from '/tmp/upsert-sp.txt';

create temp view temp_comb as select  * from temp_sym, temp_prices;

update stockprices
set day_close = cls, day_open = open, day_high = high, day_low = low
from temp_comb
where symbol = sym and date = dstamp;


commit;
