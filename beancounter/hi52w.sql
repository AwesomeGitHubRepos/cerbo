start transaction;
create sequence sp_seq;
alter table stockprices add column sp_id serial;
alter table stockprices alter column sp_id set default nextval('sp_seq');
alter table stockprices alter column sp_id set not null;
create index sp_id_idx on table stockprices (sp_id);
commit;


create type sp_type as (ticker varchar(12), dstamp date, price real);

create or replace function hi52w_for(ticker character varying(12), dstamp date)
returns sp_type as $$
declare
	result sp_type;
begin
	select p.symbol, p.date, p.day_close into result
	from stockprices p
	where p.symbol = ticker
	and p.date <= dstamp and p.date > (dstamp - interval '1' year)
	order by p.day_close desc, p.date desc
	limit 1;

	return result;

end
$$ language 'plpgsql';



create or replace view hi52w as
--select  t1.symbol, t1.date, t1.day_close
select *, hi52w_for(p.symbol, p.date)
from stockprices p;

