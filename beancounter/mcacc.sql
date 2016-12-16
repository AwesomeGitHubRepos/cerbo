begin transaction;

drop schema if exists mcacc cascade;

create schema mcacc;

create table mcacc.xtrans (
	dstamp date,
	acc varchar(6),
	amount money,
	explan varchar(40)
);

--create or replace function mcacc.import_xtrans()
--returns void as
--$$
--begin
--	delete from mcacc.xtrans;

--	copy mcacc.xtrans from '/home/mcarter/.mcacc/work/s3/pgposts.csv' delimiter ',';
--end;	
--$$
--language 'sql';
--language 'plpgsql';

--create or replace function mcacc.vacc(acc_name varchar(6))
--returns table(dstamp date, explan varchar(40), amount money, runt money) as
--$$
--with t_xtrans as
--(select dstamp, explan, amount
--	from mcacc.xtrans
--	where acc = acc_name
--)
--select
--dstamp, explan, amount, sum(t_xtrans.amount)  over (order by t_xtrans.dstamp asc)
--from t_xtrans;
--$$
--language 'sql';

create or replace function mcacc.vacc(acc_name varchar(6))
returns table(dstamp date, explan varchar(40), amount money, runt money) as
$$
--begin
--	set search_path to mcacc;
with t_xtrans as
(select dstamp, explan, amount, row_number() over (order by dstamp) as rnum
	from mcacc.xtrans
	where acc = acc_name
--	order by dstamp asc
)
select
dstamp, explan, amount, sum(t_xtrans.amount)  over (order by t_xtrans.rnum)
from t_xtrans
order by dstamp asc;
--end;
$$
--language 'plpgsql';
language 'sql';




commit;
